#include "minirt.h"

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
