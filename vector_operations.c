#include "minirt.h"

// Helper functions for vector operations
vector vector_sub(vector v1, vector v2)
{
    return (vector){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

vector vector_add(vector v1, vector v2)
{
    return (vector){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

vector vector_scale(vector v, double scalar)
{
    return (vector){v.x * scalar, v.y * scalar, v.z * scalar};
}

double vector_dot(vector v1, vector v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

double vector_length(vector v)
{
    return sqrt(vector_dot(v, v));
}

vector vector_normalize(vector v)
{
    double len = vector_length(v);
    return (vector){v.x / len, v.y / len, v.z / len};
}

vector vector_reflect(vector R, vector N)
{
    return vector_sub(vector_scale(N, 2 * vector_dot(N, R)), R);
}
