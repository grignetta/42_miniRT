#include "minirt.h"

void	parse_ambient(char *line, scene *sc)
{
	light	ambient_light;
	char	*token;

	ambient_light.type = 0;
	token = ft_strtok(line + 2, " ");
	ambient_light.intensity = get_intensity(token, sc);
	if (ambient_light.intensity == -1)
		return ;
	token = ft_strtok(NULL, ",");
	ambient_light.red = get_color(token, sc);
	if (ambient_light.red == -1)
		return ;
	token = ft_strtok(NULL, ",");
	ambient_light.green = get_color(token, sc);
	if (ambient_light.green == -1)
		return ;
	token = ft_strtok(NULL, " ");
	ambient_light.blue = get_color(token, sc);
	if (ambient_light.blue == -1)
		return ;
	sc->lights[sc->light_count++] = ambient_light;
}

void	parse_light(char *line, scene *sc)
{
	light	point_light;
	char	*token;

	point_light.type = 1;
	token = ft_strtok(line + 2, ",");
	point_light.position.x = get_position(token, sc);
	token = ft_strtok(NULL, ",");
	point_light.position.y = get_position(token, sc);
	token = ft_strtok(NULL, " ");
	point_light.position.z = get_position(token, sc);
	if (sc->success)
		return ;
	token = ft_strtok(NULL, " ");
	point_light.intensity = get_intensity(token, sc);
	if (point_light.intensity == -1)
		return ;
	parse_color_light(token, sc, &point_light);
	sc->lights[sc->light_count++] = point_light;
}

void	count_lights(char *line, scene *sc, int *a_light, int *p_light)
{
	if (line[0] == 'A')
	{
		sc->light_count++;
		(*a_light)++;
	}
	else if (line[0] == 'L')
	{
		sc->light_count++;
		(*p_light)++;
	}
}
