#include "minirt.h"

void compute_cap_centers(cylinder *cyl, vector *bottom_center, vector *top_center)
{
    vector half_h_v;

    half_h_v = vector_scale(cyl->axis, cyl->height / 2);
    *bottom_center = vector_sub(cyl->center, half_h_v);
    *top_center = vector_add(cyl->center, half_h_v);
}

int intersect_ray_with_cap(ray_params params, cylinder *cyl, double *t)
{
    plane   cap_plane;
    vector  P;
    vector dist_vec;
    double dist_sq;

    cap_plane.point = cyl->cap_center;
    cap_plane.normal = cyl->cap_normal;
    if (intersect_ray_plane(params, &cap_plane, t))
    {
        P = vector_add(params.O, vector_scale(params.D, *t));
        dist_vec = vector_sub(P, cyl->cap_center);
        dist_sq = vector_dot(dist_vec, dist_vec);

        if (dist_sq <= cyl->radius * cyl->radius)//different?
            return 1; // Intersection with cap
    }

    return 0; // No intersection with cap
}

void update_cylinder_result(intersection_result *result, int surface)
{
        result->type = SHAPE_CYLINDER;
        result->surface = surface;
}

int intersect_ray_cylinder(ray_params params, cylinder *cyl, intersection_result *result)
{
    vector bottom_center;
    vector top_center;

    compute_cap_centers(cyl, &bottom_center, &top_center);
    handle_side_intersections(params, cyl, result);

    // Intersect with bottom cap
    double t_cap;
    cyl->cap_center = bottom_center;
    cyl->cap_normal = vector_scale(cyl->axis, -1);
    if (intersect_ray_with_cap(params, cyl, &t_cap))
        if (update_result(result, t_cap, cyl, params))
            update_cylinder_result(result, 1); // Bottom cap

    // Intersect with top cap
    cyl->cap_center = top_center;
    cyl->cap_normal = cyl->axis;
    if (intersect_ray_with_cap(params, cyl, &t_cap))
        if (update_result(result, t_cap, cyl, params))
            update_cylinder_result(result, 2); // Top cap
    // Determine if any valid intersections were found
    //return (result->t < INFINITY);
    return (1);
}