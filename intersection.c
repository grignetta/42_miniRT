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