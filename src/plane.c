/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oahieiev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/06 17:59:09 by oahieiev          #+#    #+#             */
/*   Updated: 2019/10/06 17:59:10 by oahieiev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

bool				plane_intersect(void *data, t_v3 ray_start, t_v3 ray_dir,
									t_v3 *intersect)
{
	const t_plane	*pln = data;
	t_figure		f;

	f.dir = plane_normalvec(data, *intersect);
	f.k = pln->pos - ray_start;
	f.a = v3_dot(ray_dir, f.dir);
	f.b = v3_dot(f.k, f.dir);
	if (fabs(f.a) <= EPSILON)
		return (false);
	f.t = (f.b / f.a);
	*intersect = (t_v3){
		ray_start[0] + f.t * ray_dir[0],
		ray_start[1] + f.t * ray_dir[1],
		ray_start[2] + f.t * ray_dir[2]
	};
	return (f.t > 0);
}

SDL_Color			plane_color(void *data, t_v3 intersect)
{
	const t_plane	*plane = data;

	(void)intersect;
	return (plane->color);
}

t_v3				plane_normalvec(void *data, t_v3 intersect)
{
	const t_plane	*plane = data;

	(void)intersect;
	return (plane->normal);
}

void				plane_cleanup(void *data)
{
	t_plane			*plane;

	plane = (t_plane *)data;
	ft_memdel((void **)&plane);
}

t_obj				*new_plane(t_v3 center, t_v3 dir, double radius,
							SDL_Color color)
{
	t_plane			*plane;
	t_obj			*obj;

	if (!(plane = malloc(sizeof(*plane))))
		return (NULL);
	plane->pos = center;
	v3_normalize(&dir);
	plane->normal = dir;
	(void)radius;
	plane->color = color;
	if (!(obj = malloc(sizeof(*obj))))
		return (NULL);
	obj->data = plane;
	obj->type = PLANE;
	obj->intersects = &plane_intersect;
	obj->get_color = &plane_color;
	obj->normal_vec = &plane_normalvec;
	obj->cleanup = &plane_cleanup;
	return (obj);
}
