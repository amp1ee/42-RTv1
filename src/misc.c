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
