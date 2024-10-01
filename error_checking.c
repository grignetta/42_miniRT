#include "minirt.h"

double	get_color(char *token, scene *sc)
{
	double	color;

	if (token && ft_isdigit(*token))
		color = ft_atof(token);
	else
	{
		sc->success = 1;
		perror("Error: Invalid input for color");
		return (-1);
	}
	if (color < 0 || color > 255)
	{
		sc->success = 1;
		perror("Error: The value must be between 0 and 255");
		return (-1);
	}
	return (color);
}

double	get_intensity(char *token, scene *sc)
{
	double	intensity;

	if (token && ft_isdigit(*token))
		intensity = ft_atof(token);
	else
	{
		perror("Error: Invalid input for ambient light intensity");
		sc->success = 1;
		return (-1);
	}
	if (intensity < 0 || intensity > 1)
	{
		perror("Error: Ambient light intensity must be between 0 and 1");
		sc->success = 1;
		return (-1);
	}
	return (intensity);
}
