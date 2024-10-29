#include "minirt.h"

double	get_intensity(char *token, t_scene *sc)
{
	double	intensity;

	if (sc->success)
		return (-1);
	if (token && ft_isnum(token))
	{
		intensity = ft_atof(token);
		if (intensity == DBL_MAX || intensity == -DBL_MAX)
			return (perror("Error\nOut of range"),
					sc->success = 1, -1);
	}
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

	if (sc->success)
		return (-1);
	if (token && ft_isnum(token))
	{
		position = ft_atof(token);
		if (position == DBL_MAX || position == -DBL_MAX)
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

	if (sc->success)
		return (-1);
	if (token && ft_isnum(token))
	{
		fov = ft_atof(token);
		if (fov == DBL_MAX || fov == -DBL_MAX)
			return (perror("Error\nOut of range"),
					sc->success = 1, -1);
	}
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
	double value;

	if (sc->success)
		return (-1);
	if (token && ft_isnum(token))
	{
		value = ft_atof(token);
		if (value == DBL_MAX || value == -DBL_MAX || value < 0)
		{
			perror("Error\nValue is out of range");
			sc->success = 1;
			return (-1);
		}
		return (value);
	}
	else
	{
		perror("Error\nInvalid input");
		sc->success = 1;
		return (-1);
	}
}

int	get_value_int(char *token, t_scene *sc)
{
	int value;

	if (sc->success)
		return (-1);
	if (token && ft_isnum(token))
	{
		value = ft_atoi(token);
		if (value == INT_MAX || value == INT_MIN || value < 0)
		{
			perror("Error\nValue is out of range");
			sc->success = 1;
			return (-1);
		}
		return (value);
	}
	else
	{
		perror("Error\nInvalid input");
		sc->success = 1;
		return (-1);
	}
}
