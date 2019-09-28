#include "rtv1.h"

t_v3				plane_normalvec(void *data, t_v3 intersect)
{
	t_plane	*plane = (t_plane *)data;

	(void)intersect;
	v3_normalize(&(plane->normal));
	return (plane->normal);
}

bool				plane_intersect(void *data, t_v3 eye, t_v3 rdir,
										t_v3 *intersect)
{
	const t_plane	*pln = data;
	const t_v3	pos = *pln->pos;
	const t_v3	dir = plane_normalvec(data, *intersect);
	double			t;

	t_v3			oc = pos - eye;
	double			a = v3_dot(rdir, dir);
	double			b = v3_dot(oc, dir);
	if (fabs(a) <= EPSILON)
		return (false);
	t = (b / a);
	*intersect = (t_v3){
		eye[0] + t * rdir[0],
		eye[1] + t * rdir[1],
		eye[2] + t * rdir[2]
	};
	return (t > 0);
}

SDL_Color			plane_color(void *data, t_v3 intersect)
{
	const t_plane	*plane = (t_plane *)data;

	(void)intersect;
	return (plane->color);
}

void				plane_cleanup(void *data)
{
	t_plane			*plane;

	plane = (t_plane *)data;
	ft_memdel((void **)&(plane->pos));
	ft_memdel((void **)&plane);
}

t_obj				*new_plane(t_v3 *center, t_v3 dir, double radius,
							SDL_Color color)
{
	t_plane			*plane;
	t_obj			*obj;

	if (!(plane = malloc(sizeof(*plane))))
		return (NULL);
	plane->pos = center;
	plane->normal = dir;
	(void)radius;
	plane->color = color;
	if (!(obj = malloc(sizeof(*obj))))
		return (NULL);
	obj->data = plane;
	obj->type = PLANE;
	obj->intersects = &plane_intersect;
	obj->get_color = &plane_color;
	obj->normal_vec = &plane_normalvec;
	obj->cleanup = &plane_cleanup;
	return (obj);
}
