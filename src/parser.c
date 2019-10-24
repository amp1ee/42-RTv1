/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oahieiev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/20 19:00:20 by oahieiev          #+#    #+#             */
/*   Updated: 2019/10/20 19:00:22 by oahieiev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static t_v3			parse_vector(char *line)
{
	t_v3			vec;
	size_t			pos;

	vec[0] = ft_atof(line);
	pos = ft_strpos(line, ',');
	line += (pos + 1);
	vec[1] = ft_atof(line);
	pos = ft_strpos(line, ',');
	line += (pos + 1);
	vec[2] = ft_atof(line);
	return (vec);
}

static t_v3			parse_color(char *line)
{
	return (parse_vector(line));
}

t_obj				*ft_new_object(t_new_obj obj_creator, char *line)
{
	t_v3			pos;
	t_v3			dir;
	double			radius;
	t_v3			color;

	while (*line && *line != '#')
	{
		if (*line == 'P')
		{
			pos = parse_vector(&line[2]);
		}
		else if (*line == 'D')
		{
			dir = parse_vector(&line[2]);
		}
		else if (*line == 'R')
		{
			radius = ft_atof(&line[2]);
		}
		else if (*line == 'C')
			color = parse_color(&line[2]);
		line++;
	}
	return (obj_creator(pos, dir, radius, color));
}

void				parsing_loop(t_main *m, int fd, char *object_types,
								t_new_obj *object_creators)
{
	char			*line;
	int				type;
	t_obj			*obj;

	while (get_next_line(fd, &line) > 0)
	{
		type = 0;
		while (object_types[type])
		{
			if (line[0] == object_types[type])
			{
				obj = ft_new_object((object_creators[type]), &line[2]);
				ft_lstadd(&(m->obj_list), ft_lstnew(obj, sizeof(*obj)));
				ft_memdel((void **)&obj);
				m->obj_num++;
			}
			type++;
		}
		if (line[0] == 's')
			m->start_pos = parse_vector(&line[2]);
		ft_strdel(&line);
	}
}

/*
**  P - plane
**  S - sphere
**  C - cylinder
**  c - cone
**  L - light source
*/

t_obj				**parse_scene(t_main *m, char *path)
{
	const char		*object_types = "PSCcL";
	const t_new_obj	object_creators[] = {
		&new_plane,
		&new_sphere,
		&new_cylinder,
		&new_cone,
		&new_light
	};
	int				fd;

	if ((fd = open(path, O_RDONLY)) < 0)
		return (NULL);
	m->obj_num = 0;
	m->obj_list = NULL;
	m->start_pos = (t_v3){0, 0, 0};
	parsing_loop(m, fd, (char *)object_types, (t_new_obj *)object_creators);
	close(fd);
	return (list_to_array(&(m->obj_list), m->obj_num));
}
