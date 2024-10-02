#include "minirt.h"

void	parse_camera(char *line, scene *sc)
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

void	parse_sphere(char *line, scene *sc)
{
	sphere	new_sphere;
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
	new_sphere.base.specular = 100;
	new_sphere.base.reflective = 0.5;
	sc->spheres[sc->sphere_count++] = new_sphere;
}

void	parse_plane(char *line, scene *sc)
{
	plane	new_plane;
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
	new_plane.base.specular = 100;
	new_plane.base.reflective = 0.5;
	new_plane.square_size = 0;
	sc->planes[sc->plane_count++] = new_plane;
}

void	parse_cylinder(char *line, scene *sc)
{
	cylinder	new_cylinder;
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
	if (sc->success)
		return ;
	new_cylinder.base.specular = 100;
	new_cylinder.base.reflective = 0.5;
	sc->cylinders[sc->cylinder_count++] = new_cylinder;
}
