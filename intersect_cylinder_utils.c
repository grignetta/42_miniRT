#include "minirt.h"

int solve_quadratic(quadratic *quad, double *t0, double *t1)
{
    double discriminant;
    double sqrt_discriminant;

    discriminant = quad->b * quad->b - 4 * quad->a * quad->c;
    if (discriminant < 0)
        return (0); // No real roots
    sqrt_discriminant = sqrt(discriminant);
    *t0 = (-quad->b - sqrt_discriminant) / (2 * quad->a);
    *t1 = (-quad->b + sqrt_discriminant) / (2 * quad->a);
    return (1);
}

int cross_ray_inf_cyl(ray_params params, cylinder *cyl, double *t1, double *t2)
{
    vector CO;
    vector D_perp;
    vector CO_perp;
    quadratic quad;

    // Translate the ray origin
    CO = vector_sub(params.O, cyl->center);

    // Compute components perpendicular to the cylinder's axis
    D_perp = vector_sub(params.D, vector_scale(cyl->axis, vector_dot(params.D, cyl->axis)));
    CO_perp = vector_sub(CO, vector_scale(cyl->axis, vector_dot(CO, cyl->axis)));

    // Quadratic coefficients
    quad.a = vector_dot(D_perp, D_perp);
    quad.b = 2 * vector_dot(D_perp, CO_perp);
    quad.c = vector_dot(CO_perp, CO_perp) - cyl->radius * cyl->radius;

    if (!solve_quadratic(&quad, t1, t2))
        return 0; // No real roots
    return 1; // Intersections exist
}

int is_within_caps(vector P, cylinder *cyl)
{
    double axis_dist;
    double half_h;

    axis_dist = vector_dot(vector_sub(P, cyl->center), cyl->axis);
    half_h = cyl->height / 2;

    return (axis_dist >= -half_h && axis_dist <= half_h);
}

void handle_side_intersections(ray_params params, cylinder *cyl, intersection_result *result)
{
    double t1;
    double t2;
    vector P1;
    vector P2;

    if (cross_ray_inf_cyl(params, cyl, &t1, &t2))
    {
        // Check t1
        P1 = vector_add(params.O, vector_scale(params.D, t1));
        if (!is_within_caps(P1, cyl))
            t1 = INFINITY; // Intersection is outside the caps
        if (update_result(result, t1, cyl, params))
            update_cylinder_result(result, 0); // Side surface
        // Check t2
        P2 = vector_add(params.O, vector_scale(params.D, t2));
        if (!is_within_caps(P2, cyl))
            t2 = INFINITY; // Intersection is outside the caps
        if (update_result(result, t2, cyl, params))
            update_cylinder_result(result, 0); // Side surface
    }
}
