#include "minirt.h"

// Function to calculate the intersection of a ray with a sphere
//if (intersect_ray_sphere(params.O, params.D, &scene->spheres[i], &t1, &t2))
int update_result(intersection_result *result, double t, void *object, ray_params params)
{
    if (t < result->t && t > params.t_min)
    {
        result->t = t;
        //result->type = type;
        result->object = object;
		return (1);
    }
	return (0);
}

//int intersect_ray_sphere(ray_params params, sphere *sphere, double *t1, double *t2)
int intersect_ray_sphere(ray_params params, sphere *sphere, intersection_result *result)
{
	double t1;
	double t2;
	intersection point;

    point.CO = vector_sub(params.O, sphere->center);
    point.a = vector_dot(params.D, params.D);
    point.b = 2 * vector_dot(point.CO, params.D);
    point.c = vector_dot(point.CO, point.CO) - sphere->radius * sphere->radius;

    point.discriminant = point.b * point.b - 4 * point.a * point.c;
    if (point.discriminant < 0) return 0; // No intersection (return inf, inf)?

    t1 = (-point.b + sqrt(point.discriminant)) / (2 * point.a);
    t2 = (-point.b - sqrt(point.discriminant)) / (2 * point.a);
	if (update_result(result, t1, sphere, params))
        result->type = SHAPE_SPHERE;
    if (update_result(result, t2, sphere, params))
		result->type = SHAPE_SPHERE;
    return (1);
}


int intersect_ray_plane(ray_params params, plane *pl, double *t)
{
    double denom;

	denom = vector_dot(pl->normal, params.D);
    if (fabs(denom) > 1e-6)
	{ // Avoid division by zero and small values that might cause numerical instability
        vector P0L0 = vector_sub(pl->point, params.O);
        *t = vector_dot(P0L0, pl->normal) / denom;
		if (*t >= 0)
        	return (1); // intersection exists
    }
    return 0; // No intersection
}

int intersect_ray_cylinder(ray_params params, cylinder *cyl, intersection_result *result)
{
    // Compute cap centers
    vector half_height_vector = vector_scale(cyl->axis, cyl->height / 2);
    vector bottom_center = vector_sub(cyl->center, half_height_vector);
    vector top_center = vector_add(cyl->center, half_height_vector);

	double t1;
	double t2;
	intersection point;
    // Translate the ray and cylinder to a common origin, typically the cylinder's center
    point.CO = vector_sub(params.O, cyl->center);

    // Compute the components perpendicular to the axis
    vector D_perp = vector_sub(params.D, vector_scale(cyl->axis, vector_dot(params.D,     cyl->axis)));
    vector CO_perp = vector_sub(point.CO, vector_scale(cyl->axis, vector_dot(point.CO,  cyl->axis)));

    // Coefficients for the quadratic equation
    // point.a = params.D.x * params.D.x + params.D.z * params.D.z;
    // point.b = 2 * (point.CO.x * params.D.x + point.CO.z * params.D.z);
    // point.c = point.CO.x * point.CO.x + point.CO.z * point.CO.z - cyl->radius * cyl->radius;

    point.a = vector_dot(D_perp, D_perp);
    point.b = 2 * vector_dot(D_perp, CO_perp);
    point.c = vector_dot(CO_perp, CO_perp) - cyl->radius * cyl->radius;

    // Discriminant
    point.discriminant = point.b * point.b - 4 * point.a * point.c;
    if (point.discriminant < 0)
        return 0; // No intersection
    t2 = (-point.b + sqrt(point.discriminant)) / (2 * point.a);//t1
    t1 = (-point.b - sqrt(point.discriminant)) / (2 * point.a);//t2

    // Check if the intersection points are within the cylinder's height
    // double y1 = params.O.y + t1 * params.D.y;
    // double y2 = params.O.y + t2 * params.D.y;
    // if (y1 < cyl->center.y || y1 > cyl->center.y + cyl->height)
    //     t1 = INFINITY;
    // if (y2 < cyl->center.y || y2 > cyl->center.y + cyl->height)
    //     {t2 = INFINITY;}
    // For t1
    vector P1 = vector_add(params.O, vector_scale(params.D, t1));
    double axis_dist1 = vector_dot(vector_sub(P1, cyl->center), cyl->axis);
    if (axis_dist1 < -cyl->height / 2 || axis_dist1 > cyl->height / 2)
        t1 = INFINITY; // Intersection is outside the caps

    // For t2
    vector P2 = vector_add(params.O, vector_scale(params.D, t2));
    double axis_dist2 = vector_dot(vector_sub(P2, cyl->center), cyl->axis);
    if (axis_dist2 < -cyl->height / 2 || axis_dist2 > cyl->height / 2)
        {t2 = INFINITY; }// Intersection is outside the caps
	if (update_result(result, t1, cyl, params))//two if statements are necessary
    {
        result->type = SHAPE_CYLINDER;
        result->surface = 0; // Side surface
    }
    if (update_result(result, t2, cyl, params))
	{
        result->type = SHAPE_CYLINDER;
        result->surface = 0; // Side surface
    }
    // Intersect with the bottom cap
    plane bottom_cap;
    // bottom_cap.point = cyl->center; // Center of the bottom cap
    // bottom_cap.normal = vector_init(0, -1, 0); // Normal pointing downwards
    bottom_cap.point = bottom_center;
    bottom_cap.normal = vector_scale(cyl->axis, -1); // Normal pointing outward from the cap


    double t_cap;
    if (intersect_ray_plane(params, &bottom_cap, &t_cap))
    {
        vector P = vector_add(params.O, vector_scale(params.D, t_cap));
        //vector dist_vec = vector_sub(P, cyl->center);
        vector dist_vec = vector_sub(P, bottom_cap.point);
        vector dist_vec_perp = vector_sub(dist_vec, vector_scale(cyl->axis, vector_dot(dist_vec, cyl->axis)));
        double dist_sq = vector_dot(dist_vec_perp, dist_vec_perp);
        if (dist_sq <= cyl->radius * cyl->radius)
        {
            if (update_result(result, t_cap, cyl, params))
            {
                result->type = SHAPE_CYLINDER;
                result->surface = 1; // Bottom cap
            }
        }
    }

    // Intersect with the top cap
    plane top_cap;
    // top_cap.point = vector_add(cyl->center, vector_init(0, cyl->height, 0)); // Center of the top cap
    // top_cap.normal = vector_init(0, 1, 0); // Normal pointing upwards
    top_cap.point = top_center;
    top_cap.normal = cyl->axis; // Normal pointing outward from the cap

    if (intersect_ray_plane(params, &top_cap, &t_cap))
    {
        vector P = vector_add(params.O, vector_scale(params.D, t_cap));
        vector dist_vec = vector_sub(P, top_cap.point);
        vector dist_vec_perp = vector_sub(dist_vec, vector_scale(cyl->axis, vector_dot(dist_vec, cyl->axis)));
        //double dist_sq = dist_vec.x * dist_vec.x + dist_vec.z * dist_vec.z;
        double dist_sq = vector_dot(dist_vec_perp, dist_vec_perp);
        if (dist_sq <= cyl->radius * cyl->radius)
        {
            if (update_result(result, t_cap, cyl, params))
            {
                result->type = SHAPE_CYLINDER;
                result->surface = 2; // Top cap
            }
        }
    }

    return (1);
}


// Closest intersection function
intersection_result closest_intersection(scene *scene, ray_params params)// vector O, vector D, double t_min, double t_max)
{
    intersection_result result;
	int i;
    double t;

    result.object = NULL;
    result.type = SHAPE_NONE;
    result.t = params.t_max;
    // Check spheres
	i = -1;
	while (++i < scene->sphere_count)
		intersect_ray_sphere(params, &scene->spheres[i], &result);
    // Check cylinders
	i = -1;
	while (++i < scene->cylinder_count)
		intersect_ray_cylinder(params, &scene->cylinders[i], &result);
    // Check planes
    i = -1;
	while (++i < scene->plane_count)
	{
        if (intersect_ray_plane(params, &scene->planes[i], &t)) {
            if (t < result.t && t > params.t_min) {
                result.t = t;
                result.type = SHAPE_PLANE;
                result.object = &scene->planes[i];
            }
        }
    }
    return (result);
}