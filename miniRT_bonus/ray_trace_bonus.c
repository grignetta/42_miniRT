#include "minirt_bonus.h"

t_base_shape	*get_base_shape(t_intersect_result result)
{
	t_base_shape	*shape;

	shape = (t_base_shape *)result.object;
	if (result.type == SHAPE_SPHERE)
		shape = &((t_sphere *)result.object)->base;
	else if (result.type == SHAPE_CYLINDER)
		shape = &((t_cylinder *)result.object)->base;
	else if (result.type == SHAPE_PLANE)
		shape = &((t_plane *)result.object)->base;
	return (shape);
}

//checkered pattern plane
t_color	get_plane_color(t_vector p, t_plane *pl)
{
	t_color	base_color;
	t_color	inverse_color;
	int		sum;
	int		x_square;
	int		z_square;

	base_color = (t_color){
		.red = pl->base.red / 255.0,
		.green = pl->base.green / 255.0,
		.blue = pl->base.blue / 255.0};
	if (pl->base.square_size == 0)
		return (base_color);
	x_square = (int)floor(p.x / pl->base.square_size);
	z_square = (int)floor(p.z / pl->base.square_size);
	sum = x_square + z_square;
	inverse_color = (t_color){
		.red = 1.0 - base_color.red,
		.green = 1.0 - base_color.green,
		.blue = 1.0 - base_color.blue
	};
	if (sum % 2 == 0)
		return (base_color);
	else
		return (inverse_color);
}

// surface 0 for side, 1 for bottom cap, 2 for top cap
t_vector	cyl_normal(t_vector p, t_cylinder *cyl, int surface)
{
	t_vector	n;
	t_vector	cp;
	t_vector	n_perp;

	n = (t_vector){0, 0, 0};
	if (surface == 0)
	{
		cp = vector_sub(p, cyl->center);
		n_perp = vector_sub(cp, vector_scale(cyl->axis,
					vector_dot(cp, cyl->axis)));
		n = vector_normalize(n_perp);
	}
	else if (surface == 1)
		n = vector_scale(cyl->axis, -1);
	else if (surface == 2)
		n = cyl->axis;
	return (n);
}

t_vector	compute_normal(t_intersect_result result, t_vector p)
{
	t_vector	n;

	n = (t_vector){0, 0, 0};
	if (result.type == SHAPE_SPHERE)
		n = vector_normalize(vector_sub(p,
					((t_sphere *)result.object)->center));
	else if (result.type == SHAPE_CYLINDER)
		n = cyl_normal(p, (t_cylinder *)result.object, result.surface);
	else if (result.type == SHAPE_PLANE)
		n = ((t_plane *)result.object)->normal;
	return (n);
}

int	trace_ray(t_scene *t_scene, t_ray_params params, int depth)
{
	t_trace				vars;
	t_intersect_result	result;
	int					reflected_color_int;

	result = closest_intersection(t_scene, params);
	if (result.t == params.t_max)
		return (BACKGROUND_COLOR);
	vars.p = vector_add(params.o, vector_scale(params.d, result.t));
	vars.n = compute_normal(result, vars.p);
	vars.v = vector_scale(params.d, -1);
	vars.shape = get_base_shape(result);
	vars.lighting = compute_lighting(t_scene, vars);
	color_and_light(&vars, result);
	vars.r = vars.shape->reflective;
	if (depth <= 0 || vars.r <= 0)
		return (color_to_int(vars.local_color));
	vars.vr = vector_reflect(vars.v, vars.n);
	params.o = vars.p;
	params.d = vars.vr;
	params.t_min = 0.001;
	params.t_max = INFINITY;
	reflected_color_int = trace_ray(t_scene, params, depth - 1);
	return (final_color(&vars, reflected_color_int));
}
