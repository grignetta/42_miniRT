#include "minirt_bonus.h"

int	get_fd(char *filename, t_canvas *canvas)
{
	int	fd;

	if (ft_strcmp(filename + ft_strlen(filename) - 3, ".rt"))
	{
		free_close(canvas, -1);
		printf("Error\nInvalid file extension\n");
		return (-1);
	}
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (perror("Error\n"), free_close(canvas, fd), -1);
	return (fd);
}

int	is_bonus(char **argv)
{
	if (ft_strcmp(argv[0], "./miniRT_bonus") == 0)
		return (1);
	else
		return (0);
}

void	mlx_helper(t_canvas *canvas)
{
	mlx_hook(canvas->win_ptr, KeyPress, KeyPressMask, key_handle, canvas);
	mlx_hook(canvas->win_ptr, 17, 0, close_event, canvas);
	mlx_loop(canvas->mlx_ptr);
}

int	main(int argc, char **argv)
{
	t_canvas	*canvas;
	t_scene		scene;
	int			fd;
	int			bonus;

	if (argc == 2)
	{
		canvas = init_mlx();
		if (canvas == NULL)
			return (1);
		bonus = is_bonus(argv);
		fd = get_fd(argv[1], canvas);
		if (fd == -1)
			return (1);
		scene = parse_rt(fd, argv[1], bonus);
		if (scene.success)
			return (free_everything(scene, canvas, fd), 1);
		set_camera(&scene);
		canvas->scene = &scene;
		render(canvas, &scene, &scene.camera);
		mlx_helper(canvas);
	}
	else
		return (write(1, "Error\n", 6), 1);
}
