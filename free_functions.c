#include "minirt.h"

int	scene_success(scene scene, t_canvas *canvas, int fd)
{
	if (scene.success)
	{
		close(fd);
		if (canvas)
		{
			if (canvas->img)
            	mlx_destroy_image(canvas->mlx_ptr, canvas->img);
        	if (canvas->win_ptr)
			{
            	mlx_destroy_window(canvas->mlx_ptr, canvas->win_ptr);
				mlx_destroy_display(canvas->mlx_ptr);
			}
        	if (canvas->mlx_ptr)
            	free(canvas->mlx_ptr);
        	free(canvas);
    	}
		free_scene(&scene);
    	return (1);
    }
	return (0);
}

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

int	key_handle(int keysym, void *param)
{
	t_canvas	*canvas;

	canvas = (t_canvas *)param;
	if (keysym == XK_Escape)
		close_event(canvas);
	return (0);
}

int	close_event(void *param)
{
    t_canvas	*canvas;
	scene		*scene;
	
	canvas = (t_canvas *)param;
	scene = canvas->scene;
    if (canvas)
	{
		if (canvas->img)
            mlx_destroy_image(canvas->mlx_ptr, canvas->img);
        if (canvas->win_ptr)
		{
            mlx_destroy_window(canvas->mlx_ptr, canvas->win_ptr);
			mlx_destroy_display(canvas->mlx_ptr);
		}
        if (canvas->mlx_ptr)
            free(canvas->mlx_ptr);
        free(canvas);
    }
	if (scene) //check if ok like this
		free_scene(scene);
    exit(0);
    return (0);
}
