#include "rtv1.h"

bool				cone_intersect(void *data, t_vec3f eye, t_vec3f rdir,
						t_vec3f *intersect)
{
	const t_cone	*cone = data;
	const t_vec3f	pos = *cone->pos;
	const double	a = (cone->angle * M_PI / 180.0);
	const t_vec3f	dir = cone->dir;

	t_vec3f		Va = dir - pos;
	vec3f_normalize(&Va);
	t_vec3f		CO = (eye - pos);

	double		DVa = vec3f_dot(rdir, Va);
	double		COVa = vec3f_dot(CO, Va);
	t_vec3f		DVaD = rdir - vec3f_multsc(Va, DVa);
	t_vec3f		DVaCO = CO - vec3f_multsc(Va, COVa);

	double A = SQ(cos(a)) * vec3f_squared(DVaD) - SQ(sin(a)) * SQ(DVa);
	double B = 2.0 * SQ(cos(a)) * vec3f_dot(DVaD, DVaCO)
				- 2.0 * SQ(sin(a)) * DVa * COVa;
	double C = SQ(cos(a)) * vec3f_squared(DVaCO) - SQ(sin(a)) * SQ(COVa);
	double d = SQ(B) - 4.0 * A * C;
	if (d < 0)
		return (false);
	double sqrtd = sqrt(d);
	const double		t1 = (-B - sqrtd) / (2.0 * A);
	const double		t2 = (-B + sqrtd) / (2.0 * A);
	const double		t = (MIN(t1, t2) >= 0) ? MIN(t1, t2) : MAX(t1, t2);
	*intersect = (t_vec3f){	eye[0] + t * rdir[0],
							eye[1] + t * rdir[1],
							eye[2] + t * rdir[2]};
	return (t > 0);
}

SDL_Color			cone_color(void *data, t_vec3f intersect)
{
	const t_cone	*cone = data;

	(void)intersect;
	return (cone->color);
}

t_vec3f				cone_normalvec(void *data, t_vec3f intersect)
{
	const t_cone	*cone = data;
	t_vec3f			N, t;
	t_vec3f			dir;

	dir = cone->dir;
	t_vec3f CP = intersect - *(cone->pos);
	t = dir - *(cone->pos);
	vec3f_normalize(&t);
	t_vec3f CQ = vec3f_multsc(t, vec3f_dot(CP, t));
	t_vec3f cp = CP;
	vec3f_normalize(&cp);
	t_vec3f CN = vec3f_multsc(cp, vec3f_dot(CQ, cp));
	N = CN - CQ;
	vec3f_normalize(&N);
	return (N);
}

void				cone_cleanup(void *data)
{
	t_cone *cone;

	cone = data;
	ft_memdel((void **)&(cone->pos));
	ft_memdel((void **)&cone);
}

t_obj				*new_cone(t_vec3f *pos, t_vec3f dir, double angle,
						SDL_Color color)
{
	t_cone			*cone;
	t_obj			*obj;

	if (!(cone = malloc(sizeof(t_cone))))
		return (NULL);
	cone->pos = pos;
	cone->dir = dir;
	cone->angle = angle;
	cone->color = color;
	if (!(obj = malloc(sizeof(t_obj))))
		return (NULL);
	obj->type = CONE;
	obj->data = cone;
	obj->intersects = &cone_intersect;
	obj->get_color = &cone_color;
	obj->normal_vec = &cone_normalvec;
	obj->cleanup = &cone_cleanup;
	return (obj);
}