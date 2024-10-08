#ifndef MINIRT_H
# define MINIRT_H

# include <mlx.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <X11/keysym.h>
# include <X11/X.h>
# include <X11/Xlib.h>
# include "./libft/libft.h"

# ifndef M_PI
#  define M_PI 3.14159265358979323846
# endif

# define WIDTH 800
# define HEIGHT 600
# define BACKGROUND_COLOR 0x000000 // Black background

// from: p = O + t(V - O), taking t only from the scene to infinity
# define T_MIN 1.0

// Structs for vectors, spheres, lights, and the scene

typedef enum s_shape_type
{
	SHAPE_NONE,
	SHAPE_SPHERE,
	SHAPE_CYLINDER,
	SHAPE_PLANE
}	t_shape_type;

typedef struct s_color
{
	double	red;
	double	green;
	double	blue;
}	t_color;

typedef struct s_vector
{
	double	x;
	double	y;
	double	z;
}	t_vector;

typedef struct s_light
{
	int			type; // 0 for ambient, 1 for point, 2 for directional - no directional
	double		intensity;
	t_vector	position;
	t_vector	direction;
	int			red;
	int			green;
	int			blue;
}	t_light;

typedef struct s_base_shape
{
	int		red;
	int		green;
	int		blue;
	int		specular; //when low 0 or 1 the picture is very bad
	double	reflective;
}	t_base_shape; //to handle shades and reflections regardless of shape

typedef struct s_sphere
{
	t_vector		center;
	double			radius;
	t_base_shape	base;
	t_shape_type	type;
}	t_sphere;

typedef struct s_cylinder
{
	t_vector		center;
	double			radius;
	double			height;
	t_vector		axis;
	t_base_shape	base;
	t_shape_type	type;
	t_vector		cap_center;
	t_vector		cap_normal;
}	t_cylinder;

typedef struct s_plane
{
	t_vector		point;
	t_vector		normal;
	t_base_shape	base;
	t_shape_type	type;
	double			square_size;
}	t_plane;

typedef struct s_camera
{
	t_vector	position;// Camera position (x, y, z)
	t_vector	orientation;// Camera direction (normalized t_vector)
	double		fov;// Field of view in degrees
	double		viewport_size;
	double		projection_plane_d;
}	t_camera;

typedef struct s_scene
{
	t_sphere	*spheres;
	int			sphere_count;
	t_cylinder	*cylinders;
	int			cylinder_count;
	t_plane		*planes;
	int			plane_count;
	t_light		*lights;
	int			light_count;
	t_camera	camera;
	int			camera_count;
	int			success;
}	t_scene;

typedef struct s_intersect_result
{
	t_shape_type	type;
	void			*object;
	double			t; //distance to intersection
	int				surface; // 0 for side, 1 for bottom cap, 2 for top cap
}	t_intersect_result;

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
	t_scene	*scene;
}	t_canvas;

typedef struct s_ray_params
{
	t_vector	o;
	t_vector	d;
	double		t_min;
	double		t_max;
}	t_ray_params;

typedef struct s_trace
{
	t_vector		p;
	t_vector		n;
	t_vector		v;
	t_vector		vr;
	t_color			local_color;
	t_color			lighting;
	t_color			reflected_color;
	t_color			final_color;
	t_base_shape	*shape;
	double			r;
}	t_trace;

typedef struct s_intersection
{
	t_vector	co;
	double		a;
	double		b;
	double		c;
	double		discriminant;
}	t_intersection;

//For quadratic equations
typedef struct s_quadratic
{
	double	a;
	double	b;
	double	c;
}	t_quadratic;

/* typedef enum {
	VECTOR_SUB,
	VECTOR_ADD,
	VECTOR_SCALE,
	VECTOR_NORM,
	VECTOR_REFLECT
} vector_operation; */

//vector_operations.c
t_vector			vector_sub(t_vector v1, t_vector v2);
t_vector			vector_add(t_vector v1, t_vector v2);
t_vector			vector_scale(t_vector v, double scalar);
double				vector_dot(t_vector v1, t_vector v2);
double				vector_length(t_vector v);
t_vector			vector_normalize(t_vector v);
t_vector			vector_reflect(t_vector vr, t_vector n);
t_vector			compute_base(t_cylinder *cyl);
t_vector			compute_top(t_cylinder *cyl);

//init.c
void				init_canvas(t_canvas *canvas);
t_canvas			*init_mlx(void);
t_canvas			*initialize_matrix(void);
int					initialize_graphics(t_canvas *canvas);
int					initialize_image(t_canvas *canvas);

//render.c
void				render(t_canvas *app, t_scene *t_scene, t_camera *camera);
t_scene				create_scene(void);//temporary

//ray_trace_1.c
int					trace_ray(t_scene *scene, t_ray_params params, int depth);
t_vector			vector_init(double x, double y, double z);
t_color				color_to_double(int color_int);
t_base_shape		*get_base_shape(t_intersect_result result);
t_color				get_plane_color(t_vector p, t_plane *pl);
int					trace_ray(t_scene *scene, t_ray_params params, int depth);

//ray_trace_2.c
t_vector			cyl_normal(t_vector p, t_cylinder *cyl, int surface);
t_vector			compute_normal(t_intersect_result result, t_vector p);
int					color_to_int(t_color local_color);

//intersection.c
t_intersect_result	closest_intersection(t_scene *scene, t_ray_params params);
int					update_result(t_intersect_result *result, double t,
						void *object, t_ray_params params);
void				update_cyl_result(t_intersect_result *result, int surface);
void				handle_side_intersect(t_ray_params params, t_cylinder *cyl,
						t_intersect_result *result);
int					cross_ray_cyl(t_ray_params params, t_cylinder *cyl,
						t_intersect_result *result);
int					cross_ray_plane(t_ray_params params, t_plane *pl, double *t);

//utils.c
void				put_pixel(t_canvas *app, int x, int y, int t_color);
void				check_limit_double(double *value, double limit);
void				check_limit_int(int *value, int limit);

//camera.c
void				set_camera(t_scene *scene);

//light_computation.c
t_color				compute_lighting(t_scene *scene, t_trace vars);
int					is_in_shadow(t_scene *scene, t_trace vars,
						t_vector L, double t_max);
void				diffuse_light(t_color *result, t_light light,
						t_trace vars, t_vector L);
void				specular_reflection(t_color *result, t_light light,
						t_trace vars, t_vector L);
t_color				color_init(double red, double green, double blue);
t_color				compute_lighting(t_scene *scene, t_trace vars);

//light_computation_utils.c
void				add_light(t_color *result, t_light light, double intensity);
void				ambient_light(t_color *result, t_light light);
t_vector			get_light_direction(t_light light, t_trace vars,
						double *t_max);

//free_functions.c
void				free_scene(t_scene *sc);
int					close_event(void *param);
int					key_handle(int keysym, void *param);
void				free_everything(t_scene scene, t_canvas *canvas, int fd);
void				free_close(t_canvas *canvas, int fd);

//parsing.c
t_scene				parse_rt(int fd, char *filename);
void				parse_line(char *line, t_scene *sc);
void				count_and_allocate(t_scene *sc, int fd);

//input_check_color.c
int					get_color(char *token, t_scene *sc);
int					parse_color_light(char *token, t_scene *sc,
						t_light *point_light);
int					parse_color_sphere(char *token, t_scene *sc,
						t_base_shape *base);
int					parse_color_cylinder(char *token, t_scene *sc,
						t_base_shape *base);
int					parse_color_plane(char *token, t_scene *sc,
						t_base_shape *base);

//input_check_rest.c
double				get_intensity(char *token, t_scene *sc);
double				get_position(char *token, t_scene *sc);
double				get_fov(char *token, t_scene *sc);
double				get_value(char *token, t_scene *sc);

//parsing_objects.c
void				parse_cylinder(char *line, t_scene *sc);
void				parse_plane(char *line, t_scene *sc);
void				parse_sphere(char *line, t_scene *sc);
void				parse_camera(char *line, t_scene *sc);

//parsing_lights.c
void				parse_ambient(char *line, t_scene *sc);
void				parse_light(char *line, t_scene *sc);
void				count_lights(char *line, t_scene *sc,
						int *a_light, int *p_light);

//parsing_utils.c
void				reset_count(t_scene *sc);
void				count_objects(int fd, t_scene *sc);
void				initiate_count(t_scene *sc);

#endif