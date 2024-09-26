#include "minirt.h"

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

int intersect_ray_sphere(ray_params params, sphere *sphere,
        intersection_result *result)
{
	double t1;
	double t2;
	intersection point;

    point.CO = vector_sub(params.O, sphere->center);
    point.a = vector_dot(params.D, params.D);
    point.b = 2 * vector_dot(point.CO, params.D);
    point.c = vector_dot(point.CO, point.CO) - sphere->radius * sphere->radius;
    point.discriminant = point.b * point.b - 4 * point.a * point.c;
    if (point.discriminant < 0)
        return (0); // No intersection (return inf, inf)?
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
    if (fabs(denom) > 1e-6) // Avoid division by zero and small values that might cause numerical instability
	{
        *t = vector_dot(vector_sub(pl->point, params.O), pl->normal) / denom;
		if (*t >= 0)
        	return (1); // intersection exists
    }
    return 0; // No intersection
}

typedef struct
{
    double a;
    double b;
    double c;
} quadratic;

void compute_cap_centers(cylinder *cyl, vector *bottom_center, vector *top_center)
{
    vector half_h_v;

    half_h_v = vector_scale(cyl->axis, cyl->height / 2);
    *bottom_center = vector_sub(cyl->center, half_h_v);
    *top_center = vector_add(cyl->center, half_h_v);
}

int solve_quadratic(quadratic *quad, double *t0, double *t1)
{
    // // Check if the equation is quadratic
    // if (fabs(quad->a) < 1e-8)
    // {
    //     if (fabs(quad->b) < 1e-8)
    //         return (0); // Not an equation
    //     else
    //     {
    //         // Linear equation
    //         quad->t0 = quad->t1 = -quad->c / quad->b;
    //         return (1);
    //     }
    // }
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

//int intersect_ray_with_cap(ray_params params, vector cap_center, vector cap_normal, double radius, double *t)
int intersect_ray_with_cap(ray_params params, cylinder *cyl, double *t)
{
    plane cap_plane;
    cap_plane.point = cyl->cap_center;
    cap_plane.normal = cyl->cap_normal;

    if (intersect_ray_plane(params, &cap_plane, t))
    {
        vector P = vector_add(params.O, vector_scale(params.D, *t));
        vector dist_vec = vector_sub(P, cyl->cap_center);
        double dist_sq = vector_dot(dist_vec, dist_vec);

        if (dist_sq <= cyl->radius * cyl->radius)
            return 1; // Intersection with cap
    }

    return 0; // No intersection with cap
}

void update_cylinder_result(intersection_result *result, double t, cylinder *cyl, int surface, ray_params params)
{
    if (update_result(result, t, cyl, params))
    {
        result->type = SHAPE_CYLINDER;
        result->surface = surface;
    }
}

int intersect_ray_cylinder(ray_params params, cylinder *cyl, intersection_result *result)
{
    vector bottom_center, top_center;
    compute_cap_centers(cyl, &bottom_center, &top_center);

    double t1, t2;

    if (cross_ray_inf_cyl(params, cyl, &t1, &t2))
    {
        // Check t1
        vector P1 = vector_add(params.O, vector_scale(params.D, t1));
        if (is_within_caps(P1, cyl))
        {
            update_cylinder_result(result, t1, cyl, 0, params); // Side surface
        }
        else
        {
            t1 = INFINITY;
        }

        // Check t2
        vector P2 = vector_add(params.O, vector_scale(params.D, t2));
        if (is_within_caps(P2, cyl))
        {
            update_cylinder_result(result, t2, cyl, 0, params); // Side surface
        }
        else
        {
            t2 = INFINITY;
        }
    }

    // Intersect with bottom cap
    double t_cap;
    cyl->cap_center = bottom_center;
    cyl->cap_normal = vector_scale(cyl->axis, -1);
    if (intersect_ray_with_cap(params, cyl, &t_cap))
    {
        update_cylinder_result(result, t_cap, cyl, 1, params); // Bottom cap
    }

    // Intersect with top cap
    cyl->cap_center = top_center;
    cyl->cap_normal = cyl->axis;
    if (intersect_ray_with_cap(params, cyl, &t_cap))
    {
        update_cylinder_result(result, t_cap, cyl, 2, params); // Top cap
    }

    // Determine if any valid intersections were found
    return (result->t < INFINITY);
}



int intersect_ray_cylinder_1(ray_params params, cylinder *cyl, intersection_result *result)
{
    // Compute cap centers
    vector half_h_v = vector_scale(cyl->axis, cyl->height / 2);
    vector bottom_center = vector_sub(cyl->center, half_h_v);
    vector top_center = vector_add(cyl->center, half_h_v);

	double t1;
	double t2;
	intersection point;
    // Translate the ray and cylinder to a common origin, typically the cylinder's center
    point.CO = vector_sub(params.O, cyl->center);

    // Compute the components perpendicular to the axis
    vector D_perp = vector_sub(params.D, vector_scale(cyl->axis, vector_dot(params.D, cyl->axis)));
    vector CO_perp = vector_sub(point.CO, vector_scale(cyl->axis, vector_dot(point.CO, cyl->axis)));

    // Coefficients for the quadratic equation
    point.a = vector_dot(D_perp, D_perp);
    point.b = 2 * vector_dot(D_perp, CO_perp);
    point.c = vector_dot(CO_perp, CO_perp) - cyl->radius * cyl->radius;

    // Discriminant
    point.discriminant = point.b * point.b - 4 * point.a * point.c;
    if (point.discriminant < 0)
        return 0; // No intersection
    t2 = (-point.b + sqrt(point.discriminant)) / (2 * point.a);//t1
    t1 = (-point.b - sqrt(point.discriminant)) / (2 * point.a);//t2

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