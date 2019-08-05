#include "rtv1.h"

t_obj			*new_plane(t_vec3f *center, t_vec3f dir, double radius, SDL_Color color)
{
	(void)center;
	(void)dir;
	(void)radius;
	(void)color;
	return NULL; }

t_obj			*new_cylinder(t_vec3f *center, t_vec3f dir, double radius, SDL_Color color)
{
	(void)center;
	(void)dir;
	(void)radius;
	(void)color;
	return NULL; }
t_obj			*new_cone(t_vec3f *center, t_vec3f dir, double radius, SDL_Color color)
{
	(void)center;
	(void)dir;
	(void)radius;
	(void)color;
	return NULL; }
t_obj			*new_light(t_vec3f *center, t_vec3f dir, double radius, SDL_Color color)
{
	(void)center;
	(void)dir;
	(void)radius;
	(void)color;
	return NULL; }

void			ft_lstpush(t_list **dest, t_list *src)
{
	t_list		*cur;

	if (!dest || !src)
		return ;
	if (*dest == NULL)
		*dest = src;
	else
	{
		cur = *dest;
		while (cur->next)
			cur = cur->next;
		cur->next = src;
	}
}


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
		objs[--num] = (t_obj *)(elem->content);
		elem = elem->next;
	}
	return (objs);
}

t_vec3f			parse_vec3f(char *line)
{
	t_vec3f		vec;
	size_t		pos;

	vec.x = atof(line);
	printf("vec.x: %f\n", vec.x);
	pos = ft_strpos(line, '_');
	line += (pos + 1);
	vec.y = atof(line);
	printf("vec.y: %f\n", vec.y);
	pos = ft_strpos(line, '_');
	line += (pos + 1);
	vec.z = atof(line);
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
t_obj			*ft_new_object(t_of obj_creator, char *line)
{
	t_obj		*obj;
	t_vec3f		*pos;
	t_vec3f		dir;
	double		radius;
	SDL_Color	color;

	while (*line)
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
	obj = obj_creator(pos, dir, radius, color);
	printf("ret obj->intersects: %p\n", obj->intersects);
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
	//t_obj		**objs;
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
				printf("line[0]: %c\n", line[0]);
				obj = ft_new_object((obj_func[i]), &line[2]);
				ft_lstpush(&obj_list, ft_lstnew(obj, sizeof(*obj)));
				m->obj_num++;
			}
			i++;
		}
	}
	close(fd);
	return (lst_to_arr(obj_list));
}