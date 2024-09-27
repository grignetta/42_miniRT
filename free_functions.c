#include "minirt.h"

void	free_scene(scene *sc)
{
	if (sc->spheres)
	{
		free(sc->spheres);
		sc->spheres = NULL;
	}
	if (sc->cylinders)
	{
		free(sc->cylinders);
		sc->cylinders = NULL;
	}
	if (sc->planes)
	{
		free(sc->planes);
		sc->planes = NULL;
	}
	if (sc->lights)
	{
		free(sc->lights);
		sc->lights = NULL;
	}
	sc->success = 1;
}

int	close_event(void *param)
{
    t_canvas *canvas = (t_canvas *)param;
    if (canvas)
	{
        if (canvas->win_ptr) {
            mlx_destroy_window(canvas->mlx_ptr, canvas->win_ptr);
        }
        if (canvas->mlx_ptr)
            free(canvas->mlx_ptr);
        free(canvas);
    }
    exit(0);
    return (0);
}