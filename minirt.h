#ifndef MINIRT_H
# define MINIRT_H

# include <mlx.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include "./libft/libft.h"

# ifndef M_PI
#  define M_PI 3.14159265358979323846
# endif

# define WIDTH 800
# define HEIGHT 600
# define BACKGROUND_COLOR 0x000000 // Black background

// from: P = O + t(V - O), taking t only from the scene to infinity
#define T_MIN 1.0

// Structs for vectors, spheres, lights, and the scene

typedef enum
{
    SHAPE_NONE,
    SHAPE_SPHERE,
    SHAPE_CYLINDER,
    SHAPE_PLANE
} shape_type;

typedef struct
{
	double x, y, z;
} vector;

typedef struct
{
	int type; // 0 for ambient, 1 for point, 2 for directional - no directional
	double intensity;
	vector position;
	vector direction;
	int red;
    int green;
    int blue;
} light;

typedef struct {
    int red;
    int green;
    int blue;
    int specular; //when low 0 or 1 the picture is very bad
    double reflective;
} base_shape; //to handle shades and reflections regardless of shape

typedef struct
{
	vector center;
	double radius;
	base_shape base;
	shape_type type;
} sphere;

typedef struct
{
    vector center;
    double radius;
    double height;
	vector axis;
	base_shape base;
	shape_type type;
} cylinder;

typedef struct
{
    vector point;
    vector normal;
	base_shape base;
	shape_type type;
} plane;

typedef struct
{
    vector position;       // Camera position (x, y, z)
    vector orientation;    // Camera direction (normalized vector)
    double fov;            // Field of view in degrees
    double viewport_size;
    double projection_plane_d;
} camera;

typedef struct
{
    sphere *spheres;
    int sphere_count;
    cylinder *cylinders;
    int cylinder_count;
    plane *planes;
    int plane_count;
    light *lights;
    int light_count;
    camera camera;
} scene;
typedef struct
{
    double red;
    double green;
    double blue;
} color;

typedef struct {
    shape_type type;
    void *object;
    double t; //distance to intersection
} intersection_result;

typedef struct s_canvas
{
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

typedef struct {
    vector O;
    vector D;
    double t_min;
    double t_max;
} ray_params;

typedef struct {
    vector P;
    vector N;
    vector V;
    vector R;
    color local_color;
    color lighting;
    color reflected_color;
    color final_color;
    base_shape *shape;
    double r;
} trace;

typedef struct
{
    vector CO;
    double a;
	double b;
	double c;
    double discriminant;
} intersection;

//Vector operations
vector vector_sub(vector v1, vector v2);
vector vector_add(vector v1, vector v2);
vector vector_scale(vector v, double scalar);
double vector_dot(vector v1, vector v2);
double vector_length(vector v);
vector vector_normalize(vector v);
vector vector_reflect(vector R, vector N);
vector compute_base(cylinder *cyl);
vector compute_top(cylinder *cyl);

//rest.c
intersection_result closest_intersection(scene *scene, ray_params params);
//double compute_lighting(scene *scene, vector P, vector N, vector V, int specular);
//color compute_lighting(scene *scene, vector P, vector N, vector V, int specular);
//int trace_ray(scene *scene, vector O, vector D, double t_min, double t_max, int depth);
int trace_ray(scene *scene, ray_params params, int depth);
void render(t_canvas *app, scene *scene, camera *camera);
scene create_scene();

//utils.c
void put_pixel(t_canvas *app, int x, int y, int color);
void check_limit_double(double *value, double limit);
void check_limit_int(int *value, int limit);

//camera.c
void set_camera(scene *scene);

//light_computation.c
color compute_lighting(scene *scene, trace vars);

//intersection.c
// int intersect_ray_sphere(vector O, vector D, sphere *sphere, double *t1, double *t2);
// int intersect_ray_cylinder(vector O, vector D, cylinder *cyl, double *t1, double *t2);
// int intersect_ray_plane(vector O, vector D, plane *pl, double *t);


#endif