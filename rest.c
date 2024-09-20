#include "minirt.h"

// Function to calculate the intersection of a ray with a sphere
int intersect_ray_sphere(vector O, vector D, sphere sphere, double *t1, double *t2)
{
    vector CO = vector_sub(O, sphere.center);
    double a = vector_dot(D, D);
    double b = 2 * vector_dot(CO, D);
    double c = vector_dot(CO, CO) - sphere.radius * sphere.radius;

    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0) return 0; // No intersection (return inf, inf)?

    *t1 = (-b + sqrt(discriminant)) / (2 * a);
    *t2 = (-b - sqrt(discriminant)) / (2 * a);
    return 1;
}

// Closest intersection function
sphere *closest_intersection(scene *scene, vector O, vector D, double t_min, double t_max, double *closest_t)
{
    *closest_t = t_max;
    sphere *closest_sphere = NULL;

    for (int i = 0; i < scene->sphere_count; i++) {
        double t1, t2;
        if (intersect_ray_sphere(O, D, scene->spheres[i], &t1, &t2)) {
            if (t1 >= t_min && t1 <= t_max && t1 < *closest_t) {
                *closest_t = t1;
                closest_sphere = &scene->spheres[i];
            }
            if (t2 >= t_min && t2 <= t_max && t2 < *closest_t) {
                *closest_t = t2;
                closest_sphere = &scene->spheres[i];
            }
        }
    }
    return closest_sphere;
}

color compute_lighting(scene *scene, vector P, vector N, vector V, int specular)
{
    color result = {0.0, 0.0, 0.0};

    for (int i = 0; i < scene->light_count; i++)
	{
        light light = scene->lights[i];

        if (light.type == 0) // Ambient
        {
            result.red += light.intensity * light.red / 255.0;
            result.green += light.intensity * light.green / 255.0;
            result.blue += light.intensity * light.blue / 255.0;
        }
        else
        {
            vector L;
            double t_max;
            //if (light.type == 1)
			//{ // Point
                L = vector_sub(light.position, P);
                t_max = 1.0;
            //}
			//else
			//{ // Directional
            //    L = light.direction;
            //    t_max = INFINITY;
            //}
            // Check for shadows
            double shadow_t;
            sphere *shadow_sphere = closest_intersection(scene, P, L, 0.001, t_max, &shadow_t);
            if (shadow_sphere != NULL) continue;
            // Diffuse lighting
            double n_dot_l = vector_dot(N, L);
            if (n_dot_l > 0)
            {
                double diffuse_intensity = light.intensity * n_dot_l / (vector_length(N) * vector_length(L));
                result.red += diffuse_intensity * light.red / 255.0;
                result.green += diffuse_intensity * light.green / 255.0;
                result.blue += diffuse_intensity * light.blue / 255.0;
            }
            // Specular reflection
            //if (specular != -1) { //maybe make if specular > 5
            if (specular > 5) {
                vector R = vector_reflect(L, N);
                double r_dot_v = vector_dot(R, V);
                if (r_dot_v > 0)
                {
                    double specular_intensity = light.intensity * pow(r_dot_v / (vector_length(R) * vector_length(V)), specular);
                    result.red += specular_intensity * light.red / 255.0;
                    result.green += specular_intensity * light.green / 255.0;
                    result.blue += specular_intensity * light.blue / 255.0;
                }
            }
        }
    }

    // Ensure the values are within the 0-1 range
    result.red = result.red > 1.0 ? 1.0 : result.red;
    result.green = result.green > 1.0 ? 1.0 : result.green;
    result.blue = result.blue > 1.0 ? 1.0 : result.blue;

    return result;
}

// Function to trace a ray
/* int trace_ray(scene *scene, vector O, vector D, double t_min, double t_max)//, int depth)
{
    double closest_t;
    sphere *closest_sphere = closest_intersection(scene, O, D, t_min, t_max, &closest_t);
    if (closest_sphere == NULL)
        return BACKGROUND_COLOR; // Background color
    // Compute local color at the intersection point
    vector P = vector_add(O, vector_scale(D, closest_t)); // Intersection point
    vector N = vector_normalize(vector_sub(P, closest_sphere->center)); // Normal at the intersection
    vector V = vector_scale(D, -1); // View direction - not found in pseudocode !!!!!!!!!!!!!!
    //double lighting = compute_lighting(scene, P, N, V, closest_sphere->specular);
    color lighting = compute_lighting(scene, P, N, V, closest_sphere->specular);

    // int r = closest_sphere->red * lighting;
    // int g = closest_sphere->green * lighting;
    // int b = closest_sphere->blue * lighting;
    int r = (int)closest_sphere->red * lighting.red;
    int g = (int)closest_sphere->green * lighting.green;
    int b = (int)closest_sphere->blue * lighting.blue;

    // Ensure the values are within the 0-255 range
    r = (r > 255) ? 255 : r;
    g = (g > 255) ? 255 : g;
    b = (b > 255) ? 255 : b;

    return ((r << 16) | (g << 8) | b);
} */

int trace_ray(scene *scene, vector O, vector D, double t_min, double t_max, int depth)
{
    double closest_t;
    sphere *closest_sphere = closest_intersection(scene, O, D, t_min, t_max, &closest_t);
    if (closest_sphere == NULL)
        return BACKGROUND_COLOR; // Background color

    // Compute local color at the intersection point
    vector P = vector_add(O, vector_scale(D, closest_t)); // Intersection point
    vector N = vector_normalize(vector_sub(P, closest_sphere->center)); // Normal at the intersection
    vector V = vector_scale(D, -1); // View direction

    color local_lighting = compute_lighting(scene, P, N, V, closest_sphere->specular);

    color local_color;
    local_color.red = (closest_sphere->red / 255.0) * local_lighting.red;
    local_color.green = (closest_sphere->green / 255.0) * local_lighting.green;
    local_color.blue = (closest_sphere->blue / 255.0) * local_lighting.blue;

    double r = closest_sphere->reflective;
    if (depth <= 0 || r <= 0) {
        int final_red = (int)(local_color.red * 255);
        int final_green = (int)(local_color.green * 255);
        int final_blue = (int)(local_color.blue * 255);

        final_red = (final_red > 255) ? 255 : final_red;
        final_green = (final_green > 255) ? 255 : final_green;
        final_blue = (final_blue > 255) ? 255 : final_blue;

        return (final_red << 16) | (final_green << 8) | final_blue;
    }

    // Compute reflection vector
    vector R = vector_reflect(V, N);

    // Trace the reflected ray
    int reflected_color_int = trace_ray(scene, P, R, 0.001, INFINITY, depth - 1);

    color reflected_color = {(reflected_color_int >> 16 & 0xFF) / 255.0, (reflected_color_int >> 8 & 0xFF) / 255.0, (reflected_color_int & 0xFF) / 255.0};
    // Combine local color and reflected color based on reflectivity

    color final_color;

    final_color.red = local_color.red * (1 - r) + reflected_color.red * r;
    final_color.green = local_color.green * (1 - r) + reflected_color.green * r;
    final_color.blue = local_color.blue * (1 - r) + reflected_color.blue * r;

    int final_red = (int)(final_color.red * 255);
    int final_green = (int)(final_color.green * 255);
    int final_blue = (int)(final_color.blue * 255);

    final_red = (final_red > 255) ? 255 : final_red;
    final_green = (final_green > 255) ? 255 : final_green;
    final_blue = (final_blue > 255) ? 255 : final_blue;

    return (final_red << 16) | (final_green << 8) | final_blue;
}

// Function to draw pixels on the canvas
void put_pixel(t_canvas *canvas, int x, int y, int color)
{
    char *dst = canvas->addr + (y * canvas->line_length + x * (canvas->bits_per_pixel / 8));
    *(unsigned int *)dst = color;
}

// Main rendering loop
void render(t_canvas *canvas, scene *scene)
{
    camera *camera = &scene->camera;
    //double viewport_size = 1.0;
    double viewport_size = 2 * tan(camera->fov * M_PI / 360.0);
    double projection_plane_d = viewport_size;//1.0; should be same as vp size, otherwise a fisheye effect
    //vector camera_position = {0, 0, -5}; // change to camera stuct
    vector camera_position = scene->camera.position;

    for (int x = -canvas->win_width / 2; x < canvas->win_width / 2; x++) {
        for (int y = -canvas->win_height / 2; y < canvas->win_height / 2; y++) {
            vector D = {x * viewport_size / canvas->win_width, y * viewport_size / canvas->win_height, projection_plane_d};
            D = vector_normalize(D); // Normalize the ray direction
            // Adjust ray direction based on camera's orientation
            D = vector_add(D, camera->orientation);  // Align with camera's orientation
            int color = trace_ray(scene, camera_position, D, 1.0, INFINITY, 3);
            put_pixel(canvas, x + canvas->win_width / 2, canvas->win_height / 2 - y, color);
        }
    }
    mlx_put_image_to_window(canvas->mlx_ptr, canvas->win_ptr, canvas->img, 0, 0);
}

// Example scene setup
scene create_scene() {
    scene scene;
    scene.sphere_count = 4;
    scene.spheres = malloc(sizeof(sphere) * scene.sphere_count);

    // Red sphere
    scene.spheres[0] = (sphere){{0, -1, 3}, 1, 255, 0, 0, 500, 0.2};
    // Blue sphere
    scene.spheres[1] = (sphere){{-2, 0, 4}, 1, 0, 0, 255, 10, 0.3};
    // Green sphere
    scene.spheres[2] = (sphere){{2, 0, 4}, 1, 0, 255, 0, 10, 0.4};
    // Yellow large sphere (floor)
    scene.spheres[3] = (sphere){{0, -5001, 0}, 5000, 255, 255, 0, 1000, 0.5};

    scene.light_count = 3;
    scene.lights = malloc(sizeof(light) * scene.light_count);

    // Ambient
	scene.lights[0] = (light){0, 0.2, {0, 0, 0}, {0, 0, 0}, 255, 255, 255};
    // Point light
    scene.lights[1] = (light){1, 0.6, {2, 1, 0}, {0, 0, 0}, 255, 255, 255};//point light also can have its own color (for bonus)
    // Directional light
   // scene.lights[2] = (light){2, 0.2, {0, 0, 0}, {1, 4, 4}, 245, 144, 144};//if ambient and point lights have different colors, what to write here?

     // Initialize camera (you will replace these values after reading the .rt file)
    scene.camera.position = (vector){0, 0, -5}; // Example values
    scene.camera.orientation = (vector){0.0, 0.0, 0.0}; // Example values
    scene.camera.fov = 100.0; // Example value

    return scene;
}