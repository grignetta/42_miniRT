/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:45:58 by mmasarov          #+#    #+#             */
/*   Updated: 2024/10/29 16:45:59 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	free_close(t_canvas *canvas, int fd)
{
	if (fd != -1)
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
}

void	free_everything(t_scene scene, t_canvas *canvas, int fd)
{
	if (fd != -1)
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
}

void	free_scene(t_scene *sc)
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
	t_scene		*scene;

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
	if (scene)
		free_scene(scene);
	exit(0);
	return (0);
}
