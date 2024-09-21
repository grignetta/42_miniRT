#include "minirt.h"

/* typedef struct
{
    vector CO;
    double a;
	double b;
	double c;
    double discriminant;
} intersection; */

// Function to calculate the intersection of a ray with a sphere
int intersect_ray_sphere(vector O, vector D, sphere *sphere, double *t1, double *t2)
{
	intersection point;

    point.CO = vector_sub(O, sphere->center);
    point.a = vector_dot(D, D);
    point.b = 2 * vector_dot(point.CO, D);
    point.c = vector_dot(point.CO, point.CO) - sphere->radius * sphere->radius;

    point.discriminant = point.b * point.b - 4 * point.a * point.c;
    if (point.discriminant < 0) return 0; // No intersection (return inf, inf)?

    *t1 = (-point.b + sqrt(point.discriminant)) / (2 * point.a);
    *t2 = (-point.b - sqrt(point.discriminant)) / (2 * point.a);
    return (1);
}

int intersect_ray_cylinder(vector O, vector D, cylinder *cyl, double *t1, double *t2)
{
	intersection point;
    // Translate the ray and cylinder to a common origin, typically the cylinder's center
    point.CO = vector_sub(O, cyl->center);

    // Coefficients for the quadratic equation
    point.a = D.x * D.x + D.z * D.z;
    point.b = 2 * (point.CO.x * D.x + point.CO.z * D.z);
    point.c = point.CO.x * point.CO.x + point.CO.z * point.CO.z - cyl->radius * cyl->radius;

    // Discriminant
    point.discriminant = point.b * point.b - 4 * point.a * point.c;
    if (point.discriminant < 0)
        return 0; // No intersection
    *t1 = (-point.b + sqrt(point.discriminant)) / (2 * point.a);
    *t2 = (-point.b - sqrt(point.discriminant)) / (2 * point.a);
	
    // Check if the intersection points are within the cylinder's height
    double y1 = O.y + *t1 * D.y;
    double y2 = O.y + *t2 * D.y;
    if (y1 < cyl->center.y || y1 > cyl->center.y + cyl->height)
        *t1 = INFINITY;
    if (y2 < cyl->center.y || y2 > cyl->center.y + cyl->height)
        *t2 = INFINITY;
    return (1);
}

int intersect_ray_plane(vector O, vector D, plane *pl, double *t)
{
    double denom;

	denom = vector_dot(pl->normal, D);
    if (fabs(denom) > 1e-6)
	{ // Avoid division by zero and small values that might cause numerical instability
        vector P0L0 = vector_sub(pl->point, O);
        *t = vector_dot(P0L0, pl->normal) / denom;
		if (*t >= 0)
        	return (1); // intersection exists
    }
    return 0; // No intersection
}