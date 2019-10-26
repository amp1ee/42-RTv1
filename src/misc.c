/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oahieiev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/20 19:44:23 by oahieiev          #+#    #+#             */
/*   Updated: 2019/10/20 19:44:43 by oahieiev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

bool			key_is_move(SDL_Keycode k)
{
	return (k == SDLK_w || k == SDLK_a || k == SDLK_s ||
			k == SDLK_d || k == SDLK_q || k == SDLK_e);
}

bool			key_is_rotate(SDL_Keycode k)
{
	return (k == SDLK_DOWN || k == SDLK_UP || k == SDLK_x ||
			k == SDLK_RIGHT || k == SDLK_LEFT || k == SDLK_z);
}

void			check_leakage(void)
{
	system("leaks RTv1 2>/dev/null");
}

void			print_help(void)
{
	ft_putstr("=== Scene format: ===\n\n"\
		"\t[Camera position]\n"\
		"\t[Object type] [Position] [Direction] "\
		"[Radius/Radiance] [Color]\n\n"\
		"\tCamera position:\ts:[x,y,z]\t*if unspecified, default is 0,0,0\n"\
		"\tObject types:\n"\
		"\t\t\t\tP - Plane\n"\
		"\t\t\t\tS - Sphere\n"\
		"\t\t\t\tC - Cylinder\n"\
		"\t\t\t\tc - Cone\n"\
		"\t\t\t\tL - Light source\n"\
		"\tPosition/Direction:\tP/D:[x,y,z]\n"\
		"\tRadius/Radiance:\tR:1.0\n"\
		"\tColor:\t\t\tC:[r,g,b] in 0..255 range\n"\
		"\tUse '#' for comments\n");
	ft_putstr("=== Controls: ===\n\n"\
		"\tW/A/S/D:\t\tMove forward/left/backward/right\n"\
		"\tQ/E\t\t\tMove camera up/down\n"\
		"\tArrow keys/Z/X\t\tRotate camera\n");
}

t_obj			**list_to_array(t_list **obj_list, int num)
{
	t_list		*elem;
	t_obj		**objs;

	ft_lstreverse(obj_list);
	if (!(objs = malloc(num * sizeof(*objs))))
		return (NULL);
	elem = *obj_list;
	while (elem)
	{
		if (!(objs[--num] = malloc(sizeof(t_obj))))
			return (NULL);
		ft_memcpy(objs[num], (t_obj *)(elem->content), sizeof(t_obj));
		elem = elem->next;
	}
	ft_lstdel(obj_list, ft_bzero);
	return (objs);
}
