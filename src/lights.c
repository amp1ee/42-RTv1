/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lights.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oahieiev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/20 18:46:27 by oahieiev          #+#    #+#             */
/*   Updated: 2019/10/20 18:46:29 by oahieiev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_v3				lights_color(void *data, t_v3 intersect)
{
	const t_light	*light = data;

	(void)intersect;
	return (light->color);
}

void				lights_cleanup(void *data)
{
	t_light			*lights;

	lights = data;
	ft_memdel((void **)&lights);
}

t_obj				*new_light(t_v3 pos, t_v3 dir, double brightness,
								t_v3 color)
{
	t_light			*light;
	t_obj			*obj;

	(void)dir;
	if (!(light = malloc(sizeof(*light))))
		return (NULL);
	light->pos = pos;
	light->brightness = brightness;
	light->color = color;
	if (!(obj = malloc(sizeof(*obj))))
		return (NULL);
	obj->type = LIGHT_SOURCE;
	obj->data = light;
	obj->intersects = NULL;
	obj->normal_vec = NULL;
	obj->cleanup = &lights_cleanup;
	obj->get_color = &lights_color;
	return (obj);
}
