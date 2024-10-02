#include "minirt.h"

int	main(int argc, char **argv)
{
	t_canvas	*canvas;
	scene		scene;
	int			fd;
	//(void)argv;

	if (argc == 2)
	{
		canvas = init_mlx();
		if (canvas == NULL)
			return (1);
		//scene scene = create_scene(); // change to parcing from .rt file
		fd = open(argv[1], O_RDONLY);
		if (fd == -1)
			return (perror("Error:"), free_close(canvas, fd), 1);
		scene = parse_rt(fd, argv[1]);
		if (scene.success)
			return (free_everything(scene, canvas, fd), 1);
		set_camera(&scene);
		canvas->scene = &scene;
		render(canvas, &scene, &scene.camera);
		mlx_hook(canvas->win_ptr, KeyPress, KeyPressMask, key_handle, canvas);
		mlx_hook(canvas->win_ptr, 17, 0, close_event, canvas);
		mlx_loop(canvas->mlx_ptr);
	}
	else
		return (write(1, "Error\n", 6), 1);
}
