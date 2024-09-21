#include "minirt.h"

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

color color_to_double(int color_int)
{
    color result;

    result.red = ((color_int >> 16) & 0xFF) / 255.0;
    result.green = ((color_int >> 8) & 0xFF) / 255.0;
    result.blue = (color_int & 0xFF) / 255.0;
    return (result);
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
    //params.t_max = INFINITY; was declared in the beginning
    // Trace the reflected ray
    int reflected_color_int = trace_ray(scene, params, depth - 1);
    vars.reflected_color = color_to_double(reflected_color_int);
    // Combine local color and reflected color based on reflectivity
    vars.final_color.red = vars.local_color.red * (1 - vars.r) + vars.reflected_color.red * vars.r;
    vars.final_color.green = vars.local_color.green * (1 - vars.r) + vars.reflected_color.green * vars.r;
    vars.final_color.blue = vars.local_color.blue * (1 - vars.r) + vars.reflected_color.blue * vars.r;
    return color_to_int(vars.final_color);
}