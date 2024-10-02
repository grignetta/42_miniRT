#include "minirt.h"

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

double	get_position(char *token, scene *sc)
{
	double	position;

	if (token && ft_isnum(token))
		position = ft_atof(token);
	else
	{
		perror("Error: Invalid input for position");
		sc->success = 1;
		return (-1);
	}
	return (position);
}

double	get_fov(char *token, scene *sc)
{
	double	fov;

	if (token && ft_isnum(token))
		fov = ft_atof(token);
	else
	{
		perror("Error: Invalid input for field of view");
		sc->success = 1;
		return (-1);
	}
	if (fov < 0 || fov > 180)
	{
		perror("Error: Field of view must be between 0 and 180 degrees");
		sc->success = 1;
		return (-1);
	}
	return (fov);
}

double	get_value(char *token, scene *sc)
{
	if (token && ft_isnum(token))
		return (ft_atof(token));
	else
	{
		perror("Error: Invalid input");
		sc->success = 1;
		return (-1);
	}
}
