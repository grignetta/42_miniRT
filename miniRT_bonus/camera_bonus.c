#include "minirt_bonus.h"

void	set_camera(t_scene *scene)
{
	t_camera	*camera;

	camera = &scene->camera;
	camera->vp_size = 2 * tan(camera->fov * M_PI / 360.0);
	camera->vp_d = camera->vp_size;
}
