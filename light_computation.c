#include "minirt.h"

void add_light(color *result, light light, double intensity)
{
    result->red += intensity * light.red / 255.0;
    result->green += intensity * light.green / 255.0;
    result->blue += intensity * light.blue / 255.0;
}

void ambient_light(color *result, light light)//added just for more clarity
{
    add_light(result, light, light.intensity);
}

vector get_light_direction(light light, trace vars, double *t_max)
{
    vector L;

    // if (light.type == 1) // Point
    // {
        L = vector_sub(light.position, vars.P);
        *t_max = 1.0;
    // }
    // else // Directional
    // {
    //     L = light.direction;
    //     *t_max = INFINITY;
    // }
    return L;
}

int is_in_shadow(scene *scene, trace vars, vector L, double t_max)
{
    ray_params shadow_params;
	intersection_result shadow_result;

    shadow_params.O = vars.P;
    shadow_params.D = L;
    shadow_params.t_min = 0.001;
    shadow_params.t_max = t_max;
    shadow_result = closest_intersection(scene, shadow_params);
	if (shadow_result.t < shadow_params.t_max)
		return (1);
	return (0);
}

void diffuse_light(color *result, light light, trace vars, vector L)
{
	double n_dot_l;
	double diffuse_intensity;

    n_dot_l = vector_dot(vars.N, L);
    if (n_dot_l > 0)
    {
        diffuse_intensity = light.intensity * n_dot_l
			/ (vector_length(vars.N) * vector_length(L));
        add_light(result, light, diffuse_intensity);
    }
}

void specular_reflection(color *result, light light, trace vars, vector L)
{
	vector R;
	double r_dot_v;
	double specular_intensity;

    if (vars.shape->specular > 5)
    {
        R = vector_reflect(L, vars.N);
        r_dot_v = vector_dot(R, vars.V);
        if (r_dot_v > 0)
        {
            specular_intensity = light.intensity
				* pow(r_dot_v / (vector_length(R) * vector_length(vars.V)),
				vars.shape->specular);
            add_light(result, light, specular_intensity);
        }
    }
}

color compute_lighting(scene *scene, trace vars)
{
    color result = {0.0, 0.0, 0.0};//change initialization
	light light;
	double t_max;
	vector L;
	int i;

    i = -1;
    while (++i < scene->light_count)
    {
        light = scene->lights[i];
        if (light.type == 0) // Ambient
            ambient_light(&result, light);
        else
        {
            L = get_light_direction(light, vars, &t_max);
            if (is_in_shadow(scene, vars, L, t_max))
                continue; // In shadow, skip this light
            diffuse_light(&result, light, vars, L);
            specular_reflection(&result, light, vars, L);
        }
    }
	// Ensure the values are within the 0-1 range
    check_limit_double(&result.red, 1.0);
    check_limit_double(&result.green, 1.0);
    check_limit_double(&result.blue, 1.0);
    return (result);
}

/* color compute_lighting(scene *scene, trace vars)//vector P, vector N, vector V, int specular)
{
    ray_params shadow_params;
    int i;

    color result = {0.0, 0.0, 0.0};
    i = -1;
    while (++i < scene->light_count)
	{
        light light = scene->lights[i];

        if (light.type == 0) // Ambient
            add_light(&result, light, light.intensity);
        else
        {
            vector L;
            double t_max;
            if (light.type == 1)
			{ // Point
                L = vector_sub(light.position, vars.P);
                t_max = 1.0; //left in case we add directional light
            }
			else
			{ // Directional
               L = light.direction;
               t_max = INFINITY;
            }
            //Check for shadows
            shadow_params.O = vars.P;
            shadow_params.D = L;
            shadow_params.t_min = 0.001;
            shadow_params.t_max = t_max;

            intersection_result shadow_result = closest_intersection(scene, shadow_params);
            //if (shadow_sphere != NULL) continue;
            if (shadow_result.t < shadow_params.t_max)
                continue; // In shadow, skip this light
            // Diffuse lighting
            double n_dot_l = vector_dot(vars.N, L);
            if (n_dot_l > 0)
            {
                double diffuse_intensity = light.intensity * n_dot_l / (vector_length(vars.N) * vector_length(L));
                add_light(&result, light, diffuse_intensity);
            }
            // Specular reflection
            //if (specular != -1) {
            if (vars.shape->specular > 5) //maybe make if specular > 5
            {
                vector R = vector_reflect(L, vars.N);
                double r_dot_v = vector_dot(R, vars.V);
                if (r_dot_v > 0)
                {
                    double specular_intensity = light.intensity * pow(r_dot_v / (vector_length(R) * vector_length(vars.V)), vars.shape->specular);
                    add_light(&result, light, specular_intensity);
                }
            }
        }
    }

    // Ensure the values are within the 0-1 range
    check_limit_double(&result.red, 1.0);
    check_limit_double(&result.green, 1.0);
    check_limit_double(&result.blue, 1.0);
    return (result);
} */