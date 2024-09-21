#include "minirt.h"

// Closest intersection function
intersection_result closest_intersection(scene *scene, ray_params params)// vector O, vector D, double t_min, double t_max)
{
    intersection_result result = {0};
    result.t = params.t_max;

    // Check spheres
    for (int i = 0; i < scene->sphere_count; i++) {
        double t1, t2;
        if (intersect_ray_sphere(params.O, params.D, &scene->spheres[i], &t1, &t2)) {
            if (t1 < result.t && t1 > params.t_min) {
                result.t = t1;
                result.type = SHAPE_SPHERE;
                result.object = &scene->spheres[i];
            }
            if (t2 < result.t && t2 > params.t_min) {
                result.t = t2;
                result.type = SHAPE_SPHERE;
                result.object = &scene->spheres[i];
            }
        }
    }

    // Check cylinders
    for (int i = 0; i < scene->cylinder_count; i++) {
        double t1, t2;
        if (intersect_ray_cylinder(params.O, params.D, &scene->cylinders[i], &t1, &t2)) {
            if (t1 < result.t && t1 > params.t_min) {
                result.t = t1;
                result.type = SHAPE_CYLINDER;
                result.object = &scene->cylinders[i];
            }
            if (t2 < result.t && t2 > params.t_min) {
                result.t = t2;
                result.type = SHAPE_CYLINDER;
                result.object = &scene->cylinders[i];
            }
        }
    }

    // Check planes
    for (int i = 0; i < scene->plane_count; i++) {
        double t;
        if (intersect_ray_plane(params.O, params.D, &scene->planes[i], &t)) {
            if (t < result.t && t > params.t_min) {
                result.t = t;
                result.type = SHAPE_PLANE;
                result.object = &scene->planes[i];
            }
        }
    }

    return result;
}

color compute_lighting(scene *scene, vector P, vector N, vector V, int specular)
{
    ray_params shadow_params;

    color result = {0.0, 0.0, 0.0};

    for (int i = 0; i < scene->light_count; i++)
	{
        light light = scene->lights[i];

        if (light.type == 0) // Ambient
        {
            result.red += light.intensity * light.red / 255.0;
            result.green += light.intensity * light.green / 255.0;
            result.blue += light.intensity * light.blue / 255.0;
        }
        else
        {
            vector L;
            double t_max;
            //if (light.type == 1)
			//{ // Point
                L = vector_sub(light.position, P);
                t_max = 1.0; //left in case we add directional light
            //}
			//else
			//{ // Directional
            //    L = light.direction;
            //    t_max = INFINITY;
            //}
            // Check for shadows
            //double shadow_t;
            //sphere *shadow_sphere =
            shadow_params.O = P;
            shadow_params.D = L;
            shadow_params.t_min = 0.001;
            shadow_params.t_max = t_max;

            intersection_result shadow_result = closest_intersection(scene, shadow_params);//P, L, 0.001, t_max);//, &shadow_t);
            //if (shadow_sphere != NULL) continue;
            if (shadow_result.t < shadow_params.t_max)
                continue; // In shadow, skip this light
            // Diffuse lighting
            double n_dot_l = vector_dot(N, L);
            if (n_dot_l > 0)
            {
                double diffuse_intensity = light.intensity * n_dot_l / (vector_length(N) * vector_length(L));
                result.red += diffuse_intensity * light.red / 255.0;
                result.green += diffuse_intensity * light.green / 255.0;
                result.blue += diffuse_intensity * light.blue / 255.0;
            }
            // Specular reflection
            //if (specular != -1) { //maybe make if specular > 5
            if (specular > 5) {
                vector R = vector_reflect(L, N);
                double r_dot_v = vector_dot(R, V);
                if (r_dot_v > 0)
                {
                    double specular_intensity = light.intensity * pow(r_dot_v / (vector_length(R) * vector_length(V)), specular);
                    result.red += specular_intensity * light.red / 255.0;
                    result.green += specular_intensity * light.green / 255.0;
                    result.blue += specular_intensity * light.blue / 255.0;
                }
            }
        }
    }

    // Ensure the values are within the 0-1 range
    result.red = result.red > 1.0 ? 1.0 : result.red;
    result.green = result.green > 1.0 ? 1.0 : result.green;
    result.blue = result.blue > 1.0 ? 1.0 : result.blue;

    return result;
}

// Main rendering loop
void render(t_canvas *canvas, scene *scene)
{
    ray_params params;
    camera *camera = &scene->camera;
    //double viewport_size = 1.0;
    double viewport_size = 2 * tan(camera->fov * M_PI / 360.0);
    double projection_plane_d = viewport_size;//1.0; should be same as vp size, otherwise a fisheye effect
    //vector camera_position = {0, 0, -5}; // change to camera stuct
    vector camera_position = scene->camera.position;

    for (int x = -canvas->win_width / 2; x < canvas->win_width / 2; x++) {
        for (int y = -canvas->win_height / 2; y < canvas->win_height / 2; y++) {
            vector D = {x * viewport_size / canvas->win_width, y * viewport_size / canvas->win_height, projection_plane_d};
            D = vector_normalize(D); // Normalize the ray direction
            // Adjust ray direction based on camera's orientation
            D = vector_add(D, camera->orientation);  // Align with camera's orientation
            params.O = camera_position;
            params.D = D;
            params.t_min = 1.0;
            params.t_max = INFINITY;
            int color = trace_ray(scene, params, 3);
            put_pixel(canvas, x + canvas->win_width / 2, canvas->win_height / 2 - y, color);
        }
    }
    mlx_put_image_to_window(canvas->mlx_ptr, canvas->win_ptr, canvas->img, 0, 0);
}

// Example scene setup
scene create_scene() {
    scene scene;
    scene.sphere_count = 4;
    scene.spheres = malloc(sizeof(sphere) * scene.sphere_count);

    // Red sphere
    scene.spheres[0] = (sphere){{0, -1, 3}, 1, {255, 0, 0, 500, 0.2}};
    // Blue sphere
   // scene.spheres[1] = (sphere){{-2, 0, 4}, 1, 0, 0, 255, 10, 0.3};
    // Green sphere
    scene.spheres[2] = (sphere){{2, 0, 4}, 1, {0, 255, 0, 10, 0.4}};
    // Yellow large sphere (floor)
    //scene.spheres[3] = (sphere){{0, -5001, 0}, 5000, 255, 255, 0, 1000, 0.5};

    // Example plane
    scene.plane_count = 1;
    scene.planes = malloc(sizeof(plane) * scene.plane_count);
    scene.planes[0] = (plane){{0, -1, 0}, {0, 1, 0}, {255, 255, 0, 500, 0.5}};

    scene.cylinder_count = 1;
    scene.cylinders = malloc(sizeof(cylinder) * scene.cylinder_count);

    // Example cylinder
    scene.cylinders[0] = (cylinder){{-1, -1, 4}, 1, 2, {0, 1, 0}, {0, 0, 255, 500, 0.3}};


    scene.light_count = 3;
    scene.lights = malloc(sizeof(light) * scene.light_count);

    // Ambient
	scene.lights[0] = (light){0, 0.2, {0, 0, 0}, {0, 0, 0}, 255, 255, 255};
    // Point light
    scene.lights[1] = (light){1, 0.6, {2, 1, 0}, {0, 0, 0}, 255, 255, 255};//point light also can have its own color (for bonus)
    // Directional light
   // scene.lights[2] = (light){2, 0.2, {0, 0, 0}, {1, 4, 4}, 245, 144, 144};//if ambient and point lights have different colors, what to write here?

     // Initialize camera (you will replace these values after reading the .rt file)
    scene.camera.position = (vector){0, 0, -5}; // Example values
    scene.camera.orientation = (vector){0.0, 0.0, 0.0}; // Example values
    scene.camera.fov = 100.0; // Example value

    return scene;
}