#include "rtv1.h"


t_obj			*new_cone(t_vec3f *center, t_vec3f dir, double radius, SDL_Color color)
{
	(void)center;
	(void)dir;
	(void)radius;
	(void)color;
	return NULL; }

// TODO:
// double			ft_atof()

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

t_vec3f			parse_vec3f(char *line)
{
	t_vec3f		vec;
	size_t		pos;

	vec[0] = atof(line);
	//printf("vec.x: %f\n", vec[0]);
	pos = ft_strpos(line, '_');
	line += (pos + 1);
	vec[1] = atof(line);
	//printf("vec.y: %f\n", vec[1]);
	pos = ft_strpos(line, '_');
	line += (pos + 1);
	vec[2] = atof(line);
	//printf("vec.z: %f\n", vec[2]);
	return (vec);
}

SDL_Color		parse_color(char *line)
{
	SDL_Color	color;
	t_vec3f		tmp;

	tmp = parse_vec3f(line);
	color = (SDL_Color){ tmp[0], tmp[1], tmp[2], 255 };
	return (color);
}

/*
**	S P:10.0_10.0_10.0 D:0_0_0 R:12.0 C:128_12_2
*/
t_obj			*ft_new_object(t_of obj_creator, char *line)
{
	t_obj		*obj;
	t_vec3f		*pos;
	t_vec3f		dir;
	double		radius;
	SDL_Color	color;

	while (*line && *line != '#')
	{
		if (*line == 'P')
		{
			if (!(pos = malloc(sizeof(t_vec3f))))
				return (NULL);
			*pos = parse_vec3f(&line[2]);
		}
		else if (*line == 'D')
		{
			dir = parse_vec3f(&line[2]);
			//printf("dir: %f %f %f\n", dir[0], dir[1], dir[2]);
		}
		else if (*line == 'R')
		{
			radius = atof(&line[2]);
			//printf("R: %f\n", radius);
		}
		else if (*line == 'C')
			color = parse_color(&line[2]);
		line++;
	}
	obj = obj_creator(pos, dir, radius, color);
	return (obj);
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
	int			fd;
	int			i;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (NULL);
	m->obj_num = 0;
	obj_list = NULL;
	while (get_next_line(fd, &line) > 0)
	{
		i = 0;
		while (objs_str[i])
		{
			if (line[0] == objs_str[i])
			{
				//printf("line[0]: %c\n", line[0]);
				obj = ft_new_object((obj_func[i]), &line[2]);
				ft_lstadd(&obj_list, ft_lstnew(obj, sizeof(*obj)));
				ft_memdel((void **)&obj);
				m->obj_num++;
			}
			i++;
		}
		ft_strdel(&line);
	}
	close(fd);
	return (lst_to_arr(&obj_list, m->obj_num));
}