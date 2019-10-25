/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oahieiev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/20 18:45:00 by oahieiev          #+#    #+#             */
/*   Updated: 2019/10/20 18:45:02 by oahieiev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void				free_mem(t_main *m)
{
	int				obj_num;

	obj_num = m->obj_num;
	while (obj_num--)
	{
		(m->objects[obj_num])->cleanup((m->objects[obj_num])->data);
		ft_memdel((void **)&(m->objects[obj_num]));
	}
	ft_memdel((void **)&(m->objects));
	ft_memdel((void **)&(m->cam));
}

t_cam				*init_cam(t_v3 start_pos)
{
	t_cam			*cam;

	if (!(cam = malloc(sizeof(t_cam))))
		return (NULL);
	cam->angle = (t_v3){ 0, 0, 0 };
	cam->rot_mtx = init_matrix(cam->angle);
	cam->pos = start_pos;
	matrix_apply(&(cam->pos), cam->rot_mtx);
	cam->focus = FOCUS;
	return (cam);
}

t_main				*rtv1_init(char **argv)
{
	t_main			*m;

	if (!(m = malloc(sizeof(t_main))))
		return (NULL);
	m->objects = parse_scene(m, argv[1]);
	if (m->objects == NULL)
	{
		perror(argv[1]);
		ft_memdel((void **)&m);
		return (NULL);
	}
	if (!(m->cam = init_cam(m->start_pos)))
	{
		ft_memdel((void **)&m);
		return (NULL);
	}
	m->recur_depth = (argv[2]) ? ft_atoi(argv[2]) : 3;
	if (SDL_Init(SDL_INIT_VIDEO) != 0 ||
		!(m->window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED,
										SDL_WINDOWPOS_CENTERED, W, H, 0)) ||
		!(m->screen = SDL_GetWindowSurface(m->window)))
		return (NULL);
	return (m);
}

void				main_loop(t_main *m)
{
	SDL_Event		e;

	SDL_RaiseWindow(m->window);
	m->running = true;
	render(m);
	while (m->running)
	{
		while (SDL_PollEvent(&e))
			handle_events(m, e);
	}
}

int					main(int argc, char *argv[])
{
	t_main			*m;

	if (argc < 2 || argc > 3)
	{
		ft_putendl(USAGE);
		ft_putendl("./RTv1 --help for details");
		return (1);
	}
	atexit(check_leakage);
	if (ft_strequ(argv[1], "--help"))
	{
		ft_putendl(USAGE);
		ft_putendl(SCENE_FORMAT);
		ft_putendl(CONTROLS);
		return (0);
	}
	m = rtv1_init(argv);
	if (m == NULL)
	{
		ft_putendl("Initialization error");
		return (10);
	}
	main_loop(m);
	return (0);
}
