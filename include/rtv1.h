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
# define EPSILON		(1e-12)
# define INF			(2147483647)
# define ALBEDO			(0.26f)

# define BGCOLOR	((SDL_Color){ 12, 12, 12, 255 })
# define FOCUS	320
# define TITLE	"rtv1"
# define W		640
# define H		480
# define ASPECT	(H / (double)W)
# define OBJ	2
# define LIGHT	1

typedef enum	e_figures
{
	TRIANGLE = 0,
	SPHERE, TORUS
}				t_figures;

typedef struct	s_vec3f
{
	double		x;
	double		y;
	double		z;
}				t_vec3f;

typedef struct	s_vec3i
{
	int			x;
	int			y;
	int			z;
}				t_vec3i;

typedef struct	s_light
{
	t_vec3f		*loc;
	double		brightness;
	SDL_Color	color;
}				t_light;

typedef struct	s_sphere
{
	t_vec3f		*center;
	double		radius;
	SDL_Color	color;
}				t_sphere;

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
	t_vec3f		*center;
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
	t_vec3f		*loc;
	double		xang;
	double		xcos;
	double		xsin;
	double		yang;
	double		ycos;
	double		ysin;
	double		zang;
	double		zcos;
	double		zsin;
	double		focus;
}				t_cam;

typedef struct	s_main
{
	SDL_Window	*window;
	SDL_Surface	*screen;
	bool		running;
	int			bpp;
	t_cam		*cam;
	t_light		*lights[LIGHT];
	t_vec3f		*ray;
	t_obj		*objects[OBJ];
}				t_main;

/*
**	sphere.c
*/
t_obj			*new_sphere(t_vec3f *center, double radius, SDL_Color color);
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
**	vec4f.c
*/
t_vec3f			get_vec3f(t_vec3f p0, t_vec3f p1);
void			vec3f_normalize(t_vec3f *vec);
double			vec3f_length(t_vec3f vec);
double			vec3f_dot(t_vec3f a, t_vec3f b);
t_vec3f			vec3f_cross(t_vec3f a, t_vec3f b);
/*
**	render.c
*/
void			render(t_main *m);
void			set_pixel(t_main *m, int x, int y, unsigned int p);

unsigned int	trace(t_main *m, t_vec3f ray);


#endif