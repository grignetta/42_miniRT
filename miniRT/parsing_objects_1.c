#include "minirt.h"

void	parse_camera(char *line, t_scene *sc)
{
	char	*token;

	token = ft_strtok(line + 2, ",");
	sc->camera.position.x = get_position(token, sc);
	token = ft_strtok(NULL, ",");
	sc->camera.position.y = get_position(token, sc);
	token = ft_strtok(NULL, " ");
	sc->camera.position.z = get_position(token, sc);
	token = ft_strtok(NULL, ",");
	sc->camera.orientation.x = get_position(token, sc);
	token = ft_strtok(NULL, ",");
	sc->camera.orientation.y = get_position(token, sc);
	token = ft_strtok(NULL, " ");
	sc->camera.orientation.z = get_position(token, sc);
	if (sc->success)
		return ;
	token = ft_strtok(NULL, " ");
	sc->camera.fov = get_fov(token, sc);
	if (sc->camera.fov == -1)
		return ;
}

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
	char	*token;

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

int	plane_bonus(char *token, t_scene *sc, t_plane *new_plane, int bonus)
{
	new_plane->base.specular = 0;
	new_plane->base.reflective = 0;
	new_plane->square_size = 0;
	if (bonus)
	{
		token = ft_strtok(NULL, " ");
		new_plane->base.specular = get_value_int(token, sc);
		if (sc->success)
			return (1);
		token = ft_strtok(NULL, " ");
		new_plane->base.reflective = get_value(token, sc);
		if (sc->success)
			return (1);
		token = ft_strtok(NULL, " ");
		new_plane->base.square_size = get_value(token, sc);
		if (sc->success)
			return (1);
	}
	return (0);
}

void	parse_plane(char *line, t_scene *sc, int bonus)
{
	t_plane	new_plane;
	char	*token;

	token = ft_strtok(line + 3, ",");
	new_plane.point.x = get_position(token, sc);
	token = ft_strtok(NULL, ",");
	new_plane.point.y = get_position(token, sc);
	token = ft_strtok(NULL, " ");
	new_plane.point.z = get_position(token, sc);
	token = ft_strtok(NULL, ",");
	if (sc->success)
		return ;
	new_plane.normal.x = get_position(token, sc);
	token = ft_strtok(NULL, ",");
	new_plane.normal.y = get_position(token, sc);
	token = ft_strtok(NULL, " ");
	new_plane.normal.z = get_position(token, sc);
	if (sc->success)
		return ;
	parse_color_plane(token, sc, &new_plane.base);
	if (plane_bonus(token, sc, &new_plane, bonus))
		return ;
	sc->planes[sc->plane_count++] = new_plane;
}
