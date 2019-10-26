/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oahieiev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/06 17:56:31 by oahieiev          #+#    #+#             */
/*   Updated: 2019/10/06 17:56:33 by oahieiev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

bool				cone_intersect(void *data, t_v3 eye, t_v3 rdir,
						t_v3 *intersect)
{
	const t_cone	*cone = data;
	const t_v3		pos = cone->pos;
	const double	a = (cone->angle * M_PI / 180.0);
	t_figure		f;

	f.k = (eye - pos);
	f.e = v3_dot(rdir, cone->dir);
	f.f = v3_dot(f.k, cone->dir);
	f.l = rdir - v3_mult_scalar(cone->dir, f.e);
	f.m = f.k - v3_mult_scalar(cone->dir, f.f);
	f.a = SQ(cos(a)) * v3_squared(f.l) - SQ(sin(a)) * SQ(f.e);
	f.b = 2.0 * SQ(cos(a)) * v3_dot(f.l, f.m)
				- 2.0 * SQ(sin(a)) * f.e * f.f;
	f.c = SQ(cos(a)) * v3_squared(f.m) - SQ(sin(a)) * SQ(f.f);
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

t_v3				cone_color(void *data, t_v3 intersect)
{
	const t_cone	*cone = data;

	(void)intersect;
	return (cone->color);
}

t_v3				cone_normalvec(void *data, t_v3 intersect)
{
	const t_cone	*cone = data;
	t_figure		f;
	t_v3			n;

	f.k = intersect - cone->pos;
	v3_normalize(&f.k);
	f.m = v3_mult_scalar(cone->dir, v3_dot(f.k, cone->dir));
	f.n = v3_mult_scalar(f.k, v3_dot(f.m, f.k));
	n = f.n - f.m;
	v3_normalize(&n);
	return (n);
}

void				cone_cleanup(void *data)
{
	t_cone			*cone;

	cone = data;
	ft_memdel((void **)&cone);
}

t_obj				*new_cone(t_v3 pos, t_v3 dir, double angle, t_v3 color)
{
	t_cone			*cone;
	t_obj			*obj;

	if (!(cone = malloc(sizeof(t_cone))))
		return (NULL);
	cone->pos = pos;
	v3_normalize(&dir);
	cone->dir = dir;
	cone->angle = angle;
	cone->color = color;
	if (!(obj = malloc(sizeof(t_obj))))
		return (NULL);
	obj->type = CONE;
	obj->data = cone;
	obj->intersects = &cone_intersect;
	obj->get_color = &cone_color;
	obj->normal_vec = &cone_normalvec;
	obj->cleanup = &cone_cleanup;
	return (obj);
}
