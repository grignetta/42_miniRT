#include "minirt_bonus.h"

void	cap_centers(t_cylinder *cyl, t_vector *bottom_center,
		t_vector *top_center)
{
	t_vector	half_h_v;

	half_h_v = vector_scale(cyl->axis, cyl->height / 2);
	*bottom_center = vector_sub(cyl->center, half_h_v);
	*top_center = vector_add(cyl->center, half_h_v);
}

int	cross_ray_cap(t_ray_params params, t_cylinder *cyl, double *t)
{
	t_plane		cap_plane;
	t_vector	p;
	t_vector	dist_vec;
	double		dist_sq;

	cap_plane.point = cyl->cap_center;
	cap_plane.normal = cyl->cap_normal;
	if (cross_ray_plane(params, &cap_plane, t))
	{
		p = vector_add(params.o, vector_scale(params.d, *t));
		dist_vec = vector_sub(p, cyl->cap_center);
		dist_sq = vector_dot(dist_vec, dist_vec);
		if (dist_sq <= cyl->radius * cyl->radius)
			return (1);
	}
	return (0);
}

void	update_cyl_result(t_intersect_result *result, int surface)
{
	result->type = SHAPE_CYLINDER;
	result->surface = surface;
}

void	cross_ray_cyl(t_ray_params params, t_cylinder *cyl,
		t_intersect_result *result)
{
	t_vector	bottom_center;
	t_vector	top_center;
	double		t_cap;

	cyl->axis = vector_normalize(cyl->axis);//added
	cap_centers(cyl, &bottom_center, &top_center);
	handle_side_intersect(params, cyl, result);
	cyl->cap_center = bottom_center;
	cyl->cap_normal = vector_scale(cyl->axis, -1);
	if (cross_ray_cap(params, cyl, &t_cap))
		if (update_result(result, t_cap, cyl, params))
			update_cyl_result(result, 1);
	cyl->cap_center = top_center;
	cyl->cap_normal = cyl->axis;
	if (cross_ray_cap(params, cyl, &t_cap))
		if (update_result(result, t_cap, cyl, params))
			update_cyl_result(result, 2);
}
