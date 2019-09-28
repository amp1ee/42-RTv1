#include "rtv1.h"

bool		cylinder_intersect(void *data, t_v3 eye, t_v3 rdir,
				t_v3 *intersect)
{
	const t_cylind	*cyl = data;
	const t_v3	pos = *cyl->pos;
	const double	r = cyl->radius;
	const t_v3	dir = cyl->dir;

	t_v3		Va = dir - pos;
	v3_normalize(&Va);
	t_v3		D_Va = rdir - (v3_multsc(Va, v3_dot(rdir, Va)));

	t_v3		DO = (eye - pos);
	t_v3		OC_Va = DO - v3_multsc(Va, v3_dot(Va, DO));

	double A = v3_squared(D_Va);
	double B = 2.0 * v3_dot(D_Va, OC_Va);
	double C = v3_squared(OC_Va) - SQ(r);
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

SDL_Color	cylinder_color(void *data, t_v3 intersect)
{
	const t_cylind	*cylinder = data;

	(void)intersect;
	return (cylinder->color);
}

t_v3		cylinder_normalvec(void *data, t_v3 intersect)
{
	const t_cylind	*cyl = data;
	t_v3			n, t;
	t_v3			dir;

	dir = cyl->dir;
	n = intersect - *(cyl->pos);
	t = dir - *(cyl->pos);
	v3_normalize(&t);
	n = n - v3_multsc(t, v3_dot(n, t));
	v3_normalize(&n);
	return (n);
}

void		cylinder_cleanup(void *data)
{
	t_cylind *cylinder;

	cylinder = data;
	ft_memdel((void **)&(cylinder->pos));
	ft_memdel((void **)&cylinder);
}

t_obj		*new_cylinder(t_v3 *pos, t_v3 dir, double radius, SDL_Color color)
{
	t_cylind	*cyl;
	t_obj		*obj;

	if (!(cyl = malloc(sizeof(t_cylind))))
		return (NULL);
	cyl->pos = pos;
	cyl->dir = dir;
	cyl->radius = radius;
	cyl->color = color;
	if (!(obj = malloc(sizeof(t_obj))))
		return (NULL);
	obj->type = CYLINDER;
	obj->data = cyl;
	obj->intersects = &cylinder_intersect;
	obj->get_color = &cylinder_color;
	obj->normal_vec = &cylinder_normalvec;
	obj->cleanup = &cylinder_cleanup;
	return (obj);
}
