#include "minirt.h"

double	get_intensity(char *token, t_scene *sc)
{
	double	intensity;

	if (token && ft_isnum(token))
		intensity = ft_atof(token);
	else
	{
		perror("Error\nInvalid input for ambient light intensity");
		sc->success = 1;
		return (-1);
	}
	if (intensity < 0 || intensity > 1)
	{
		perror("Error\nAmbient light intensity must be between 0 and 1");
		sc->success = 1;
		return (-1);
	}
	return (intensity);
}

double	get_position(char *token, t_scene *sc)
{
	double	position;

	if (token && ft_isnum(token))
	{
		position = ft_atof(token);
		if (position == DBL_MAX || position == DBL_MIN)
		{
			perror("Error\nPosition is out of range");
			sc->success = 1;
			return (-1);
		}
	}
	else
	{
		perror("Error\nInvalid input for position");
		sc->success = 1;
		return (-1);
	}
	return (position);
}

double	get_fov(char *token, t_scene *sc)
{
	double	fov;

	if (token && ft_isnum(token))
		fov = ft_atof(token);
	else
	{
		perror("Error\nInvalid input for field of view");
		sc->success = 1;
		return (-1);
	}
	if (fov < 0 || fov > 180)
	{
		perror("Error\nField of view must be between 0 and 180 degrees");
		sc->success = 1;
		return (-1);
	}
	return (fov);
}

double	get_value(char *token, t_scene *sc)
{
	if (token && ft_isnum(token))
		return (ft_atof(token));
	else
	{
		perror("Error\nInvalid input");
		sc->success = 1;
		return (-1);
	}
}
