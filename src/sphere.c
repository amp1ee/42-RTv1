/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oahieiev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/20 18:45:53 by oahieiev          #+#    #+#             */
/*   Updated: 2019/10/20 18:45:55 by oahieiev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

bool				sphere_intersect(void *data, t_v3 ray_start, t_v3 ray,
									t_v3 *intersect)
{
	const t_sphere	*sph = data;
	t_figure		f;

	f.k = ray_start - sph->pos;
	f.b = v3_dot(ray, f.k);
	f.c = v3_squared(f.k) - SQ(sph->radius);
	f.d = SQ(f.b) - f.c;
	if (f.d < 0)
		return (false);
	f.droot = sqrt(f.d);
	f.t1 = (-f.b - f.droot);
	f.t2 = (-f.b + f.droot);
	f.t = (MIN(f.t1, f.t2) >= 0) ? MIN(f.t1, f.t2) : MAX(f.t1, f.t2);
	*intersect = (t_v3){	ray_start[0] + f.t * ray[0],
							ray_start[1] + f.t * ray[1],
							ray_start[2] + f.t * ray[2]
						};
	return (f.t > 0);
}

SDL_Color			sphere_color(void *data, t_v3 intersect)
{
	const t_sphere	*sphere = data;

	(void)intersect;
	return (sphere->color);
}

t_v3				sphere_normalvec(void *data, t_v3 hit)
{
	const t_sphere	*sphere = data;
	t_v3			n;

	n = hit - sphere->pos;
	v3_normalize(&n);
	return (n);
}

void				sphere_cleanup(void *data)
{
	t_sphere		*sphere;

	sphere = data;
	ft_memdel((void **)&sphere);
}

t_obj				*new_sphere(t_v3 pos, t_v3 dir, double radius,
								SDL_Color color)
{
	t_sphere		*sph;
	t_obj			*obj;

	(void)dir;
	if (!(sph = malloc(sizeof(t_sphere))))
		return (NULL);
	sph->pos = pos;
	sph->radius = radius;
	sph->color = color;
	if (!(obj = malloc(sizeof(t_obj))))
		return (NULL);
	obj->type = SPHERE;
	obj->data = sph;
	obj->intersects = &sphere_intersect;
	obj->get_color = &sphere_color;
	obj->normal_vec = &sphere_normalvec;
	obj->cleanup = &sphere_cleanup;
	return (obj);
}
