#include "minirt.h"

void set_camera(scene *scene)
{
    camera *camera;

    camera = &scene->camera;
    //double viewport_size = 1.0;
    camera->viewport_size = 2 * tan(camera->fov * M_PI / 360.0);
    camera->projection_plane_d = camera->viewport_size;//1.0; should be same as vp size, otherwise a fisheye effect
}