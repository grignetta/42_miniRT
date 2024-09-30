#include "minirt.h"

void parse_ambient(char *line, scene *sc)
{
	light	ambient_light;
	char	*token;

	ambient_light.type = 0;
	token = ft_strtok(line + 2, " ");
	ambient_light.intensity = ft_atof(token);
	token = ft_strtok(NULL, ",");
	ambient_light.red = ft_atoi(token);
	token = ft_strtok(NULL, ",");
	ambient_light.green = ft_atoi(token);
	token = ft_strtok(NULL, " ");
	ambient_light.blue = ft_atoi(token);
	sc->lights[sc->light_count++] = ambient_light;
	printf("Ambient: intensity = %lf, color = (%d, %d, %d)\n",
		   ambient_light.intensity, ambient_light.red, ambient_light.green, ambient_light.blue);
}

// Parse camera
void parse_camera(char *line, scene *sc)
{
	char	*token;

	token = ft_strtok(line + 2, ",");
	sc->camera.position.x = ft_atof(token);
	token = ft_strtok(NULL, ",");
	sc->camera.position.y = ft_atof(token);
	token = ft_strtok(NULL, " ");
	sc->camera.position.z = ft_atof(token);
	token = ft_strtok(NULL, ",");
	sc->camera.orientation.x = ft_atof(token);
	token = ft_strtok(NULL, ",");
	sc->camera.orientation.y = ft_atof(token);
	token = ft_strtok(NULL, " ");
	sc->camera.orientation.z = ft_atof(token);
	token = ft_strtok(NULL, " ");
	sc->camera.fov = ft_atof(token);

	printf("Camera: position = (%lf, %lf, %lf), orientation = (%lf, %lf, %lf), fov = %lf\n",
		   sc->camera.position.x, sc->camera.position.y, sc->camera.position.z,
		   sc->camera.orientation.x, sc->camera.orientation.y, sc->camera.orientation.z,
		   sc->camera.fov);
}

// Parse light
void parse_light(char *line, scene *sc)
{
	light	point_light;

	point_light.type = 1;  // 1 for point light
	char *token = ft_strtok(line + 2, ",");
	point_light.position.x = ft_atof(token);
	token = ft_strtok(NULL, ",");
	point_light.position.y = ft_atof(token);
	token = ft_strtok(NULL, " ");
	point_light.position.z = ft_atof(token);
	token = ft_strtok(NULL, " ");
	point_light.intensity = ft_atof(token);
	token = ft_strtok(NULL, ",");
	point_light.red = ft_atoi(token);
	token = ft_strtok(NULL, ",");
	point_light.green = ft_atoi(token);
	token = ft_strtok(NULL, " ");
	point_light.blue = ft_atoi(token);
	sc->lights[sc->light_count++] = point_light;
	printf("Light: position = (%lf, %lf, %lf), intensity = %lf, color = (%d, %d, %d)\n",
		   point_light.position.x, point_light.position.y, point_light.position.z,
		   point_light.intensity, point_light.red, point_light.green, point_light.blue);
}

void parse_sphere(char *line, scene *sc)
{
	sphere	new_sphere;

	char *token = ft_strtok(line + 3, ",");
	new_sphere.center.x = ft_atof(token);
	token = ft_strtok(NULL, ",");
	new_sphere.center.y = ft_atof(token);
	token = ft_strtok(NULL, " ");
	new_sphere.center.z = ft_atof(token);
	token = ft_strtok(NULL, " ");
	new_sphere.radius = ft_atof(token) / 2.0;  // Assuming the input is diameter, convert to radius
	token = ft_strtok(NULL, ",");
	new_sphere.base.red = ft_atoi(token);
	token = ft_strtok(NULL, ",");
	new_sphere.base.green = ft_atoi(token);
	token = ft_strtok(NULL, " ");
	new_sphere.base.blue = ft_atoi(token);
	new_sphere.base.specular = 100; // Default specular value
	new_sphere.base.reflective = 0.5;  // Default reflective value
	sc->spheres[sc->sphere_count++] = new_sphere;
	printf("Sphere: center = (%lf, %lf, %lf), radius = %lf, color = (%d, %d, %d)\n",
		   new_sphere.center.x, new_sphere.center.y, new_sphere.center.z,
		   new_sphere.radius, new_sphere.base.red, new_sphere.base.green, new_sphere.base.blue);
}

void parse_plane(char *line, scene *sc)
{
	plane	new_plane;
	char	*token;

	token = ft_strtok(line + 3, ",");
	new_plane.point.x = ft_atof(token);
	token = ft_strtok(NULL, ",");
	new_plane.point.y = ft_atof(token);
	token = ft_strtok(NULL, " ");
	new_plane.point.z = ft_atof(token);
	token = ft_strtok(NULL, ",");
	new_plane.normal.x = ft_atof(token);
	token = ft_strtok(NULL, ",");
	new_plane.normal.y = ft_atof(token);
	token = ft_strtok(NULL, " ");
	new_plane.normal.z = ft_atof(token);
	token = ft_strtok(NULL, ",");
	new_plane.base.red = ft_atoi(token);
	token = ft_strtok(NULL, ",");
	new_plane.base.green = ft_atoi(token);
	token = ft_strtok(NULL, " ");
	new_plane.base.blue = ft_atoi(token);
	new_plane.base.specular = 100;
	new_plane.base.reflective = 0.5;
	new_plane.square_size = 0;
	sc->planes[sc->plane_count++] = new_plane;
	printf("Plane: point = (%lf, %lf, %lf), normal = (%lf, %lf, %lf), color = (%d, %d, %d)\n",
		   new_plane.point.x, new_plane.point.y, new_plane.point.z,
		   new_plane.normal.x, new_plane.normal.y, new_plane.normal.z,
		   new_plane.base.red, new_plane.base.green, new_plane.base.blue);
}

void parse_cylinder(char *line, scene *sc)
{
	cylinder	new_cylinder;
	char		*token;

	token = ft_strtok(line + 3, ",");
	new_cylinder.center.x = ft_atof(token);
	token = ft_strtok(NULL, ",");
	new_cylinder.center.y = ft_atof(token);
	token = ft_strtok(NULL, " ");
	new_cylinder.center.z = ft_atof(token);
	token = ft_strtok(NULL, ",");
	new_cylinder.axis.x = ft_atof(token);
	token = ft_strtok(NULL, ",");
	new_cylinder.axis.y = ft_atof(token);
	token = ft_strtok(NULL, " ");
	new_cylinder.axis.z = ft_atof(token);
	token = ft_strtok(NULL, " ");
	new_cylinder.radius = ft_atof(token) / 2.0;
	token = ft_strtok(NULL, " ");
	new_cylinder.height = ft_atof(token);
	token = ft_strtok(NULL, ",");
	new_cylinder.base.red = ft_atoi(token);
	token = ft_strtok(NULL, ",");
	new_cylinder.base.green = ft_atoi(token);
	token = ft_strtok(NULL, " ");
	new_cylinder.base.blue = ft_atoi(token);
	new_cylinder.base.specular = 100;  // Default specular value
	new_cylinder.base.reflective = 0.5;  // Default reflective value
	sc->cylinders[sc->cylinder_count++] = new_cylinder;

	printf("Cylinder: center = (%lf, %lf, %lf), axis = (%lf, %lf, %lf), radius = %lf, height = %lf, color = (%d, %d, %d)\n",
		   new_cylinder.center.x, new_cylinder.center.y, new_cylinder.center.z,
		   new_cylinder.axis.x, new_cylinder.axis.y, new_cylinder.axis.z,
		   new_cylinder.radius, new_cylinder.height, new_cylinder.base.red, new_cylinder.base.green, new_cylinder.base.blue);
}

void count_objects(int fd, scene *sc)
{
    char	*line;

    sc->sphere_count = 0;
    sc->cylinder_count = 0;
    sc->plane_count = 0;
    sc->light_count = 0;

    while ((line = get_next_line(fd)))
	{
        if (line[0] == 'A')
            sc->light_count++;
        else if (line[0] == 'C')  //necessary or can I just skip?
            ;
        else if (line[0] == 'L')
            sc->light_count++;
        else if (ft_strncmp(line, "sp", 2) == 0)
            sc->sphere_count++;
        else if (ft_strncmp(line, "pl", 2) == 0)
            sc->plane_count++;
        else if (ft_strncmp(line, "cy", 2) == 0)
            sc->cylinder_count++;
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

// Parse the .rt file
scene	parse_rt(int fd, char *filename)
{
	scene	sc;
	char	*line;

	sc.success = 0;
	count_objects(fd, &sc);
	sc.spheres = malloc(sizeof(sphere) * sc.sphere_count);
	sc.cylinders = malloc(sizeof(cylinder) * sc.cylinder_count);
	sc.planes = malloc(sizeof(plane) * sc.plane_count);
	sc.lights = malloc(sizeof(light) * sc.light_count);
	//sc.planes = NULL;
	if (!sc.spheres || !sc.cylinders || !sc.planes || !sc.lights)
	{
		perror("Error: Memory allocation failed:");
		free_scene(&sc);
		sc.success = 1;
		return (sc);
	}
	close(fd);
	reset_count(&sc);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror("Error\n");
		free_scene(&sc);
		sc.success = 1;
		return (sc);
	}
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
