#include "minirt_bonus.h"

// Function to create a t_vector
t_vector	vector_init(double x, double y, double z)
{
	t_vector	v;

	v.x = x;
	v.y = y;
	v.z = z;
	return (v);
}

void	set_ray_params(t_ray_params *params, t_camera *camera, t_vector d)
{
	params->o = camera->position;
	params->d = d;
	params->t_min = camera->vp_size;
	params->t_max = INFINITY;
}

// Main rendering loop
void	render(t_canvas *canvas, t_scene *scene, t_camera *camera)
{
	t_ray_params	params;
	int				x;
	int				y;
	t_vector		d;
	int				color;

	x = -canvas->win_width / 2 - 1;
	while (++x < canvas->win_width / 2)
	{
		y = -canvas->win_height / 2;
		while (y < canvas->win_height / 2)
		{
			d = vector_init(x * camera->vp_size / canvas->win_width,
					y * camera->vp_size / canvas->win_height, camera->vp_d);
			d = vector_normalize(d);
			d = vector_add(d, camera->orientation);
			set_ray_params(&params, camera, d);
			color = trace_ray(scene, params, 5);
			put_pixel(canvas, x + canvas->win_width / 2,
				canvas->win_height / 2 - y, color);
			y++;
		}
	}
	mlx_put_image_to_window(canvas->mlx_ptr, canvas->win_ptr,
		canvas->img, 0, 0);
}
