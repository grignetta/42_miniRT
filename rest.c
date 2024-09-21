#include "minirt.h"

// Function to calculate the intersection of a ray with a sphere
int intersect_ray_sphere(vector O, vector D, sphere *sphere, double *t1, double *t2)
{
    vector CO = vector_sub(O, sphere->center);
    double a = vector_dot(D, D);
    double b = 2 * vector_dot(CO, D);
    double c = vector_dot(CO, CO) - sphere->radius * sphere->radius;

    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0) return 0; // No intersection (return inf, inf)?

    *t1 = (-b + sqrt(discriminant)) / (2 * a);
    *t2 = (-b - sqrt(discriminant)) / (2 * a);
    return 1;
}

int intersect_ray_cylinder(vector O, vector D, cylinder *cyl, double *t1, double *t2)
{
    // Translate the ray and cylinder to a common origin, typically the cylinder's center
    vector CO = vector_sub(O, cyl->center);

    // Coefficients for the quadratic equation
    double a = D.x * D.x + D.z * D.z;
    double b = 2 * (CO.x * D.x + CO.z * D.z);
    double c = CO.x * CO.x + CO.z * CO.z - cyl->radius * cyl->radius;

    // Discriminant
    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return 0; // No intersection

    *t1 = (-b + sqrt(discriminant)) / (2 * a);
    *t2 = (-b - sqrt(discriminant)) / (2 * a);

    // Check if the intersection points are within the cylinder's height
    double y1 = O.y + *t1 * D.y;
    double y2 = O.y + *t2 * D.y;
    if (y1 < cyl->center.y || y1 > cyl->center.y + cyl->height)
        *t1 = INFINITY;
    if (y2 < cyl->center.y || y2 > cyl->center.y + cyl->height)
        *t2 = INFINITY;

    return 1;
}

int intersect_ray_plane(vector O, vector D, plane *pl, double *t)
{
    double denom = vector_dot(pl->normal, D);
    if (fabs(denom) > 1e-6) { // Avoid division by zero and small values that might cause numerical instability
        vector P0L0 = vector_sub(pl->point, O);
        *t = vector_dot(P0L0, pl->normal) / denom;
        return (*t >= 0);
    }
    return 0; // No intersection
}

// Closest intersection function
intersection_result closest_intersection(scene *scene, ray_params params)// vector O, vector D, double t_min, double t_max)
{
    intersection_result result = {0};
    result.t = params.t_max;

    // Check spheres
    for (int i = 0; i < scene->sphere_count; i++) {
        double t1, t2;
        if (intersect_ray_sphere(params.O, params.D, &scene->spheres[i], &t1, &t2)) {
            if (t1 < result.t && t1 > params.t_min) {
                result.t = t1;
                result.type = SHAPE_SPHERE;
                result.object = &scene->spheres[i];
            }
            if (t2 < result.t && t2 > params.t_min) {
                result.t = t2;
                result.type = SHAPE_SPHERE;
                result.object = &scene->spheres[i];
            }
        }
    }

    // Check cylinders
    for (int i = 0; i < scene->cylinder_count; i++) {
        double t1, t2;
        if (intersect_ray_cylinder(params.O, params.D, &scene->cylinders[i], &t1, &t2)) {
            if (t1 < result.t && t1 > params.t_min) {
                result.t = t1;
                result.type = SHAPE_CYLINDER;
                result.object = &scene->cylinders[i];
            }
            if (t2 < result.t && t2 > params.t_min) {
                result.t = t2;
                result.type = SHAPE_CYLINDER;
                result.object = &scene->cylinders[i];
            }
        }
    }

    // Check planes
    for (int i = 0; i < scene->plane_count; i++) {
        double t;
        if (intersect_ray_plane(params.O, params.D, &scene->planes[i], &t)) {
            if (t < result.t && t > params.t_min) {
                result.t = t;
                result.type = SHAPE_PLANE;
                result.object = &scene->planes[i];
            }
        }
    }

    return result;
}

color compute_lighting(scene *scene, vector P, vector N, vector V, int specular)
{
    ray_params shadow_params;

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
                t_max = 1.0; //left in case we add directional light
            //}
			//else
			//{ // Directional
            //    L = light.direction;
            //    t_max = INFINITY;
            //}
            // Check for shadows
            //double shadow_t;
            //sphere *shadow_sphere =
            shadow_params.O = P;
            shadow_params.D = L;
            shadow_params.t_min = 0.001;
            shadow_params.t_max = t_max;

            intersection_result shadow_result = closest_intersection(scene, shadow_params);//P, L, 0.001, t_max);//, &shadow_t);
            //if (shadow_sphere != NULL) continue;
            if (shadow_result.t < shadow_params.t_max)
                continue; // In shadow, skip this light
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

// Function to compute the normal at a point on the cylinder
vector cyl_normal(vector p, cylinder *cyl)
{
    vector base = compute_base(cyl);
    vector top = compute_top(cyl);
    vector pa = vector_sub(p, base);  // pa = p - base
    vector ba = vector_sub(top, base);  // ba = top - base
    double baba = vector_dot(ba, ba);
    double h = vector_dot(pa, ba) / baba;
    vector normal = vector_sub(pa, vector_scale(ba, h));
    return vector_normalize(vector_scale(normal, 1.0 / cyl->radius));
}

//int trace_ray(scene *scene, vector O, vector D, double t_min, double t_max, int depth)
vector compute_normal(intersection_result result, vector P)
{
    vector N = {0, 0, 0};
    if (result.type == SHAPE_SPHERE)
        N = vector_normalize(vector_sub(P, ((sphere *)result.object)->center));
    else if (result.type == SHAPE_CYLINDER)
        N = cyl_normal(P, (cylinder *)result.object);
    else if (result.type == SHAPE_PLANE)
        N = ((plane *)result.object)->normal;
    return (N);
}

int color_to_int(color local_color)
{
    int final_red;
    int final_green;
    int final_blue;

    final_red = (int)(local_color.red * 255);
    final_green = (int)(local_color.green * 255);
    final_blue = (int)(local_color.blue * 255);
    if (final_red > 255)
        final_red = 255;
    if (final_green > 255)
        final_green = 255;
    if (final_blue > 255)
        final_blue = 255;
    return ((final_red << 16) | (final_green << 8) | final_blue);
}

base_shape* get_base_shape(intersection_result result)
{
    base_shape *shape = (base_shape *)result.object;
    if (result.type == SHAPE_SPHERE)
        shape = &((sphere *)result.object)->base;
    else if (result.type == SHAPE_CYLINDER)
        shape = &((cylinder *)result.object)->base;
    else if (result.type == SHAPE_PLANE)
        shape = &((plane *)result.object)->base;
    return shape;
}

int trace_ray(scene *scene, ray_params params, int depth)
{
    trace vars;
    intersection_result result;

    result = closest_intersection(scene, params);
    if (result.t == params.t_max)
        return BACKGROUND_COLOR; // Background color
    // Compute local color at the intersection point
    vars.P = vector_add(params.O, vector_scale(params.D, result.t)); // Intersection point
    vars.N = compute_normal(result, vars.P);
    vars.V = vector_scale(params.D, -1); // View direction

    // Determine the shape and compute lighting
    vars.shape = get_base_shape(result);
    /* vars.shape = (base_shape *)result.object;
    if (result.type == SHAPE_SPHERE)
        vars.shape = &((sphere *)result.object)->base;
    else if (result.type == SHAPE_CYLINDER)
        vars.shape = &((cylinder *)result.object)->base;
    else if (result.type == SHAPE_PLANE)
        vars.shape = &((plane *)result.object)->base; */

    // Compute lighting based on the specular value of the shape
    vars.lighting = compute_lighting(scene, vars.P, vars.N, vars.V, vars.shape->specular);

    // Calculate local color using the shape's color attributes
    vars.local_color.red = (vars.shape->red / 255.0) * vars.lighting.red;
    vars.local_color.green = (vars.shape->green / 255.0) * vars.lighting.green;
    vars.local_color.blue = (vars.shape->blue / 255.0) * vars.lighting.blue;
    vars.r = vars.shape->reflective;
    if (depth <= 0 || vars.r <= 0)
        return color_to_int(vars.local_color);

    // Compute reflection vector
    vars.R = vector_reflect(vars.V, vars.N);
    params.O = vars.P;
    params.D = vars.R;
    params.t_min = 0.001;
    params.t_max = INFINITY;
    // Trace the reflected ray
    int reflected_color_int = trace_ray(scene, params, depth - 1);

    vars.reflected_color = (color){(reflected_color_int >> 16 & 0xFF) / 255.0, (reflected_color_int >> 8 & 0xFF) / 255.0, (reflected_color_int & 0xFF) / 255.0};
    // Combine local color and reflected color based on reflectivity

    vars.final_color.red = vars.local_color.red * (1 - vars.r) + vars.reflected_color.red * vars.r;
    vars.final_color.green = vars.local_color.green * (1 - vars.r) + vars.reflected_color.green * vars.r;
    vars.final_color.blue = vars.local_color.blue * (1 - vars.r) + vars.reflected_color.blue * vars.r;
    return color_to_int(vars.final_color);
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
    ray_params params;
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
            params.O = camera_position;
            params.D = D;
            params.t_min = 1.0;
            params.t_max = INFINITY;
            int color = trace_ray(scene, params, 3);
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
    scene.spheres[0] = (sphere){{0, -1, 3}, 1, {255, 0, 0, 500, 0.2}};
    // Blue sphere
   // scene.spheres[1] = (sphere){{-2, 0, 4}, 1, 0, 0, 255, 10, 0.3};
    // Green sphere
    scene.spheres[2] = (sphere){{2, 0, 4}, 1, {0, 255, 0, 10, 0.4}};
    // Yellow large sphere (floor)
    //scene.spheres[3] = (sphere){{0, -5001, 0}, 5000, 255, 255, 0, 1000, 0.5};

    // Example plane
    scene.plane_count = 1;
    scene.planes = malloc(sizeof(plane) * scene.plane_count);
    scene.planes[0] = (plane){{0, -1, 0}, {0, 1, 0}, {255, 255, 0, 500, 0.5}};

    scene.cylinder_count = 1;
    scene.cylinders = malloc(sizeof(cylinder) * scene.cylinder_count);

    // Example cylinder
    scene.cylinders[0] = (cylinder){{-1, -1, 4}, 1, 2, {0, 1, 0}, {0, 0, 255, 500, 0.3}};


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