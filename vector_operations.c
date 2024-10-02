#include "minirt.h"

//don't forget to take into account: any number more than 255, will be still 255, less than 0 will be still 0!!

// Helper functions for vector operations
vector	vector_sub(vector v1, vector v2)
{
	return ((vector){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z});
}

vector	vector_add(vector v1, vector v2)
{
	return ((vector){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z});
}

vector	vector_scale(vector v, double scalar)//ex: color intensity
{
	return ((vector){v.x * scalar, v.y * scalar, v.z * scalar});
}

double	vector_dot(vector v1, vector v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

double	vector_length(vector v)
{
	return (sqrt(vector_dot(v, v)));
}

vector	vector_normalize(vector v)
{
	double len = vector_length(v);
	return ((vector){v.x / len, v.y / len, v.z / len});
}

vector	vector_reflect(vector R, vector N)
{
	return (vector_sub(vector_scale(N, 2 * vector_dot(N, R)), R));
}

vector	compute_base(cylinder *cyl)
{
	return (vector_sub(cyl->center, vector_scale(cyl->axis, cyl->height / 2.0)));
}

vector	compute_top(cylinder *cyl)
{
	return (vector_add(cyl->center, vector_scale(cyl->axis, cyl->height / 2.0)));
}

/* vector v_operation(vector v1, vector v2, double scalar, vector_operation op)
{
	if (op == VECTOR_SUB)
		return ((vector){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z});
	if (op == VECTOR_ADD)
		return ((vector){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z});
	if (op == VECTOR_SCALE)
		return ((vector){v1.x * scalar, v1.y * scalar, v1.z * scalar});
	if (op == VECTOR_NORM)
		return ((vector){v1.x / vector_length(v1), v1.y / vector_length(v1), v1.z /vector_length(v1)});
	if (op == VECTOR_REFLECT)
		return (v_operation(v_operation(v1, (vector){0.0, 0.0, 0.0}, 2 * vector_dot(v1, v2),    VECTOR_SCALE), v2, 0, VECTOR_SUB));
} */
