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

static t_obj		*new_object(t_new_obj obj_creator, char *line, size_t len)
{
	t_v3			pos;
	t_v3			dir;
	double			radius;
	t_v3			color;

	while (len > 2 && *line && *line != '#')
	{
		if (*line == 'P')
			pos = parse_vector(&line[2]);
		else if (*line == 'D')
			dir = parse_vector(&line[2]);
		else if (*line == 'R')
			radius = ft_atof(&line[2]);
		else if (*line == 'C')
			color = parse_vector(&line[2]);
		line++;
		len--;
	}
	return (obj_creator(pos, dir, radius, color));
}

static int			parsing_loop(t_main *m, int fd, char *object_types,
								t_new_obj *object_creators)
{
	char			*line;
	int				type;
	int				ret;
	size_t			len;
	t_obj			*obj;

	while ((ret = get_next_line(fd, &line)) > 0)
	{
		type = -1;
		while (object_types[++type])
		{
			if (line[0] == object_types[type] && (len = ft_strlen(line)) > 5)
			{
				obj = new_object(object_creators[type], &line[2], len - 2);
				ft_lstadd(&(m->obj_list), ft_lstnew(obj, sizeof(*obj)));
				ft_memdel((void **)&obj);
				m->obj_num++;
			}
		}
		if (line[0] == 's' && ft_strlen(line) > 6)
			m->start_pos = parse_vector(&line[2]);
		ft_strdel(&line);
	}
	ft_strdel(&line);
	return (ret);
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
	int				r;

	if ((fd = open(path, O_RDONLY)) < 0)
		return (NULL);
	m->obj_num = 0;
	m->obj_list = NULL;
	m->start_pos = (t_v3){0, 0, 0};
	r = parsing_loop(m, fd, (char *)object_types, (t_new_obj *)object_creators);
	close(fd);
	return ((r < 0) ? NULL : list_to_array(&(m->obj_list), m->obj_num));
}
