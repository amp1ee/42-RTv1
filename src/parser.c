#include "rtv1.h"

t_obj			**lst_to_arr(t_list *obj_list)
{
	int			num;
	t_list		*elem;
	t_obj		**objs;

	num = 0;
	elem = obj_list;
	while (elem)
	{
		num++;
		elem = elem->next;
	}
	objs = malloc(num * sizeof(*objs));
	elem = obj_list;
	while (elem)
	{
		num--;
		objs[num] = (t_obj *)elem->content;
	}
	return (objs);
}

t_vec3f			parse_vec3f(char *line)
{
	t_vec3f		vec;

	vec.x = atof(line);
	printf("vec.x: %f\n", vec.x);
	line = ft_strchr(line, '_');
	vec.y = atof(&line[1]);
	printf("vec.y: %f\n", vec.y);
	line = ft_strchr(line, '_');
	vec.z = atof(&line[1]);
	printf("vec.z: %f\n", vec.z);
	return (vec);
}

SDL_Color		parse_color(char *line)
{
	SDL_Color	color;
	t_vec3f		tmp;

	tmp = parse_vec3f(line);
	color = (SDL_Color){ tmp.x, tmp.y, tmp.z, 255 };
	return (color);
}

/*
**	S P:10.0_10.0_10.0 D:0_0_0 R:12.0 C:128_12_2
*/
t_obj			*new_object(t_obj *(*obj_func)(), char *line)
{
	t_obj		*obj;
	t_vec3f		*pos;
	t_vec3f		dir;
	double		radius;
	SDL_Color	color;

	while (line && *line != ';')
	{
		if (*line == 'P')
		{
			if (!(pos = malloc(sizeof(t_vec3f))))
				return (NULL);
			*pos = parse_vec3f(&line[2]);
		}
		else if (*line == 'D')
			dir = parse_vec3f(&line[2]);
		else if (*line == 'R')
		{
			radius = atof(&line[2]);
			printf("R: %f\n", radius);
		}
		else if (*line == 'C')
			color = parse_color(&line[2]);
		line++;
	}
	obj = obj_func(pos, dir, radius, color);
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
	const void	*obj_func[] = {
		//new_plane,
		new_sphere,
		//new_cylinder,
		//new_cone,
		//new_light
	};
	char		*line;
	t_list		*obj_list;
	t_obj		*obj;
	//t_obj		**objs;
	int			fd;
	int			i;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (NULL);
	m->obj_num = 0;
	while (get_next_line(fd, &line))
	{
		i = 0;
		while (objs_str[i])
		{
			if (line[0] == objs_str[i])
			{
				printf("line[0]: %c\n", line[0]);
				obj = new_object(obj_func[i], &line[2]);
				ft_lstadd(&obj_list, ft_lstnew(obj, sizeof(obj)));
				m->obj_num++;
			}
			i++;
		}
	}
	return (lst_to_arr(obj_list));
}