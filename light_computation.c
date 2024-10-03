#include "minirt.h"

int	is_in_shadow(t_scene *scene, t_trace vars, t_vector L, double t_max)
{
	t_ray_params		shadow_params;
	t_intersect_result	shadow_result;

	shadow_params.o = vars.p;
	shadow_params.d = L;
	shadow_params.t_min = 0.001;
	shadow_params.t_max = t_max;
	shadow_result = closest_intersection(scene, shadow_params);
	if (shadow_result.t < shadow_params.t_max)
		return (1);
	return (0);
}

void	diffuse_light(t_color *result, t_light light, t_trace vars, t_vector L)
{
	double	n_dot_l;
	double	diffuse_intensity;

	n_dot_l = vector_dot(vars.n, L);
	if (n_dot_l > 0)
	{
		diffuse_intensity = light.intensity * n_dot_l
			/ (vector_length(vars.n) * vector_length(L));
		add_light(result, light, diffuse_intensity);
	}
}

void	specular_reflection(t_color *result, t_light light,
			t_trace vars, t_vector L)
{
	t_vector	vr;
	double		r_dot_v;
	double		specular_intensity;

	if (vars.shape->specular > 5)
	{
		vr = vector_reflect(L, vars.n);
		r_dot_v = vector_dot(vr, vars.v);
		if (r_dot_v > 0)
		{
			specular_intensity = light.intensity
				* pow(r_dot_v / (vector_length(vr) * vector_length(vars.v)),
					vars.shape->specular);
			add_light(result, light, specular_intensity);
		}
	}
}

t_color	color_init(double red, double green, double blue)
{
	t_color	c;

	c.red = red;
	c.green = green;
	c.blue = blue;
	return (c);
}

t_color	compute_lighting(t_scene *scene, t_trace vars)
{
	t_color		result;
	t_light		light;
	double		t_max;
	t_vector	l;
	int			i;

	result = color_init(0.0, 0.0, 0.0);
	i = -1;
	while (++i < scene->light_count)
	{
		light = scene->lights[i];
		if (light.type == 0) // Ambient
			ambient_light(&result, light);
		else
		{
			l = get_light_direction(light, vars, &t_max);
			if (is_in_shadow(scene, vars, l, t_max))
				continue; // In shadow, skip this light
			diffuse_light(&result, light, vars, l);
			specular_reflection(&result, light, vars, l);
		}
	}
	// Ensure the values are within the 0-1 range
	check_limit_double(&result.red, 1.0);
	check_limit_double(&result.green, 1.0);
	check_limit_double(&result.blue, 1.0);
	return (result);
}

/* color compute_lighting(scene *scene, trace vars)//vector p, vector n, vector v, int specular)
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
				L = vector_sub(light.position, vars.p);
				t_max = 1.0; //left in case we add directional light
			}
			else
			{ // Directional
			   L = light.direction;
			   t_max = INFINITY;
			}
			//Check for shadows
			shadow_params.o = vars.p;
			shadow_params.d = L;
			shadow_params.t_min = 0.001;
			shadow_params.t_max = t_max;

			intersect_result shadow_result = closest_intersection(scene, shadow_params);
			//if (shadow_sphere != NULL) continue;
			if (shadow_result.t < shadow_params.t_max)
				continue; // In shadow, skip this light
			// Diffuse lighting
			double n_dot_l = vector_dot(vars.n, L);
			if (n_dot_l > 0)
			{
				double diffuse_intensity = light.intensity * n_dot_l / (vector_length(vars.n) * vector_length(L));
				add_light(&result, t_light, diffuse_intensity);
			}
			// Specular reflection
			//if (specular != -1) {
			if (vars.shape->specular > 5) //maybe make if specular > 5
			{
				vector vr = vector_reflect(L, vars.n);
				double r_dot_v = vector_dot(vr, vars.v);
				if (r_dot_v > 0)
				{
					double specular_intensity = light.intensity * pow(r_dot_v / (vector_length(vr) * vector_length(vars.v)), vars.shape->specular);
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