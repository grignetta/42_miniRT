#include "minirt.h"

// Function to create a vector
vector vector_init(double x, double y, double z)
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
            D = vector_init(x * camera->viewport_size / canvas->win_width,
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