#include "minirt.h"

// Function to compute the normal at a point on the cylinder
/* vector cyl_normal(vector p, cylinder *cyl)
{
	vector base = compute_base(cyl);
	vector top = compute_top(cyl);
	vector pa = vector_sub(p, base);  // pa = p - base
	vector ba = vector_sub(top, base);  // ba = top - base
	double baba = vector_dot(ba, ba);
	double h = vector_dot(pa, ba) / baba;
	vector normal = vector_sub(pa, vector_scale(ba, h));
	return vector_normalize(vector_scale(normal, 1.0 / cyl->radius));
} */

color	color_to_double(int color_int)
{
	color	result;

	result.red = ((color_int >> 16) & 0xFF) / 255.0;
	result.green = ((color_int >> 8) & 0xFF) / 255.0;
	result.blue = (color_int & 0xFF) / 255.0;
	return (result);
}

base_shape	*get_base_shape(intersect_result result)
{
	base_shape	*shape;

	shape = (base_shape *)result.object;
	if (result.type == SHAPE_SPHERE)
		shape = &((sphere *)result.object)->base;
	else if (result.type == SHAPE_CYLINDER)
		shape = &((cylinder *)result.object)->base;
	else if (result.type == SHAPE_PLANE)
		shape = &((plane *)result.object)->base;
	return (shape);
}

color	get_plane_color(vector P, plane *pl)
{
	color	base_color;
	color	inverse_color;
	int		sum;
	int		x_square;
	int		z_square;

	base_color = (color){
		.red = pl->base.red / 255.0,
		.green = pl->base.green / 255.0,
		.blue = pl->base.blue / 255.0};
	if (pl->square_size == 0)
		return (base_color);
	x_square = (int)floor(P.x / pl->square_size);
	z_square = (int)floor(P.z / pl->square_size);
	sum = x_square + z_square;
	inverse_color = (color){
		.red = 1.0 - base_color.red,
		.green = 1.0 - base_color.green,
		.blue = 1.0 - base_color.blue
	};
	if (sum % 2 == 0)
		return (base_color);
	else
		return (inverse_color);
}

int	trace_ray(scene *scene, ray_params params, int depth)
{
	trace				vars;
	intersect_result	result;
	int					reflected_color_int;
	plane				*pl;
	color				checker_color;

	result = closest_intersection(scene, params);
	if (result.t == params.t_max)
		return (BACKGROUND_COLOR); // Background color
	// Compute local color at the intersection point
	vars.P = vector_add(params.O, vector_scale(params.D, result.t)); // Intersection point
	vars.N = compute_normal(result, vars.P);
	vars.V = vector_scale(params.D, -1); // View direction
	//vars.V = vector_normalize(vars.V);//maybe not necessary
	// Determine the shape and compute lighting
	vars.shape = get_base_shape(result);
	// Compute lighting based on the specular value of the shape
	vars.lighting = compute_lighting(scene, vars);//vars.P, vars.N, vars.V, vars.shape->specular);
	// **Add Checkerboard Pattern for Planes**
	if (result.type == SHAPE_PLANE)
	{
		pl = (plane *)result.object;
		checker_color = get_plane_color(vars.P, pl);
		vars.local_color.red = checker_color.red * vars.lighting.red;
		vars.local_color.green = checker_color.green * vars.lighting.green;
		vars.local_color.blue = checker_color.blue * vars.lighting.blue;
	}
	else
	{
		// For other shapes, use their base color
		vars.local_color.red = (vars.shape->red / 255.0)
			* vars.lighting.red;
		vars.local_color.green = (vars.shape->green / 255.0)
			* vars.lighting.green;
		vars.local_color.blue = (vars.shape->blue / 255.0)
			* vars.lighting.blue;
	}
	//Calculate local color using the shape's color attributes
	// vars.local_color.red = (vars.shape->red / 255.0) * vars.lighting.red;
	// vars.local_color.green = (vars.shape->green / 255.0) * vars.lighting.green;
	// vars.local_color.blue = (vars.shape->blue / 255.0) * vars.lighting.blue;
	vars.r = vars.shape->reflective;
	if (depth <= 0 || vars.r <= 0)
		return (color_to_int(vars.local_color));
	// Compute reflection vector
	vars.R = vector_reflect(vars.V, vars.N);
	params.O = vars.P;
	params.D = vars.R;
	params.t_min = 0.001;
	params.t_max = INFINITY; //was declared in the beginning
	// Trace the reflected ray
	reflected_color_int = trace_ray(scene, params, depth - 1);
	vars.reflected_color = color_to_double(reflected_color_int);
	// Combine local color and reflected color based on reflectivity
	vars.final_color.red = vars.local_color.red
		* (1 - vars.r) + vars.reflected_color.red * vars.r;
	vars.final_color.green = vars.local_color.green
		* (1 - vars.r) + vars.reflected_color.green * vars.r;
	vars.final_color.blue = vars.local_color.blue
		* (1 - vars.r) + vars.reflected_color.blue * vars.r;
	return (color_to_int(vars.final_color));
}
