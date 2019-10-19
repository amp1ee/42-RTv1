/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oahieiev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/06 17:34:49 by oahieiev          #+#    #+#             */
/*   Updated: 2019/10/06 17:34:52 by oahieiev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

bool				cylinder_intersect(void *data, t_v3 eye, t_v3 rdir,
										t_v3 *intersect)
{
	const t_cylind	*cyl = data;
	const t_v3		pos = cyl->pos;
	const t_v3		dir = cyl->dir;
	t_figure		f;

	f.k = (eye - pos);
	f.l = rdir - (v3_mult_scalar(dir, v3_dot(rdir, dir)));
	f.m = f.k - v3_mult_scalar(dir, v3_dot(dir, f.k));
	f.a = v3_squared(f.l);
	f.b = 2.0 * v3_dot(f.l, f.m);
	f.c = v3_squared(f.m) - SQ(cyl->radius);
	f.d = SQ(f.b) - 4.0 * f.a * f.c;
	if (f.d < 0)
		return (false);
	f.droot = sqrt(f.d);
	f.t1 = (-f.b - f.droot) / (2.0 * f.a);
	f.t2 = (-f.b + f.droot) / (2.0 * f.a);
	f.t = (MIN(f.t1, f.t2) >= 0) ? MIN(f.t1, f.t2) : MAX(f.t1, f.t2);
	*intersect = (t_v3){	eye[0] + f.t * rdir[0],
							eye[1] + f.t * rdir[1],
							eye[2] + f.t * rdir[2]};
	return (f.t > 0);
}

SDL_Color			cylinder_color(void *data, t_v3 intersect)
{
	const t_cylind	*cylinder = data;

	(void)intersect;
	return (cylinder->color);
}

t_v3				cylinder_normalvec(void *data, t_v3 intersect)
{
	const t_cylind	*cyl = data;
	t_v3			n;
	t_v3			dir;

	dir = cyl->dir;
	n = intersect - cyl->pos;
	n = n - v3_mult_scalar(dir, v3_dot(n, dir));
	v3_normalize(&n);
	return (n);
}

void				cylinder_cleanup(void *data)
{
	t_cylind		*cylinder;

	cylinder = data;
	ft_memdel((void **)&cylinder);
}

t_obj				*new_cylinder(t_v3 pos, t_v3 dir, double radius,
									SDL_Color color)
{
	t_cylind		*cyl;
	t_obj			*obj;

	if (!(cyl = malloc(sizeof(t_cylind))))
		return (NULL);
	cyl->pos = pos;
	v3_normalize(&dir);
	cyl->dir = dir;
	cyl->radius = radius;
	cyl->color = color;
	if (!(obj = malloc(sizeof(t_obj))))
		return (NULL);
	obj->type = CYLINDER;
	obj->data = cyl;
	obj->intersects = &cylinder_intersect;
	obj->get_color = &cylinder_color;
	obj->normal_vec = &cylinder_normalvec;
	obj->cleanup = &cylinder_cleanup;
	return (obj);
}
