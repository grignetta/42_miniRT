#include "minirt.h"

void	initiate_count(t_scene *sc)
{
	sc->sphere_count = 0;
	sc->cylinder_count = 0;
	sc->plane_count = 0;
	sc->light_count = 0;
	sc->camera_count = 0;
}

void	count_check(char *line, t_scene *sc, int *a_light, int *p_light)
{
	if ((*p_light) > 1 || (*a_light) > 1 || sc->camera_count > 1)
	{
		perror("Error\nToo many ambient lights, point lights or cameras");
		return (sc->success = 1, free(line));
	}
	else if (sc->camera_count == 0)
	{
		perror("Error\nNo camera found");
		return (sc->success = 1, free(line));
	}
}

void add_count(char *trimmed_line, t_scene *sc)
{
	if (*trimmed_line == 'C')
		sc->camera_count++;
	else if (ft_strncmp(trimmed_line, "sp", 2) == 0)
		sc->sphere_count++;
	else if (ft_strncmp(trimmed_line, "pl", 2) == 0)
		sc->plane_count++;
	else if (ft_strncmp(trimmed_line, "cy", 2) == 0)
		sc->cylinder_count++;
}

void	count_objects(int fd, t_scene *sc)
{
	char	*line;
	int		a_light;
	int		p_light;
	char	*trimmed_line;

	a_light = 0;
	p_light = 0;
	initiate_count(sc);
	line = get_next_line(fd);
	while (line)
	{
		trimmed_line = line;
		while (*trimmed_line && ft_isspace(*trimmed_line))
			trimmed_line++;
		count_lights(trimmed_line, sc, &a_light, &p_light);
		add_count(trimmed_line, sc);
		free(line);
		line = get_next_line(fd);
	}
	count_check(line, sc, &a_light, &p_light);
}

void	reset_count(t_scene *sc)
{
	sc->sphere_count = 0;
	sc->cylinder_count = 0;
	sc->plane_count = 0;
	sc->light_count = 0;
}
