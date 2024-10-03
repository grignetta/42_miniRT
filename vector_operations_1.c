#include "minirt.h"

//don't forget to take into account: any number more than 255, will be still 255, less than 0 will be still 0!!
t_vector	vector_normalize(t_vector v)
{
	double	len;

	len = vector_length(v);
	return ((t_vector){v.x / len, v.y / len, v.z / len});
}

t_vector	vector_reflect(t_vector vr, t_vector n)
{
	return (vector_sub(vector_scale(n, 2 * vector_dot(n, vr)), vr));
}

t_vector	compute_base(t_cylinder *cyl)
{
	return (vector_sub(cyl->center,
			vector_scale(cyl->axis, cyl->height / 2.0)));
}

t_vector	compute_top(t_cylinder *cyl)
{
	return (vector_add(cyl->center,
			vector_scale(cyl->axis, cyl->height / 2.0)));
}

/* t_vector v_operation(t_vector v1, t_vector v2, double scalar, vector_operation op)
{
	if (op == VECTOR_SUB)
		return ((t_vector){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z});
	if (op == VECTOR_ADD)
		return ((t_vector){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z});
	if (op == VECTOR_SCALE)
		return ((t_vector){v1.x * scalar, v1.y * scalar, v1.z * scalar});
	if (op == VECTOR_NORM)
		return ((t_vector){v1.x / vector_length(v1), v1.y / vector_length(v1), v1.z /vector_length(v1)});
	if (op == VECTOR_REFLECT)
		return (v_operation(v_operation(v1, (t_vector){0.0, 0.0, 0.0}, 2 * vector_dot(v1, v2),    VECTOR_SCALE), v2, 0, VECTOR_SUB));
} */
