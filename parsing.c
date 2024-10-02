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
	/*printf("Camera: position = (%lf, %lf, %lf), orientation = (%lf, %lf, %lf), fov = %lf\n",
		   sc->camera.position.x, sc->camera.position.y, sc->camera.position.z,
		   sc->camera.orientation.x, sc->camera.orientation.y, sc->camera.orientation.z,
		   sc->camera.fov);*/
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
	/*printf("Light: position = (%lf, %lf, %lf), intensity = %lf, color = (%d, %d, %d)\n",
		   point_light.position.x, point_light.position.y, point_light.position.z,
		   point_light.intensity, point_light.red, point_light.green, point_light.blue);*/
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
	/*printf("Sphere: center = (%lf, %lf, %lf), radius = %lf, color = (%d, %d, %d)\n",
		   new_sphere.center.x, new_sphere.center.y, new_sphere.center.z,
		   new_sphere.radius, new_sphere.base.red, new_sphere.base.green, new_sphere.base.blue);*/
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
	/*printf("Plane: point = (%lf, %lf, %lf), normal = (%lf, %lf, %lf), color = (%d, %d, %d)\n",
		new_plane.point.x, new_plane.point.y, new_plane.point.z,
	   new_plane.normal.x, new_plane.normal.y, new_plane.normal.z,
	   new_plane.base.red, new_plane.base.green, new_plane.base.blue);*/
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
	/*printf("Cylinder: center = (%lf, %lf, %lf), axis = (%lf, %lf, %lf), radius = %lf, height = %lf, color = (%d, %d, %d)\n",
		   new_cylinder.center.x, new_cylinder.center.y, new_cylinder.center.z,
		   new_cylinder.axis.x, new_cylinder.axis.y, new_cylinder.axis.z,
		   new_cylinder.radius, new_cylinder.height, new_cylinder.base.red, new_cylinder.base.green, new_cylinder.base.blue);*/
}

void	count_objects(int fd, scene *sc)
{
	char	*line;

	sc->sphere_count = 0;
	sc->cylinder_count = 0;
	sc->plane_count = 0;
	sc->light_count = 0;
	sc->camera_count = 0;

	while ((line = get_next_line(fd)))
	{
		if (line[0] == 'A')
			sc->light_count++;
		else if (line[0] == 'C')
			sc->camera_count++;
		else if (line[0] == 'L')
			sc->light_count++;
		else if (ft_strncmp(line, "sp", 2) == 0)
			sc->sphere_count++;
		else if (ft_strncmp(line, "pl", 2) == 0)
			sc->plane_count++;
		else if (ft_strncmp(line, "cy", 2) == 0)
			sc->cylinder_count++;
		if (sc->light_count > 2 || sc->camera_count > 1) //still need to figure out lights
			sc->success = 1;
		free(line);
	}
}

void	reset_count(scene *sc)
{
	sc->sphere_count = 0;
	sc->cylinder_count = 0;
	sc->plane_count = 0;
	sc->light_count = 0;
}

int	count_and_allocate(scene *sc, int fd)
{
	count_objects(fd, sc);
	sc->spheres = malloc(sizeof(sphere) * sc->sphere_count);
	sc->cylinders = malloc(sizeof(cylinder) * sc->cylinder_count);
	sc->planes = malloc(sizeof(plane) * sc->plane_count);
	sc->lights = malloc(sizeof(light) * sc->light_count);
	if (!sc->spheres || !sc->cylinders || !sc->planes || !sc->lights)
	{
		perror("Error: Memory allocation failed:");
		sc->success = 1;
		return (1);
	}
	close(fd);
	return (0);
}

scene	parse_rt(int fd, char *filename)
{
	scene	sc;
	char	*line;

	sc.success = 0;
	if (count_and_allocate(&sc, fd))
		return (sc);
	reset_count(&sc);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (sc.success = 1, sc);
	while ((line = get_next_line(fd)))
	{
		if (line[0] == 'A')
			parse_ambient(line, &sc);
		else if (line[0] == 'C')
			parse_camera(line, &sc);
		else if (line[0] == 'L')
			parse_light(line, &sc);
		else if (ft_strncmp(line, "sp", 2) == 0)
			parse_sphere(line, &sc);
		else if (ft_strncmp(line, "pl", 2) == 0)
			parse_plane(line, &sc);
		else if (ft_strncmp(line, "cy", 2) == 0)
			parse_cylinder(line, &sc);
		free(line);
	}
	return (sc);
}
