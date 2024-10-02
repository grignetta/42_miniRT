#include "minirt.h"

int	update_result(intersect_result *result, double t,
	void *object, ray_params params)
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

int	cross_ray_sphere(ray_params params, sphere *sphere,
		intersect_result *result)
{
	double			t1;
	double			t2;
	intersection	point;

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

int	cross_ray_plane(ray_params params, plane *pl, double *t)
{
	double	denom;

	denom = vector_dot(pl->normal, params.D);
	if (fabs(denom) > 1e-6) // Avoid division by zero and small values that might cause numerical instability
	{
		*t = vector_dot(vector_sub(pl->point, params.O), pl->normal) / denom;
		if (*t >= 0)
			return (1); // intersection exists
	}
	return (0); // No intersection
}

// Closest intersection function
intersect_result	closest_intersection(scene *scene, ray_params params)// vector O, vector D, double t_min, double t_max)
{
	intersect_result	result;
	int					i;
	double				t;

	result.object = NULL;
	result.type = SHAPE_NONE;
	result.t = params.t_max;
	// Check spheres
	i = -1;
	while (++i < scene->sphere_count)
		cross_ray_sphere(params, &scene->spheres[i], &result);
	// Check cylinders
	i = -1;
	while (++i < scene->cylinder_count)
		cross_ray_cyl(params, &scene->cylinders[i], &result);
	// Check planes
	i = -1;
	while (++i < scene->plane_count)
	{
		if (cross_ray_plane(params, &scene->planes[i], &t))
		{
			if (t < result.t && t > params.t_min)
			{
				result.t = t;
				result.type = SHAPE_PLANE;
				result.object = &scene->planes[i];
			}
		}
	}
	return (result);
}
