#ifndef RTV1_H
# define RTV1_H

# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdbool.h>
# include <complex.h> 
# include "libft.h"

# ifdef __linux__
#  include <SDL2/SDL.h>
# elif __APPLE__
#  include "SDL2/SDL.h"
# endif

# define SQ(n)			(pow(n, 2))
# define CB(n)			(pow(n, 3))
# define QR(n)			(pow(n, 4))
# define MIN(t1, t2)	(t1 < t2) ? (t1) : (t2)
# define MAX(t1, t2)	(t1 > t2) ? (t1) : (t2)
# define EPSILON		(1e-6)
# define INF			(2147483647)
# define ALBEDO			(0.26s)

# define BGCOLOR	((SDL_Color){ 12, 12, 12, 255 })
# define FOCUS	(0.7)
# define TITLE	"rtv1"
# define W		1280
# define H		720
# define ASPECT	(W / (double)H)

typedef enum	e_figures
{
	TRIANGLE = 0,
	PLANE,
	SPHERE,
	CYLINDER,
	CONE,
	LIGHT_SOURCE,
	TORUS
}				t_figures;

typedef double	t_vec3f __attribute__((vector_size(sizeof(double)*3)));

typedef struct	s_matrix
{
	t_vec3f		m[3];
	double		cx;
	double		cy;
	double		cz;
	double		sx;
	double		sy;
	double		sz;
	double		sxsz;
	double		cxsz;
	double		cxcz;
	double		sxcz;
}				t_matrix;

typedef struct	s_light
{
	t_vec3f		*pos;
	double		brightness;
	SDL_Color	color;
}				t_light;

typedef struct	s_plane
{
	t_vec3f		*pos;
	t_vec3f		normal;
	SDL_Color	color;
}				t_plane;

typedef struct	s_sphere
{
	t_vec3f		*pos;
	double		radius;
	SDL_Color	color;
}				t_sphere;

typedef struct	s_cylind
{
	t_vec3f		*pos;
	t_vec3f		dir;
	double		radius;
	SDL_Color	color;
}				t_cylind;

typedef struct	s_cone
{
	t_vec3f		*pos;
	t_vec3f		dir;
	double		angle;
	SDL_Color	color;
}				t_cone;

typedef struct	s_triang
{
	t_vec3f		*a;
	t_vec3f		*b;
	t_vec3f		*c;
	t_vec3f		*normal;
	SDL_Color	color;
	double		dist;
}				t_triang;

typedef struct	s_torus
{
	t_vec3f		*pos;
	t_vec3f		dir;
	double		radius;
	double		tube_radius;
	SDL_Color	color;
}				t_torus;


typedef struct	s_obj
{
	int			type;
	void		*data;
	bool		(*intersects)(void *data, t_vec3f ray_start,
			t_vec3f ray, t_vec3f *intersect);
	SDL_Color	(*get_color)(void *data, t_vec3f intersect);
	t_vec3f		(*normal_vec)(void *data, t_vec3f intersect);
	void		(*cleanup)(void *data);
}				t_obj;

typedef struct	s_cam
{
	t_vec3f		*pos;
	t_vec3f		ray;
	t_vec3f		angle;
	t_matrix	rot_mtx;
	double		focus;
}				t_cam;

typedef struct	s_main
{
	SDL_Window	*window;
	SDL_Surface	*screen;
	bool		running;
	int			bpp;
	t_vec3f		p;
	t_cam		*cam;
	t_obj		**objects;
	int			obj_num;
}				t_main;

typedef t_obj	*(*t_of)(t_vec3f *, t_vec3f, double, SDL_Color);

t_obj			**parse_scene(t_main *m, char *path);

t_obj			*new_cylinder(t_vec3f *center, t_vec3f dir, double radius, SDL_Color color);
t_obj			*new_cone(t_vec3f *center, t_vec3f dir, double radius, SDL_Color color);

SDL_Color		lights_color(void *data, t_vec3f intersect);
void			lights_cleanup(void *data);
t_obj			*new_light(t_vec3f *location, t_vec3f dir, double brightness, SDL_Color color);

t_obj				*new_plane(t_vec3f *center, t_vec3f dir, double radius,
							SDL_Color color);
bool				plane_intersect(void *data, t_vec3f eye, t_vec3f rdir,
										t_vec3f *intersect);
t_vec3f				plane_normalvec(void *data, t_vec3f intersect);
SDL_Color			plane_color(void *data, t_vec3f intersect);
void				plane_cleanup(void *data);

/*
**	sphere.c
*/
t_obj			*new_sphere(t_vec3f *center, t_vec3f dir, double radius, SDL_Color color);
SDL_Color		sphere_color(void *data, t_vec3f intersect);
t_vec3f			sphere_normalvec(void *data, t_vec3f intersect);
void			sphere_cleanup(void *data);
bool			sphere_intersect(void *data, t_vec3f ray_start, t_vec3f ray,
				t_vec3f *intersect);
/*
**	triangle.c
*/
t_obj			*new_triangle(t_vec3f *a, t_vec3f *b, t_vec3f *c,
			SDL_Color color);
SDL_Color		triangle_color(void *data, t_vec3f intersect);
t_vec3f			triangle_normalvec(void *data, t_vec3f intersect);
void			triangle_cleanup(void *data);
bool			triangle_intersect(void *data, t_vec3f ray_start, t_vec3f ray,
				t_vec3f *intersect);

t_obj			*new_torus(t_vec3f *center, t_vec3f dir, double tor_rad,
							double tube_rad, SDL_Color color);
bool			torus_intersect(void *data, t_vec3f o, t_vec3f dir,
				t_vec3f *intersect);
SDL_Color		torus_color(void *data, t_vec3f intersect);
t_vec3f			torus_normalvec(void *data, t_vec3f intersect);
void			torus_cleanup(void *data);


/*
**	vec3f.c
*/
double		vec3f_dot(t_vec3f a, t_vec3f b);
double		vec3f_squared(t_vec3f v);
double		vec3f_length(t_vec3f vec);
t_vec3f		vec3f_cross(t_vec3f a, t_vec3f b);
t_vec3f		vec3f_multsc(t_vec3f v, double scalar);
void		vec3f_normalize(t_vec3f *vec);
t_vec3f		vec3f_get(double a, double b, double c);
/*
**	render.c
*/
void				matrix_apply(t_vec3f *vec, t_matrix m);
t_matrix			init_matrix(t_vec3f angle);
void			render(t_main *m);
void			set_pixel(t_main *m, int x, int y, unsigned int p);

unsigned int	trace(t_main *m, t_vec3f ray);


#endif