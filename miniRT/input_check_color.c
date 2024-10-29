#include "minirt.h"

int	parse_color_plane(char *token, t_scene *sc, t_base_shape *base)
{
	token = ft_strtok(NULL, ",");
	base->red = get_color(token, sc);
	if (base->red == -1)
		return (1);
	token = ft_strtok(NULL, ",");
	base->green = get_color(token, sc);
	if (base->green == -1)
		return (1);
	token = ft_strtok(NULL, " ");
	base->blue = get_color(token, sc);
	if (base->blue == -1)
		return (1);
	return (0);
}

int	parse_color_cylinder(char *token, t_scene *sc, t_base_shape *base)
{
	token = ft_strtok(NULL, ",");
	base->red = get_color(token, sc);
	if (base->red == -1)
		return (1);
	token = ft_strtok(NULL, ",");
	base->green = get_color(token, sc);
	if (base->green == -1)
		return (1);
	token = ft_strtok(NULL, " ");
	base->blue = get_color(token, sc);
	if (base->blue == -1)
		return (1);
	return (0);
}

int	parse_color_light(char *token, t_scene *sc, t_light *point_light)
{
	token = ft_strtok(NULL, ",");
	point_light->red = get_color(token, sc);
	if (point_light->red == -1)
		return (1);
	token = ft_strtok(NULL, ",");
	point_light->green = get_color(token, sc);
	if (point_light->green == -1)
		return (1);
	token = ft_strtok(NULL, " ");
	point_light->blue = get_color(token, sc);
	if (point_light->blue == -1)
		return (1);
	return (0);
}

int	parse_color_sphere(char *token, t_scene *sc, t_base_shape *base)
{
	token = ft_strtok(NULL, ",");
	base->red = get_color(token, sc);
	if (base->red == -1)
		return (1);
	token = ft_strtok(NULL, ",");
	base->green = get_color(token, sc);
	if (base->green == -1)
		return (1);
	token = ft_strtok(NULL, " ");
	base->blue = get_color(token, sc);
	if (base->blue == -1)
		return (1);
	return (0);
}

int	get_color(char *token, t_scene *sc)
{
	double	color;

	if (sc->success)
		return (-1);
	if (token && ft_isnum(token))
	{
		color = ft_atoi(token);
		if (color == INT_MAX || color == INT_MIN)
			return (perror("Error\nOut of range"),
				sc->success = 1, -1);
	}
	else
	{
		sc->success = 1;
		perror("Error\nInvalid input for color");
		return (-1);
	}
	if (color < 0 || color > 255)
	{
		sc->success = 1;
		perror("Error\nThe value must be between 0 and 255");
		return (-1);
	}
	return (color);
}
