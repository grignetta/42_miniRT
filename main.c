#include "minirt.h"

void	init_canvas(t_canvas *canvas)
{
	canvas->win_width = 1000;
	canvas->win_height = 1000;
}

t_canvas	*initialize_matrix()//char *filename)
{
	t_canvas	*canvas;

	canvas = (t_canvas *)malloc(sizeof(t_canvas));//do we need it on heap?
	if (!canvas)
		return (NULL);
	//if (read_file(filename, canvas)) //read geometry
	//{
	//	write(1, "error\n", 6);
	//	return (free(canvas), NULL);
	//}
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

int	main(int argc, char **argv)
{
	t_canvas	*canvas;
	scene		scene;
	//int			fd;

	(void)argv;
	if (argc == 2)
	{
		canvas = initialize_matrix();
		if (canvas == NULL)
			return (1);
		if (initialize_graphics(canvas))
			return (free(canvas), 1);
		if (initialize_image(canvas))
			return (mlx_destroy_window(canvas->mlx_ptr, canvas->win_ptr),
				mlx_destroy_display(canvas->mlx_ptr), free(canvas->mlx_ptr),
				free(canvas), 1);
		scene = create_scene(); // Initialize your
		/*fd = open(argv[1], O_RDONLY);
		if (fd == -1)
			return (perror("Error\n"), 1);
		scene = parse_rt(fd, argv[1]);
		if (scene.success)
		{
			close(fd);
        	mlx_destroy_window(canvas->mlx_ptr, canvas->win_ptr);
        	mlx_destroy_display(canvas->mlx_ptr);
        	free(canvas->mlx_ptr);
        	free(canvas);
        	return (1);
    	}
		close(fd);*/
        render(canvas, &scene, &scene.camera); // Render the scene
		/*mlx_mouse_hook(canvas->win_ptr, mouse_event, canvas);
		mlx_hook(canvas->win_ptr, 2, 1, handle_input, canvas);*/
		mlx_hook(canvas->win_ptr, 17, 0, close_event, canvas);
		//mlx_loop_hook(canvas->mlx_ptr, update, canvas);
		mlx_loop(canvas->mlx_ptr);
	}
	else
		return (write(1, "error\n", 6), 1);
}