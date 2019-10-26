/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oahieiev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/26 15:18:27 by oahieiev          #+#    #+#             */
/*   Updated: 2019/10/26 15:18:29 by oahieiev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTV1_H
# define RTV1_H

# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdbool.h>
# include <errno.h>
# include "libft.h"

# ifdef __linux__
#  include <SDL2/SDL.h>
# elif __APPLE__
#  include "SDL2/SDL.h"
# endif

# define SQ(n)			(pow(n, 2))
# define UI				((t_v3){1, 0, 0})
# define UJ				((t_v3){0, -1, 0})
# define UK				((t_v3){0, 0, 1})
# define MIN(t1, t2)	(t1 < t2) ? (t1) : (t2)
# define MAX(t1, t2)	(t1 > t2) ? (t1) : (t2)
# define EPSILON		(1e-4)
# define BGCOLOR		((t_v3){ 0, 0, 0 })
# define FOCUS			(0.7)
# define TITLE			"RTv1"
# define W				(1280)
# define H				(720)
# define ASPECT			(W / (double)H)

# define USAGE "Usage: ./RTv1 scene.scn [recursive depth]\n"

/*
**  P - plane
**  S - sphere
**  C - cylinder
**  c - cone
**  L - light source
*/
typedef enum	e_figures
{
	PLANE = 0,
	SPHERE,
	CYLINDER,
	CONE,
	LIGHT_SOURCE
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
	t_v3		pos;
	t_v3		color;
	double		brightness;
}				t_light;

typedef struct	s_plane
{
	t_v3		pos;
	t_v3		normal;
	t_v3		color;
}				t_plane;

typedef struct	s_sphere
{
	t_v3		pos;
	t_v3		color;
	double		radius;
}				t_sphere;

typedef struct	s_cylind
{
	t_v3		pos;
	t_v3		dir;
	t_v3		color;
	double		radius;
}				t_cylind;

typedef struct	s_cone
{
	t_v3		pos;
	t_v3		dir;
	t_v3		color;
	double		angle;
}				t_cone;

typedef struct	s_figure
{
	t_v3		k;
	t_v3		l;
	t_v3		m;
	t_v3		n;
	t_v3		dir;
	double		a;
	double		b;
	double		c;
	double		d;
	double		e;
	double		f;
	double		t;
	double		t1;
	double		t2;
	double		droot;
}				t_figure;

typedef struct	s_obj
{
	void		*data;
	bool		(*intersects)(void *data, t_v3 ray_start,
					t_v3 ray, t_v3 *intersect);
	t_v3		(*get_color)(void *data, t_v3 intersect);
	t_v3		(*normal_vec)(void *data, t_v3 intersect);
	void		(*cleanup)(void *data);
	int			type;
}				t_obj;

typedef struct	s_cam
{
	t_matrix	rot_mtx;
	t_v3		pos;
	t_v3		ray;
	t_v3		angle;
	double		focus;
}				t_cam;

typedef struct	s_main
{
	t_v3		rdir;
	t_v3		p;
	t_v3		refl_point;
	t_v3		start_pos;
	SDL_Window	*window;
	SDL_Surface	*screen;
	t_cam		*cam;
	t_list		*obj_list;
	t_obj		**objects;
	int			obj_num;
	int			recur_depth;
	bool		running;
}				t_main;

typedef struct	s_shedlight
{
	t_v3		light_dir;
	t_v3		spot;
	t_v3		diffuse_light;
	t_v3		spec_light;
	t_v3		ambi_light;
	t_light		*light;
	double		diffuse_k;
	double		spec_k;
	double		atten;
	double		dist;
	int			j;
}				t_shedlight;

typedef struct	s_trace
{
	t_v3		color;
	t_v3		p;
	t_v3		n;
	t_v3		refl;
	t_obj		*o;
	double		t;
	double		k;
}				t_trace;

typedef t_obj	*(*t_new_obj)(t_v3, t_v3, double, t_v3);

/*
**	main functions
*/
t_obj			**parse_scene(t_main *m, char *path);
void			render(t_main *m);
void			handle_events(t_main *m, SDL_Event e);
void			free_mem(t_main *m);

/*
**	objects
*/
t_obj			*new_cylinder(t_v3 center, t_v3 dir, double radius, t_v3 color);
t_obj			*new_cone(t_v3 center, t_v3 dir, double radius, t_v3 color);
t_obj			*new_light(t_v3 location, t_v3 dir, double intens, t_v3 color);
t_obj			*new_plane(t_v3 center, t_v3 dir, double radius, t_v3 color);
t_obj			*new_sphere(t_v3 center, t_v3 dir, double radius, t_v3 color);

/*
**	vectors and matrices
*/
double			v3_dot(t_v3 a, t_v3 b);
double			v3_squared(t_v3 v);
double			v3_length(t_v3 vec);
t_v3			v3_cross(t_v3 a, t_v3 b);
t_v3			v3_mult_scalar(t_v3 v, double scalar);
void			v3_normalize(t_v3 *vec);
t_v3			v3_get(double a, double b, double c);
t_v3			v3_reflected(t_v3 vec, t_v3 n);
void			matrix_apply(t_v3 *vec, t_matrix m);
t_matrix		init_matrix(t_v3 angle);

/*
**	render_utils.c
*/
void			set_pixel(t_main *m, int x, int y, t_v3 color);
t_v3			clamp(t_v3 color);
t_v3			color_lerp(t_v3 a, t_v3 b, double p);

/*
**	misc.c
*/
bool			key_is_rotate(SDL_Keycode k);
bool			key_is_move(SDL_Keycode k);
void			check_leakage(void);
void			print_help(void);
t_obj			**list_to_array(t_list **obj_list, int num);

#endif
