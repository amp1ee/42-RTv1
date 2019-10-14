#include "rtv1.h"

t_obj			**lst_to_arr(t_list **obj_list, int num)
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

t_v3			parse_v3(char *line)
{
	t_v3		vec;
	size_t		pos;

	vec[0] = ft_atof(line);
	pos = ft_strpos(line, '_');
	line += (pos + 1);
	vec[1] = ft_atof(line);
	pos = ft_strpos(line, '_');
	line += (pos + 1);
	vec[2] = ft_atof(line);
	return (vec);
}

SDL_Color		parse_color(char *line)
{
	SDL_Color	color;
	t_v3		tmp;

	tmp = parse_v3(line);
	color = (SDL_Color){ tmp[0], tmp[1], tmp[2], 255 };
	return (color);
}

/*
**	S P:10.0_10.0_10.0 D:0_0_0 R:12.0 C:128_12_2
*/
t_obj			*ft_new_object(t_of obj_creator, char *line)
{
	t_obj		*obj;
	t_v3		*pos;
	t_v3		dir;
	double		radius;
	SDL_Color	color;

	if (!(pos = malloc(sizeof(t_v3))))
		return (NULL);
	while (*line && *line != '#')
	{
		if (*line == 'P')
		{
			*pos = parse_v3(&line[2]);
		}
		else if (*line == 'D')
		{
			dir = parse_v3(&line[2]);
		}
		else if (*line == 'R')
		{
			radius = ft_atof(&line[2]);
		}
		else if (*line == 'C')
			color = parse_color(&line[2]);
		line++;
	}
	obj = obj_creator(pos, dir, radius, color);
	return (obj);
}

void			print_vec(t_v3 v)
{
	printf("%.2f %.2f %.2f\n", v[0], v[1], v[2]);
}

/*
**  P - plane
**  S - sphere
**  C - cylinder
**  c - cone
**  L - light source
*/

t_obj			**parse_scene(t_main *m, char *path)
{
	const char	*objs_str = "PSCcL";
	const t_of	obj_func[] = {
		&new_plane,
		&new_sphere,
		&new_cylinder,
		&new_cone,
		&new_light
	};
	char		*line;
	t_list		*obj_list;
	t_obj		*obj;
	int			type;
	int			fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (NULL);
	m->obj_num = 0;
	m->start_pos = (t_v3){0, 0, 0};
	obj_list = NULL;
	while (get_next_line(fd, &line) > 0)
	{
		type = 0;
		while (objs_str[type])
		{
			if (line[0] == objs_str[type])
			{
				obj = ft_new_object((obj_func[type]), &line[2]);
				ft_lstadd(&obj_list, ft_lstnew(obj, sizeof(*obj)));
				ft_memdel((void **)&obj);
				m->obj_num++;
			}
			type++;
		}
		if (line[0] == 's')
			m->start_pos = parse_v3(&line[2]);
		ft_strdel(&line);
	}
	close(fd);
	return (lst_to_arr(&obj_list, m->obj_num));
}
