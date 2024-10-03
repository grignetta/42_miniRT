#include "minirt.h"

t_vector	cyl_normal(t_vector p, t_cylinder *cyl, int surface)
{
	t_vector	n;
	t_vector	cp;
	t_vector	n_perp;

	n = (t_vector){0, 0, 0};
	// double y_bottom = cyl->center.y;
	// double y_top = cyl->center.y + cyl->height;
	// double epsilon = 1e-6; // Small value to handle floating-point precision
	// if (fabs(p.y - y_bottom) < epsilon) {
	//     // Bottom cap
	//     n = vector_init(0, -1, 0); // Normal pointing downwards
	// } else if (fabs(p.y - y_top) < epsilon) {
	//     // Top cap
	//     n = vector_init(0, 1, 0); // Normal pointing upwards
	// } else {
	//     // Side surface
	//     vector temp = vector_sub(p, cyl->center);
	//     temp.y = 0; // Project onto the XZ plane
	//     n = vector_normalize(temp);
	// }
	if (surface == 0) // Side surface
	{
		// Compute the vector from the cylinder center to point p, projected onto the plane perpendicular to the axis
		cp = vector_sub(p, cyl->center);
		n_perp = vector_sub(cp, vector_scale(cyl->axis,
				vector_dot(cp, cyl->axis)));
		n = vector_normalize(n_perp);
	}
	else if (surface == 1) // Bottom cap
	{
		n = vector_scale(cyl->axis, -1); // Normal pointing outward
	}
	else if (surface == 2) // Top cap
	{
		n = cyl->axis; // Normal pointing outward
	}
	return (n);
}

//int trace_ray(scene *scene, vector o, t_vector d, double t_min, double t_max, int depth)
t_vector	compute_normal(t_intersect_result result, t_vector p)
{
	//cylinder *c;
	t_vector	n;

	n = (t_vector){0, 0, 0};
	if (result.type == SHAPE_SPHERE)
		n = vector_normalize(vector_sub(p, ((t_sphere *)result.object)->center));
	else if (result.type == SHAPE_CYLINDER)
	{
		// c = (cylinder *)result.object;
		// if (result.surface == 0)
		//     // Side surface
		//     n = cyl_normal(p, c);
		// else if (result.surface == 1)
		//     // Bottom cap
		//     n = vector_init(0, -1, 0);
		// else if (result.surface == 2)
		//     // Top cap
		//     n = vector_init(0, 1, 0);
		n = cyl_normal(p, (t_cylinder *)result.object, result.surface);
	}
	else if (result.type == SHAPE_PLANE)
		n = ((t_plane *)result.object)->normal;
	return (n);
}

int	color_to_int(t_color local_color)
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
