#include "minirt_bonus.h"

int	solve_quadratic(t_quadratic *quad, double *t0, double *t1)
{
	double	discriminant;
	double	sqrt_discriminant;

	discriminant = quad->b * quad->b - 4 * quad->a * quad->c;
	if (discriminant < 0)
		return (0);
	sqrt_discriminant = sqrt(discriminant);
	*t0 = (-quad->b - sqrt_discriminant) / (2 * quad->a);
	*t1 = (-quad->b + sqrt_discriminant) / (2 * quad->a);
	return (1);
}

int	cross_ray_inf_cyl(t_ray_params params, t_cylinder *cyl,
	double *t1, double *t2)
{
	t_vector		co;
	t_vector		d_perp;
	t_vector		co_perp;
	t_quadratic		quad;

	co = vector_sub(params.o, cyl->center);
	d_perp = vector_sub(params.d, vector_scale(cyl->axis,
				vector_dot(params.d, cyl->axis)));
	co_perp = vector_sub(co, vector_scale(cyl->axis,
				vector_dot(co, cyl->axis)));
	quad.a = vector_dot(d_perp, d_perp);
	quad.b = 2 * vector_dot(d_perp, co_perp);
	quad.c = vector_dot(co_perp, co_perp) - cyl->radius * cyl->radius;
	if (!solve_quadratic(&quad, t1, t2))
		return (0);
	return (1);
}

int	is_within_caps(t_vector p, t_cylinder *cyl)
{
	double	axis_dist;
	double	half_h;

	axis_dist = vector_dot(vector_sub(p, cyl->center), cyl->axis);
	half_h = cyl->height / 2;
	return (axis_dist >= -half_h && axis_dist <= half_h);
}

void	handle_side_intersect(t_ray_params params, t_cylinder *cyl,
		t_intersect_result *result)
{
	double		t1;
	double		t2;
	t_vector	p1;
	t_vector	p2;

	if (cross_ray_inf_cyl(params, cyl, &t1, &t2))
	{
		p1 = vector_add(params.o, vector_scale(params.d, t1));
		if (!is_within_caps(p1, cyl))
			t1 = INFINITY;
		if (update_result(result, t1, cyl, params))
			update_cyl_result(result, 0);
		p2 = vector_add(params.o, vector_scale(params.d, t2));
		if (!is_within_caps(p2, cyl))
			t2 = INFINITY;
		if (update_result(result, t2, cyl, params))
			update_cyl_result(result, 0);
	}
}
