
void parse_ambient(char *line) {
    Ambient ambient;
    char *token = strtok(line + 2, " ");
    ambient.ratio = atof(token);

    token = strtok(NULL, ",");
    ambient.r = atoi(token);
    token = strtok(NULL, ",");
    ambient.g = atoi(token);
    token = strtok(NULL, " ");
    ambient.b = atoi(token);

    printf("Ambient: ratio = %lf, color = (%d, %d, %d)\n", ambient.ratio, ambient.r, ambient.g, ambient.b);
}

void parse_camera(char *line) {
    Camera camera;
    char *token = strtok(line + 2, ",");
    camera.x = atof(token);
    token = strtok(NULL, ",");
    camera.y = atof(token);
    token = strtok(NULL, " ");
    camera.z = atof(token);

    token = strtok(NULL, ",");
    camera.orient_x = atof(token);
    token = strtok(NULL, ",");
    camera.orient_y = atof(token);
    token = strtok(NULL, " ");
    camera.orient_z = atof(token);

    token = strtok(NULL, " ");
    camera.fov = atof(token);

    printf("Camera: position = (%lf, %lf, %lf), orientation = (%lf, %lf, %lf), fov = %lf\n",
           camera.x, camera.y, camera.z, camera.orient_x, camera.orient_y, camera.orient_z, camera.fov);
}

void parse_light(char *line) {
    Light light;
    char *token = strtok(line + 2, ",");
    light.x = atof(token);
    token = strtok(NULL, ",");
    light.y = atof(token);
    token = strtok(NULL, " ");
    light.z = atof(token);

    token = strtok(NULL, " ");
    light.brightness = atof(token);

    token = strtok(NULL, ",");
    light.r = atoi(token);
    token = strtok(NULL, ",");
    light.g = atoi(token);
    token = strtok(NULL, " ");
    light.b = atoi(token);

    printf("Light: position = (%lf, %lf, %lf), brightness = %lf, color = (%d, %d, %d)\n",
           light.x, light.y, light.z, light.brightness, light.r, light.g, light.b);
}

void parse_sphere(char *line) {
    Sphere sphere;
    char *token = strtok(line + 3, ",");
    sphere.x = atof(token);
    token = strtok(NULL, ",");
    sphere.y = atof(token);
    token = strtok(NULL, " ");
    sphere.z = atof(token);

    token = strtok(NULL, " ");
    sphere.diameter = atof(token);

    token = strtok(NULL, ",");
    sphere.r = atoi(token);
    token = strtok(NULL, ",");
    sphere.g = atoi(token);
    token = strtok(NULL, " ");
    sphere.b = atoi(token);

    printf("Sphere: center = (%lf, %lf, %lf), diameter = %lf, color = (%d, %d, %d)\n",
           sphere.x, sphere.y, sphere.z, sphere.diameter, sphere.r, sphere.g, sphere.b);
}

void parse_plane(char *line) {
    Plane plane;
    char *token = strtok(line + 3, ",");
    plane.x = atof(token);
    token = strtok(NULL, ",");
    plane.y = atof(token);
    token = strtok(NULL, " ");
    plane.z = atof(token);

    token = strtok(NULL, ",");
    plane.norm_x = atof(token);
    token = strtok(NULL, ",");
    plane.norm_y = atof(token);
    token = strtok(NULL, " ");
    plane.norm_z = atof(token);

    token = strtok(NULL, ",");
    plane.r = atoi(token);
    token = strtok(NULL, ",");
    plane.g = atoi(token);
    token = strtok(NULL, " ");
    plane.b = atoi(token);

    printf("Plane: point = (%lf, %lf, %lf), normal = (%lf, %lf, %lf), color = (%d, %d, %d)\n",
           plane.x, plane.y, plane.z, plane.norm_x, plane.norm_y, plane.norm_z, plane.r, plane.g, plane.b);
}

void parse_cylinder(char *line) {
    Cylinder cylinder;
    char *token = strtok(line + 3, ",");
    cylinder.x = atof(token);
    token = strtok(NULL, ",");
    cylinder.y = atof(token);
    token = strtok(NULL, " ");
    cylinder.z = atof(token);

    token = strtok(NULL, ",");
    cylinder.orient_x = atof(token);
    token = strtok(NULL, ",");
    cylinder.orient_y = atof(token);
    token = strtok(NULL, " ");
    cylinder.orient_z = atof(token);

    token = strtok(NULL, " ");
    cylinder.diameter = atof(token);

    token = strtok(NULL, " ");
    cylinder.height = atof(token);

    token = strtok(NULL, ",");
    cylinder.r = atoi(token);
    token = strtok(NULL, ",");
    cylinder.g = atoi(token);
    token = strtok(NULL, " ");
    cylinder.b = atoi(token);

    printf("Cylinder: center = (%lf, %lf, %lf), orientation = (%lf, %lf, %lf), diameter = %lf, height = %lf, color = (%d, %d, %d)\n",
           cylinder.x, cylinder.y, cylinder.z, cylinder.orient_x, cylinder.orient_y, cylinder.orient_z,
           cylinder.diameter, cylinder.height, cylinder.r, cylinder.g, cylinder.b);
}


scene parse_rt(int fd)
{
    char *line;
    while ((line = get_next_line(fd))) {
        if (line[0] == 'A') {
            parse_ambient(line);
        } else if (line[0] == 'C') {
            parse_camera(line);
        } else if (line[0] == 'L') {
            parse_light(line);
        } else if (strncmp(line, "sp", 2) == 0) {
            parse_sphere(line);
        } else if (strncmp(line, "pl", 2) == 0) {
            parse_plane(line);
        } else if (strncmp(line, "cy", 2) == 0) {
            parse_cylinder(line);
        }
        free(line);
    }
}

