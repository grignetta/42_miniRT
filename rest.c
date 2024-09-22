#include "minirt.h"

void add_light(color *result, light light, double intensity)
{
    result->red += intensity * light.red / 255.0;
    result->green += intensity * light.green / 255.0;
    result->blue += intensity * light.blue / 255.0;
}

color compute_lighting(scene *scene, trace vars)//vector P, vector N, vector V, int specular)
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
            // if (light.type == 1)
			// { // Point
                L = vector_sub(light.position, vars.P);
                t_max = 1.0; //left in case we add directional light
            // }
			// else
			// { // Directional
            //    L = light.direction;
            //    t_max = INFINITY;
            // }
            // Check for shadows
            //double shadow_t;
            //sphere *shadow_sphere =
            shadow_params.O = vars.P;
            shadow_params.D = L;
            shadow_params.t_min = 0.001;
            shadow_params.t_max = t_max;

            intersection_result shadow_result = closest_intersection(scene, shadow_params);//P, L, 0.001, t_max);//, &shadow_t);
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
            //if (specular != -1) { //maybe make if specular > 5
            if (vars.shape->specular > 5) {
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
    // result.red = result.red > 1.0 ? 1.0 : result.red;
    // result.green = result.green > 1.0 ? 1.0 : result.green;
    // result.blue = result.blue > 1.0 ? 1.0 : result.blue;
    return (result);
}

// Function to create a vector
vector create_vector(double x, double y, double z)
{
    vector v;
    v.x = x;
    v.y = y;
    v.z = z;
    return (v);
}


// Main rendering loop
void render(t_canvas *canvas, scene *scene, camera *camera)
{
    ray_params params;
    int x;
    int y;
    vector D;
    int color;

    x = -canvas->win_width / 2 - 1;//-1 for while loop shrinking
    while (++x < canvas->win_width / 2)
    {
        y = -canvas->win_height / 2;//-1 for while loop shrinking
        while (y < canvas->win_height / 2)
        {
            D = create_vector(x * camera->viewport_size / canvas->win_width,
                y * camera->viewport_size / canvas->win_height,
                camera->projection_plane_d);
            D = vector_normalize(D); // Normalize the ray direction
            // Adjust ray direction based on camera's orientation
            D = vector_add(D, camera->orientation);  // Align with camera's orientation
            params.O = camera->position;
            params.D = D;
            params.t_min = 1.0;
            params.t_max = INFINITY;
            color = trace_ray(scene, params, 3);
            put_pixel(canvas, x + canvas->win_width / 2, canvas->win_height / 2 - y, color);
            y++;
        }
        //x++;
    }
    mlx_put_image_to_window(canvas->mlx_ptr, canvas->win_ptr, canvas->img, 0, 0);
}

// Example scene setup
scene create_scene() {
    scene scene;
    scene.sphere_count = 4;
    scene.spheres = malloc(sizeof(sphere) * scene.sphere_count);

    // Red sphere
    scene.spheres[0] = (sphere){{0, -1, 3}, 1, {255, 0, 0, 500, 0.2}, 0};
    // Blue sphere
   // scene.spheres[1] = (sphere){{-2, 0, 4}, 1, 0, 0, 255, 10, 0.3};
    // Green sphere
    scene.spheres[2] = (sphere){{2, 0, 4}, 1, {0, 255, 0, 10, 0.4}, 0};
    // Yellow large sphere (floor)
    //scene.spheres[3] = (sphere){{0, -5001, 0}, 5000, 255, 255, 0, 1000, 0.5};

    // Example plane
    scene.plane_count = 1;
    scene.planes = malloc(sizeof(plane) * scene.plane_count);
    scene.planes[0] = (plane){{0, -1, 0}, {0, 1, 0}, {255, 255, 0, 500, 0.5}, 2};

    scene.cylinder_count = 1;
    scene.cylinders = malloc(sizeof(cylinder) * scene.cylinder_count);

    // Example cylinder
    scene.cylinders[0] = (cylinder){{-1, -1, 4}, 1, 2, {0, 1, 0}, {0, 0, 255, 500, 0.3}, 1};


    scene.light_count = 3;
    scene.lights = malloc(sizeof(light) * scene.light_count);

    // Ambient
	scene.lights[0] = (light){0, 0.2, {0, 0, 0}, {0, 0, 0}, 255, 255, 255};
    // Point light
    scene.lights[1] = (light){1, 0.6, {2, 1, 0}, {0, 0, 0}, 255, 255, 255};//point light also can have its own color (for bonus)
    // Directional light
    //scene.lights[2] = (light){2, 0.2, {0, 0, 0}, {1, 4, 4}, 245, 144, 144};//if ambient and point lights have different colors, what to write here?

     // Initialize camera (you will replace these values after reading the .rt file)
    scene.camera.position = (vector){0, 0, -5}; // Example values
    scene.camera.orientation = (vector){0.0, 0.0, 0.0}; // Example values
    scene.camera.fov = 100.0; // Example value

    return scene;
}