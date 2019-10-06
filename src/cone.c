#include "rtv1.h"

bool				cone_intersect(void *data, t_v3 eye, t_v3 rdir,
						t_v3 *intersect)
{
	const t_cone	*cone = data;
	const t_v3	pos = *cone->pos;
	const double	a = (cone->angle * M_PI / 180.0);
	const t_v3	dir = cone->dir;

	t_v3		Va = dir;
	v3_normalize(&Va);
	t_v3		CO = (eye - pos);

	double		DVa = v3_dot(rdir, Va);
	double		COVa = v3_dot(CO, Va);
	t_v3		DVaD = rdir - v3_multsc(Va, DVa);
	t_v3		DVaCO = CO - v3_multsc(Va, COVa);

	double A = SQ(cos(a)) * v3_squared(DVaD) - SQ(sin(a)) * SQ(DVa);
	double B = 2.0 * SQ(cos(a)) * v3_dot(DVaD, DVaCO)
				- 2.0 * SQ(sin(a)) * DVa * COVa;
	double C = SQ(cos(a)) * v3_squared(DVaCO) - SQ(sin(a)) * SQ(COVa);
	double d = SQ(B) - 4.0 * A * C;
	if (d < 0)
		return (false);
	double sqrtd = sqrt(d);
	const double		t1 = (-B - sqrtd) / (2.0 * A);
	const double		t2 = (-B + sqrtd) / (2.0 * A);
	const double		t = (MIN(t1, t2) >= 0) ? MIN(t1, t2) : MAX(t1, t2);
	*intersect = (t_v3){	eye[0] + t * rdir[0],
							eye[1] + t * rdir[1],
							eye[2] + t * rdir[2]};
	return (t > 0);
}

SDL_Color			cone_color(void *data, t_v3 intersect)
{
	const t_cone	*cone = data;

	(void)intersect;
	return (cone->color);
}

t_v3				cone_normalvec(void *data, t_v3 intersect)
{
	const t_cone	*cone = data;
	t_v3			N, t;
	t_v3			dir;

	dir = cone->dir;
	t_v3 CP = intersect - *(cone->pos);
	t = dir;
	v3_normalize(&t);
	t_v3 CQ = v3_multsc(t, v3_dot(CP, t));
	t_v3 cp = CP;
	v3_normalize(&cp);
	t_v3 CN = v3_multsc(cp, v3_dot(CQ, cp));
	N = CN - CQ;
	v3_normalize(&N);
	return (N);
}

void				cone_cleanup(void *data)
{
	t_cone *cone;

	cone = data;
	ft_memdel((void **)&(cone->pos));
	ft_memdel((void **)&cone);
}

t_obj				*new_cone(t_v3 *pos, t_v3 dir, double angle,
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
