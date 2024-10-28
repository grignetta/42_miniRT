#include "minirt.h"

// Function to create a t_vector
t_vector	vector_init(double x, double y, double z)
{
	t_vector	v;

	v.x = x;
	v.y = y;
	v.z = z;
	return (v);
}

// Main rendering loop
void	render(t_canvas *canvas, t_scene *scene, t_camera *camera)
{
	t_ray_params	params;
	int			x;
	int			y;
	t_vector		d;
	int			color;

	x = -canvas->win_width / 2 - 1;//-1 for while loop shrinking
	while (++x < canvas->win_width / 2)
	{
		y = -canvas->win_height / 2;//-1 for while loop shrinking
		while (y < canvas->win_height / 2)
		{
			d = vector_init(x * camera->viewport_size / canvas->win_width,
				y * camera->viewport_size / canvas->win_height,
				camera->projection_plane_d);
			d = vector_normalize(d); // Normalize the ray direction
			// Adjust ray direction based on camera's orientation
			d = vector_add(d, camera->orientation);  // Align with camera's orientation
			params.o = camera->position;
			params.d = d;
			params.t_min = 1; //camera->projection_plane_d;
			params.t_max = INFINITY;
			color = trace_ray(scene, params, 5);//5 is better for mirror effect
			put_pixel(canvas, x + canvas->win_width / 2, canvas->win_height / 2 - y, color);
			y++;
		}
		//x++;
	}
	mlx_put_image_to_window(canvas->mlx_ptr, canvas->win_ptr, canvas->img, 0, 0);
}

// Example scene setup
t_scene	create_scene(void)
{
	t_scene	scene;

	scene.sphere_count = 2;
	scene.spheres = malloc(sizeof(t_sphere) * scene.sphere_count);

	// Red sphere
	scene.spheres[0] = (t_sphere){{0, -1, 3}, 1, {255, 0, 0, 500, 0.1}, 1};
	// Blue sphere
   // scene.spheres[1] = (t_sphere){{-2, 0, 4}, 1, 0, 0, 255, 10, 0.3};
	// Green sphere
	scene.spheres[1] = (t_sphere){{2, -1, 4}, 1, {0, 255, 0, 500, 0.5}, 1};
	// Yellow large sphere (floor)
	//scene.spheres[3] = (t_sphere){{0, -5001, 0}, 5000, 255, 255, 0, 1000, 0.5};

	// Example plane
	scene.plane_count = 1;
	scene.planes = malloc(sizeof(t_plane) * scene.plane_count);
	scene.planes[0] = (t_plane){{0, -2, 0}, {0, 1, 0}, {255, 255, 0, 30, 0.5}, 2, 1};

	scene.cylinder_count = 3;
	scene.cylinders = malloc(sizeof(t_cylinder) * scene.cylinder_count);

	// Example t_cylinder
	scene.cylinders[0] = (t_cylinder){{-2, -0.5, 4}, 1.5, 2, {0, 1, 0}, {0, 0, 255, 500, 0.2}, 1, {0, 0, 0}, {0, 0, 0}};
	scene.cylinders[1] = (t_cylinder){{3, 0.5, 4}, 1.5, 2, {0, 1, 0}, {0, 0, 255, 500, 0.2}, 1, {0, 0, 0}, {0, 0, 0}};
	scene.cylinders[2] = (t_cylinder){{0, 3, 6}, 1.5, 2, {0, 1, 0}, {0, 0, 255, 500, 0.2}, 1, {0, 0, 0}, {0, 0, 0}};


	scene.light_count = 3;
	scene.lights = malloc(sizeof(t_light) * scene.light_count);

	// Ambient
	scene.lights[0] = (t_light){0, 0.2, {0, 0, 0}, {0, 0, 0}, 255, 162, 57};
	// Point t_light
	scene.lights[1] = (t_light){1, 0.6, {-1.5, -0.5, -3}, {0, 0, 0}, 255, 255, 255};//point t_light also can have its own color (for bonus)
	// Directional t_light
	scene.lights[2] = (t_light){2, 0.6, {0, 0 , 0}, {1, 4, 4}, 255, 255, 255};//if ambient and point lights have different colors, what to write here?
	// Point t_light
	//scene.lights[1] = (t_light){1, 0.6, {0, 0, 0}, {2, 1, 0}, 255, 255, 255};

	 // Initialize camera (you will replace these values after reading the .rt file)
	scene.camera.position = (t_vector){0, 0, -5}; // Example values
	scene.camera.orientation = (t_vector){0.0, 0.0, 0.0}; // Example values
	scene.camera.fov = 100.0;// Example value

	return (scene);
}