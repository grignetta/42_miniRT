#include "minirt.h"

void	initiate_count(scene *sc)
{
	sc->sphere_count = 0;
	sc->cylinder_count = 0;
	sc->plane_count = 0;
	sc->light_count = 0;
	sc->camera_count = 0;
}

void	count_check(char *line, scene *sc, int *a_light, int *p_light)
{
	if ((*p_light) > 1 || (*a_light) > 1 || sc->camera_count > 1)
	{
		perror("Error: Too many ambient lights, point lights or cameras");
		return (sc->success = 1, free(line));
	}
}

void	count_objects(int fd, scene *sc)
{
	char	*line;
	int		a_light;
	int		p_light;

	a_light = 0;
	p_light = 0;
	initiate_count(sc);
	line = get_next_line(fd);
	while (line)
	{
		count_lights(line, sc, &a_light, &p_light);
		if (line[0] == 'C')
			sc->camera_count++;
		else if (ft_strncmp(line, "sp", 2) == 0)
			sc->sphere_count++;
		else if (ft_strncmp(line, "pl", 2) == 0)
			sc->plane_count++;
		else if (ft_strncmp(line, "cy", 2) == 0)
			sc->cylinder_count++;
		free(line);
		line = get_next_line(fd);
	}
	count_check(line, sc, &a_light, &p_light);
}

void	reset_count(scene *sc)
{
	sc->sphere_count = 0;
	sc->cylinder_count = 0;
	sc->plane_count = 0;
	sc->light_count = 0;
}
