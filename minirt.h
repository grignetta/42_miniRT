#ifndef MINIRT_H
# define MINIRT_H

# include <mlx.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include "./libft/libft.h"

#define WIDTH 800
#define HEIGHT 600
#define BACKGROUND_COLOR 0x000000 // Black background

// Structs for vectors, spheres, lights, and the scene
typedef struct
{
	double x, y, z;
} vector;

typedef struct
{
	vector center;
	double radius;
	int color;
	int specular;
	double reflective;
} sphere;

typedef struct
{
	int type; // 0 for ambient, 1 for point, 2 for directional
	double intensity;
	vector position;
	vector direction;
} Light;

typedef struct
{
	sphere *spheres;
	int sphere_count;
	Light *lights;
	int light_count;
} scene;

typedef struct s_canvas
{
	//int		width;
	//int		height;
	//int		**z_matrix;
	//int		zoom;
	//float	z_zoom;
	//int		color;
	//int		black;
	//int		color_delta;
	//double	angle;
	//int		isometric;
	//int		x_shift;
	//int		y_shift;
	//int		legend_offset;

	int		img_side;
	void	*mlx_ptr;
	void	*win_ptr;
	int		win_width;
	int		win_height;
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_canvas;

//Vector operations
vector vector_sub(vector v1, vector v2);
vector vector_add(vector v1, vector v2);
vector vector_scale(vector v, double scalar);
double vector_dot(vector v1, vector v2);
double vector_length(vector v);
vector vector_normalize(vector v);
vector vector_reflect(vector R, vector N);

//rest.c
int intersect_ray_sphere(vector O, vector D, sphere sphere, double *t1, double *t2);
sphere *closest_intersection(scene *scene, vector O, vector D, double t_min, double t_max, double *closest_t);
double compute_lighting(scene *scene, vector P, vector N, vector V, int specular);
int trace_ray(scene *scene, vector O, vector D, double t_min, double t_max);
void put_pixel(t_canvas *app, int x, int y, int color);
void render(t_canvas *app, scene *scene);
scene create_scene();


#endif