#ifndef RTV1_H
# define RTV1_H

# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdbool.h>
# include "libft.h"

# ifdef __linux__
#  include <SDL2/SDL.h>
# elif __APPLE__
#  include "SDL2/SDL.h"
# endif

# define SQR(n)	(pow(n, 2))
# define MIN(t1, t2) = (t1 < t2) ? (t1) : (t2)
# define MAX(t1, t2) = (t1 > t2) ? (t1) : (t2);
# define EPSILON 0.00001

typedef double	t_vec4f __attribute__((vector_size(sizeof(double) * 4)));
typedef int		t_vec4i __attribute__((vector_size(sizeof(int) * 4)));

typedef double	t_vec2f __attribute__((vector_size(sizeof(double) * 2)));
typedef int		t_vec2i __attribute__((vector_size(sizeof(int) * 2)));

# define TITLE	"rtv1"
# define W		640
# define H		480
# define OBJ	5

typedef struct	s_sphere
{
	t_vec4f		*center;
	float		radius;
	SDL_Color	*color;
}				t_sphere;

typedef struct	s_triang
{
	t_vec4f		*a;
	t_vec4f		*b;
	t_vec4f		*c;
	SDL_Color	*color;
	double		dist;
}				t_triang;

typedef struct	s_obj
{
	void		*data;
	bool		(*intersects)(void *data, t_vec4f ray_start,
			t_vec4f ray, t_vec4f intersect);
	SDL_Color	*(*get_color)(void *data, t_vec4f intersect);
	t_vec4f		(*normal_vec)(void *data, t_vec4f intersect);
	void		(*cleanup)(void *data);
}				t_obj;

typedef struct	s_main
{
	SDL_Window	*window;
	t_vec4f		*camera;
	t_vec4f		*ray;
	t_obj		*objects[OBJ];
}				t_main;

/*
**	sphere.c
*/
t_obj			*new_sphere(t_vec4f *center, float radius, SDL_Color *color);
SDL_Color		*sphere_color(void *data, t_vec4f intersect);
t_vec4f			sphere_normalvec(void *data, t_vec4f intersect);
void			sphere_cleanup(void *data);
bool			sphere_intersect(void *data, t_vec4f ray_start, t_vec4f ray,
				t_vec4f *intersect);
/*
**	triangle.c
*/
t_obj			*new_triangle(t_vec4f *a, t_vec4f *b, t_vec4f *c,
			SDL_Color *color);
SDL_Color		*triangle_color(void *data, t_vec4f intersect);
t_vec4f			triangle_normalvec(void *data, t_vec4f intersect);
void			triangle_cleanup(void *data);
bool			triangle_intersect(void *data, t_vec4f ray_start, t_vec4f ray,
				t_vec4f *intersect);
/*
**	vec4f.c
*/
t_vec4f			get_vec4f(t_vec4f p0, t_vec4f p1);
void			vec4f_normalize(t_vec4f *vec);
float			vec4f_length(t_vec4f vec);
double			vec4f_dotprod(t_vec4f a, t_vec4f b);
/*
**	render.c
*/
void			render(t_main *m);
#endif