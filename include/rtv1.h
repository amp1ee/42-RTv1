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

# define BGCOLOR	((t_v3){ 12, 12, 12 })
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

typedef double	t_v3 __attribute__((vector_size(sizeof(double)*3)));

typedef struct	s_matrix
{
	t_v3		m[3];
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
	t_v3		*pos;
	double		brightness;
	SDL_Color	color;
}				t_light;

typedef struct	s_plane
{
	t_v3		*pos;
	t_v3		normal;
	SDL_Color	color;
}				t_plane;

typedef struct	s_sphere
{
	t_v3		*pos;
	double		radius;
	SDL_Color	color;
}				t_sphere;

typedef struct	s_cylind
{
	t_v3		*pos;
	t_v3		dir;
	double		radius;
	SDL_Color	color;
}				t_cylind;

typedef struct	s_cone
{
	t_v3		*pos;
	t_v3		dir;
	double		angle;
	SDL_Color	color;
}				t_cone;

typedef struct	s_triang
{
	t_v3		*a;
	t_v3		*b;
	t_v3		*c;
	t_v3		*normal;
	SDL_Color	color;
	double		dist;
}				t_triang;

typedef struct	s_torus
{
	t_v3		*pos;
	t_v3		dir;
	double		radius;
	double		tube_radius;
	SDL_Color	color;
}				t_torus;

typedef struct	s_figure
{
	double		a;
	double		b;
	double		c;
	double		d;
	double		t;
	double		t1;
	double		t2;
	double		droot;
	t_v3		k;
	t_v3		l;
	t_v3		m;
	t_v3		n;
	t_v3		dir;
}				t_figure;

typedef struct	s_obj
{
	int			type;
	void		*data;
	bool		(*intersects)(void *data, t_v3 ray_start,
			t_v3 ray, t_v3 *intersect);
	SDL_Color	(*get_color)(void *data, t_v3 intersect);
	t_v3		(*normal_vec)(void *data, t_v3 intersect);
	void		(*cleanup)(void *data);
}				t_obj;

typedef struct	s_cam
{
	t_v3		*pos;
	t_v3		ray;
	t_v3		angle;
	t_matrix	rot_mtx;
	double		focus;
}				t_cam;

typedef struct	s_main
{
	SDL_Window	*window;
	SDL_Surface	*screen;
	t_cam		*cam;
	t_obj		**objects;
	t_v3		rdir;
	t_v3		p;
	t_v3		refl_point;
	bool		running;
	int			obj_num;
	int			recur_depth;
}				t_main;

typedef struct	s_shedlight
{
	t_light		*light;
	t_v3		light_dir;
	t_v3		spot;
	t_v3		diffuse_light;
	t_v3		specular_light;
	double		diffuse_k;
	double		atten;
	double		dist;
	int			j;
}				t_shedlight;

typedef struct	s_trace
{
	t_v3		color;
	SDL_Color	c;
	t_obj		*o;
	t_v3		p;
	t_v3		n;
	t_v3		refl;
	double		t;
	double		k;
}				t_trace;

typedef t_obj	*(*t_of)(t_v3 *, t_v3, double, SDL_Color);

t_obj			**parse_scene(t_main *m, char *path);

t_obj			*new_cylinder(t_v3 *center, t_v3 dir, double radius, SDL_Color color);
t_obj			*new_cone(t_v3 *center, t_v3 dir, double radius, SDL_Color color);

SDL_Color		lights_color(void *data, t_v3 intersect);
void			lights_cleanup(void *data);
t_obj			*new_light(t_v3 *location, t_v3 dir, double brightness, SDL_Color color);

t_obj				*new_plane(t_v3 *center, t_v3 dir, double radius,
							SDL_Color color);
bool				plane_intersect(void *data, t_v3 eye, t_v3 rdir,
										t_v3 *intersect);
t_v3				plane_normalvec(void *data, t_v3 intersect);
SDL_Color			plane_color(void *data, t_v3 intersect);
void				plane_cleanup(void *data);

/*
**	sphere.c
*/
t_obj			*new_sphere(t_v3 *center, t_v3 dir, double radius, SDL_Color color);
SDL_Color		sphere_color(void *data, t_v3 intersect);
t_v3			sphere_normalvec(void *data, t_v3 intersect);
void			sphere_cleanup(void *data);
bool			sphere_intersect(void *data, t_v3 ray_start, t_v3 ray,
				t_v3 *intersect);
/*
**	triangle.c
*/
t_obj			*new_triangle(t_v3 *a, t_v3 *b, t_v3 *c,
			SDL_Color color);
SDL_Color		triangle_color(void *data, t_v3 intersect);
t_v3			triangle_normalvec(void *data, t_v3 intersect);
void			triangle_cleanup(void *data);
bool			triangle_intersect(void *data, t_v3 ray_start, t_v3 ray,
				t_v3 *intersect);

t_obj			*new_torus(t_v3 *center, t_v3 dir, double tor_rad,
							double tube_rad, SDL_Color color);
bool			torus_intersect(void *data, t_v3 o, t_v3 dir,
				t_v3 *intersect);
SDL_Color		torus_color(void *data, t_v3 intersect);
t_v3			torus_normalvec(void *data, t_v3 intersect);
void			torus_cleanup(void *data);


/*
**	v3.c
*/
double		v3_dot(t_v3 a, t_v3 b);
double		v3_squared(t_v3 v);
double		v3_length(t_v3 vec);
t_v3		v3_cross(t_v3 a, t_v3 b);
t_v3		v3_multsc(t_v3 v, double scalar);
void		v3_normalize(t_v3 *vec);
t_v3		v3_get(double a, double b, double c);
t_v3		v3_reflected(t_v3 vec, t_v3 n);
/*
**	render.c
*/
void				matrix_apply(t_v3 *vec, t_matrix m);
t_matrix			init_matrix(t_v3 angle);
void			render(t_main *m);
void			set_pixel(t_main *m, int x, int y, t_v3 color);

t_v3			trace(t_main *m, t_v3 ray, int depth);


#endif
