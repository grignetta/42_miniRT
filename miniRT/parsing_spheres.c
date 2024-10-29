#include "minirt.h"

int	sphere_bonus(char *token, t_scene *sc, t_sphere *new_sphere, int bonus)
{
	new_sphere->base.specular = 0;
	new_sphere->base.reflective = 0;
	if (bonus)
	{
		token = ft_strtok(NULL, " ");
		new_sphere->base.specular = get_value_int(token, sc);
		if (sc->success)
			return (1);
		token = ft_strtok(NULL, " ");
		new_sphere->base.reflective = get_value(token, sc);
		if (sc->success)
			return (1);
	}
	return (0);
}

void	parse_sphere(char *line, t_scene *sc, int bonus)
{
	t_sphere	new_sphere;
	char		*token;

	token = ft_strtok(line + 3, ",");
	new_sphere.center.x = get_position(token, sc);
	token = ft_strtok(NULL, ",");
	new_sphere.center.y = get_position(token, sc);
	token = ft_strtok(NULL, " ");
	new_sphere.center.z = get_position(token, sc);
	token = ft_strtok(NULL, " ");
	new_sphere.radius = get_value(token, sc) / 2;
	if (sc->success)
		return ;
	parse_color_sphere(token, sc, &new_sphere.base);
	if (sphere_bonus(token, sc, &new_sphere, bonus))
		return ;
	sc->spheres[sc->sphere_count++] = new_sphere;
}
