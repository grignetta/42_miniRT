#include "minirt.h"

void	count_and_allocate(t_scene *sc, int fd, int bonus)
{
	count_objects(fd, sc, bonus);
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

void	parse_line(char *line, t_scene *sc, int bonus)
{
	char	*trimmed_line;

	trimmed_line = line;
	while (*trimmed_line && ft_isspace(*trimmed_line))
		trimmed_line++;
	if (*trimmed_line == 'A')
		parse_ambient(trimmed_line, sc);
	else if (*trimmed_line == 'C')
		parse_camera(trimmed_line, sc);
	else if (*trimmed_line == 'L')
		parse_light(trimmed_line, sc);
	else if (ft_strncmp(trimmed_line, "sp", 2) == 0)
		parse_sphere(trimmed_line, sc, bonus);
	else if (ft_strncmp(trimmed_line, "pl", 2) == 0)
		parse_plane(trimmed_line, sc, bonus);
	else if (ft_strncmp(trimmed_line, "cy", 2) == 0)
		parse_cylinder(trimmed_line, sc, bonus);
	if (bonus && *trimmed_line == 'D')
		parse_directional(trimmed_line, sc);
	free(line);
}

t_scene	parse_rt(int fd, char *filename, int bonus)
{
	t_scene	sc;
	char	*line;

	sc.success = 0;
	count_and_allocate(&sc, fd, bonus);
	if (sc.success)
		return (sc);
	reset_count(&sc);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (sc.success = 1, sc);
	line = get_next_line(fd);
	while (line)
	{
		parse_line(line, &sc, bonus);
		line = get_next_line(fd);
	}
	close(fd);
	fd = -1;
	return (sc);
}
