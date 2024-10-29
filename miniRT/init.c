#include "minirt.h"

void	init_canvas(t_canvas *canvas)
{
	canvas->win_width = 1000;
	canvas->win_height = 1000;
}

t_canvas	*initialize_matrix(void)
{
	t_canvas	*canvas;

	canvas = (t_canvas *)malloc(sizeof(t_canvas));
	if (!canvas)
		return (NULL);
	init_canvas(canvas);
	return (canvas);
}

int	initialize_graphics(t_canvas *canvas)
{
	canvas->mlx_ptr = mlx_init();
	if (canvas->mlx_ptr == NULL)
		return (1);
	canvas->win_ptr = mlx_new_window(canvas->mlx_ptr,
			canvas->win_width,
			canvas->win_height,
			"miniRT");
	if (canvas->win_ptr == NULL)
		return (mlx_destroy_display(canvas->mlx_ptr), free(canvas->mlx_ptr), 1);
	return (0);
}

int	initialize_image(t_canvas *canvas)
{
	canvas->img = mlx_new_image(canvas->mlx_ptr,
			canvas->win_height,
			canvas->win_width);
	if (canvas->img == NULL)
		return (1);
	canvas->addr = mlx_get_data_addr(canvas->img,
			&canvas->bits_per_pixel,
			&canvas->line_length,
			&canvas->endian);
	if (canvas->addr == NULL)
		return (mlx_destroy_image(canvas->mlx_ptr, canvas->img), 1);
	return (0);
}

t_canvas	*init_mlx(void)
{
	t_canvas	*canvas;

	canvas = initialize_matrix();
	if (canvas == NULL)
		return (NULL);
	if (initialize_graphics(canvas))
		return (free(canvas), NULL);
	if (initialize_image(canvas))
		return (mlx_destroy_window(canvas->mlx_ptr, canvas->win_ptr),
			mlx_destroy_display(canvas->mlx_ptr), free(canvas->mlx_ptr),
			free(canvas), NULL);
	return (canvas);
}
