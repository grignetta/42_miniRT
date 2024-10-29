#include "minirt_bonus.h"


int	cylinder_bonus(char *token, t_scene *sc, t_cylinder *new_cylinder, int bonus)
{
	new_cylinder->base.specular = 0;
	new_cylinder->base.reflective = 0;
	if (bonus)
	{
		token = ft_strtok(NULL, " ");
		new_cylinder->base.specular = get_value_int(token, sc);
		if (sc->success)
			return (1);
		token = ft_strtok(NULL, " ");
		new_cylinder->base.reflective = get_value(token, sc);
		if (sc->success)
			return (1);
	}
	return (0);
}

void	parse_cylinder(char *line, t_scene *sc, int bonus)
{
	t_cylinder	new_cylinder;
	char		*token;

	token = ft_strtok(line + 3, ",");
	new_cylinder.center.x = get_position(token, sc);
	token = ft_strtok(NULL, ",");
	new_cylinder.center.y = get_position(token, sc);
	token = ft_strtok(NULL, " ");
	new_cylinder.center.z = get_position(token, sc);
	token = ft_strtok(NULL, ",");
	new_cylinder.axis.x = get_position(token, sc);
	token = ft_strtok(NULL, ",");
	new_cylinder.axis.y = get_position(token, sc);
	token = ft_strtok(NULL, " ");
	new_cylinder.axis.z = get_position(token, sc);
	token = ft_strtok(NULL, " ");
	new_cylinder.radius = get_value(token, sc) / 2;
	token = ft_strtok(NULL, " ");
	new_cylinder.height = get_value(token, sc);
	parse_color_cylinder(token, sc, &new_cylinder.base);
	if (cylinder_bonus(token, sc, &new_cylinder, bonus))
		return ;
	sc->cylinders[sc->cylinder_count++] = new_cylinder;
}
