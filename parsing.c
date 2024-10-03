#include "minirt.h"

void	count_and_allocate(t_scene *sc, int fd)
{
	count_objects(fd, sc);
	sc->spheres = malloc(sizeof(t_sphere) * sc->sphere_count);
	sc->cylinders = malloc(sizeof(t_cylinder) * sc->cylinder_count);
	sc->planes = malloc(sizeof(t_plane) * sc->plane_count);
	sc->lights = malloc(sizeof(t_light) * sc->light_count);
	if (!sc->spheres || !sc->cylinders || !sc->planes || !sc->lights)
	{
		perror("Error\nMemory allocation failed:");
		sc->success = 1;
		close(fd);
		return ;
	}
	close(fd);
	return ;
}

void	parse_line(char *line, t_scene *sc)
{
	if (line[0] == 'A')
		parse_ambient(line, sc);
	else if (line[0] == 'C')
		parse_camera(line, sc);
	else if (line[0] == 'L')
		parse_light(line, sc);
	else if (ft_strncmp(line, "sp", 2) == 0)
		parse_sphere(line, sc);
	else if (ft_strncmp(line, "pl", 2) == 0)
		parse_plane(line, sc);
	else if (ft_strncmp(line, "cy", 2) == 0)
		parse_cylinder(line, sc);
	free(line);
}

t_scene	parse_rt(int fd, char *filename)
{
	t_scene	sc;
	char	*line;

	sc.success = 0;
	count_and_allocate(&sc, fd);
	if (sc.success)
		return (sc);
	reset_count(&sc);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (sc.success = 1, sc);
	line = get_next_line(fd);
	while (line)
	{
		parse_line(line, &sc);
		line = get_next_line(fd);
	}
	return (sc);
}
