#include "minirt.h"

int	update_result(t_intersect_result *result, double t,
	void *object, t_ray_params params)
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

int	cross_ray_sphere(t_ray_params params, t_sphere *sphere,
		t_intersect_result *result)
{
	double			t1;
	double			t2;
	t_intersection	point;

	point.co = vector_sub(params.o, sphere->center);
	point.a = vector_dot(params.d, params.d);
	point.b = 2 * vector_dot(point.co, params.d);
	point.c = vector_dot(point.co, point.co) - sphere->radius * sphere->radius;
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

int	cross_ray_plane(t_ray_params params, t_plane *pl, double *t)
{
	double	denom;

	denom = vector_dot(pl->normal, params.d);
	if (fabs(denom) > 1e-6) // Avoid division by zero and small values that might cause numerical instability
	{
		*t = vector_dot(vector_sub(pl->point, params.o), pl->normal) / denom;
		if (*t >= 0)
			return (1); // intersection exists
	}
	return (0); // No intersection
}

// Closest intersection function
t_intersect_result	closest_intersection(t_scene *scene, t_ray_params params)// vector o, vector d, double t_min, double t_max)
{
	t_intersect_result	result;
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
