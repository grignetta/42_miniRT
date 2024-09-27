#include "minirt.h"

// Function to compute the normal at a point on the cylinder
/* vector cyl_normal(vector p, cylinder *cyl)
{
    vector base = compute_base(cyl);
    vector top = compute_top(cyl);
    vector pa = vector_sub(p, base);  // pa = p - base
    vector ba = vector_sub(top, base);  // ba = top - base
    double baba = vector_dot(ba, ba);
    double h = vector_dot(pa, ba) / baba;
    vector normal = vector_sub(pa, vector_scale(ba, h));
    return vector_normalize(vector_scale(normal, 1.0 / cyl->radius));
} */

vector cyl_normal(vector P, cylinder *cyl, int surface)
{
    vector N = {0, 0, 0};
    // double y_bottom = cyl->center.y;
    // double y_top = cyl->center.y + cyl->height;
    // double epsilon = 1e-6; // Small value to handle floating-point precision

    // if (fabs(P.y - y_bottom) < epsilon) {
    //     // Bottom cap
    //     N = vector_init(0, -1, 0); // Normal pointing downwards
    // } else if (fabs(P.y - y_top) < epsilon) {
    //     // Top cap
    //     N = vector_init(0, 1, 0); // Normal pointing upwards
    // } else {
    //     // Side surface
    //     vector temp = vector_sub(P, cyl->center);
    //     temp.y = 0; // Project onto the XZ plane
    //     N = vector_normalize(temp);
    // }
    if (surface == 0) // Side surface
    {
        // Compute the vector from the cylinder center to point P, projected onto the plane perpendicular to the axis
        vector CP = vector_sub(P, cyl->center);
        vector N_perp = vector_sub(CP, vector_scale(cyl->axis, vector_dot(CP, cyl->axis)));
        N = vector_normalize(N_perp);
    }
    else if (surface == 1) // Bottom cap
    {
        N = vector_scale(cyl->axis, -1); // Normal pointing outward
    }
    else if (surface == 2) // Top cap
    {
        N = cyl->axis; // Normal pointing outward
    }
    return N;
}



//int trace_ray(scene *scene, vector O, vector D, double t_min, double t_max, int depth)
vector compute_normal(intersect_result result, vector P)
{
    //cylinder *c;

    vector N = {0, 0, 0};
    if (result.type == SHAPE_SPHERE)
        N = vector_normalize(vector_sub(P, ((sphere *)result.object)->center));
    else if (result.type == SHAPE_CYLINDER)
    {
        // c = (cylinder *)result.object;
        // if (result.surface == 0)
        //     // Side surface
        //     N = cyl_normal(P, c);
        // else if (result.surface == 1)
        //     // Bottom cap
        //     N = vector_init(0, -1, 0);
        // else if (result.surface == 2)
        //     // Top cap
        //     N = vector_init(0, 1, 0);
        N = cyl_normal(P, (cylinder *)result.object, result.surface);
    }
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
    check_limit_int(&final_red, 255);
    check_limit_int(&final_green, 255);
    check_limit_int(&final_blue, 255);
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

base_shape* get_base_shape(intersect_result result)
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

color get_plane_color(vector P, plane *pl)
{
    color base_color = {
        .red = pl->base.red / 255.0,
        .green = pl->base.green / 255.0,
        .blue = pl->base.blue / 255.0
    };
    if (pl->square_size == 0)
        return (base_color);
    // Calculate the texture coordinates
    int x_square = (int)floor(P.x / pl->square_size);
    int z_square = (int)floor(P.z / pl->square_size);

    // Determine if the sum is even or odd
    int sum = x_square + z_square;

    color inverse_color = {
        .red = 1.0 - base_color.red,
        .green = 1.0 - base_color.green,
        .blue = 1.0 - base_color.blue
    };

    if (sum % 2 == 0)
        return base_color;
    else
        return inverse_color;
}

int trace_ray(scene *scene, ray_params params, int depth)
{
    trace vars;
    intersect_result result;

    result = closest_intersection(scene, params);
    if (result.t == params.t_max)
        return BACKGROUND_COLOR; // Background color
    // Compute local color at the intersection point
    vars.P = vector_add(params.O, vector_scale(params.D, result.t)); // Intersection point
    vars.N = compute_normal(result, vars.P);
    vars.V = vector_scale(params.D, -1); // View direction
    //vars.V = vector_normalize(vars.V);//maybe not necessary
    // Determine the shape and compute lighting
    vars.shape = get_base_shape(result);

    // Compute lighting based on the specular value of the shape
    vars.lighting = compute_lighting(scene, vars);//vars.P, vars.N, vars.V, vars.shape->specular);

    // **Add Checkerboard Pattern for Planes**
    if (result.type == SHAPE_PLANE)
    {
        plane *pl = (plane *)result.object;
        color checker_color = get_plane_color(vars.P, pl);
        vars.local_color.red = checker_color.red * vars.lighting.red;
        vars.local_color.green = checker_color.green * vars.lighting.green;
        vars.local_color.blue = checker_color.blue * vars.lighting.blue;
    }
    else
    {
        // For other shapes, use their base color
        vars.local_color.red = (vars.shape->red / 255.0) * vars.lighting.red;
        vars.local_color.green = (vars.shape->green / 255.0) * vars.lighting.green;
        vars.local_color.blue = (vars.shape->blue / 255.0) * vars.lighting.blue;
    }
    //Calculate local color using the shape's color attributes
    // vars.local_color.red = (vars.shape->red / 255.0) * vars.lighting.red;
    // vars.local_color.green = (vars.shape->green / 255.0) * vars.lighting.green;
    // vars.local_color.blue = (vars.shape->blue / 255.0) * vars.lighting.blue;
    vars.r = vars.shape->reflective;
    if (depth <= 0 || vars.r <= 0)
        return color_to_int(vars.local_color);
    // Compute reflection vector
    vars.R = vector_reflect(vars.V, vars.N);
    params.O = vars.P;
    params.D = vars.R;
    params.t_min = 0.001;
    params.t_max = INFINITY; //was declared in the beginning
    // Trace the reflected ray
    int reflected_color_int = trace_ray(scene, params, depth - 1);
    vars.reflected_color = color_to_double(reflected_color_int);
    // Combine local color and reflected color based on reflectivity
    vars.final_color.red = vars.local_color.red * (1 - vars.r) + vars.reflected_color.red * vars.r;
    vars.final_color.green = vars.local_color.green * (1 - vars.r) + vars.reflected_color.green * vars.r;
    vars.final_color.blue = vars.local_color.blue * (1 - vars.r) + vars.reflected_color.blue * vars.r;

    return color_to_int(vars.final_color);
}