#include "minirt.h"

vector	cyl_normal(vector P, cylinder *cyl, int surface)
{
	vector	N;
	vector	CP;
	vector	N_perp;

	N = (vector){0, 0, 0};
	// double y_bottom = cyl->center.y;
	// double y_top = cyl->center.y + cyl->height;
	// double epsilon = 1e-6; // Small value to handle floating-point precision
	// if (fabs(P.y - y_bottom) < epsilon) {
	//     // Bottom cap
	//     N = vector_init(0, -1, 0); // Normal pointing downwards
	// } else if (fabs(P.y - y_top) < epsilon) {
	//     // Top cap
	//     N = vector_init(0, 1, 0); // Normal pointing upwards
	// } else {
	//     // Side surface
	//     vector temp = vector_sub(P, cyl->center);
	//     temp.y = 0; // Project onto the XZ plane
	//     N = vector_normalize(temp);
	// }
	if (surface == 0) // Side surface
	{
		// Compute the vector from the cylinder center to point P, projected onto the plane perpendicular to the axis
		CP = vector_sub(P, cyl->center);
		N_perp = vector_sub(CP, vector_scale(cyl->axis,
				vector_dot(CP, cyl->axis)));
		N = vector_normalize(N_perp);
	}
	else if (surface == 1) // Bottom cap
	{
		N = vector_scale(cyl->axis, -1); // Normal pointing outward
	}
	else if (surface == 2) // Top cap
	{
		N = cyl->axis; // Normal pointing outward
	}
	return (N);
}

//int trace_ray(scene *scene, vector O, vector D, double t_min, double t_max, int depth)
vector	compute_normal(intersect_result result, vector P)
{
	//cylinder *c;
	vector	N;

	N = (vector){0, 0, 0};
	if (result.type == SHAPE_SPHERE)
		N = vector_normalize(vector_sub(P, ((sphere *)result.object)->center));
	else if (result.type == SHAPE_CYLINDER)
	{
		// c = (cylinder *)result.object;
		// if (result.surface == 0)
		//     // Side surface
		//     N = cyl_normal(P, c);
		// else if (result.surface == 1)
		//     // Bottom cap
		//     N = vector_init(0, -1, 0);
		// else if (result.surface == 2)
		//     // Top cap
		//     N = vector_init(0, 1, 0);
		N = cyl_normal(P, (cylinder *)result.object, result.surface);
	}
	else if (result.type == SHAPE_PLANE)
		N = ((plane *)result.object)->normal;
	return (N);
}

int	color_to_int(color local_color)
{
	int	final_red;
	int	final_green;
	int	final_blue;

	final_red = (int)(local_color.red * 255);
	final_green = (int)(local_color.green * 255);
	final_blue = (int)(local_color.blue * 255);
	check_limit_int(&final_red, 255);
	check_limit_int(&final_green, 255);
	check_limit_int(&final_blue, 255);
	return ((final_red << 16) | (final_green << 8) | final_blue);
}
