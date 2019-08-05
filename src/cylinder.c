#include "rtv1.h"
/*
** 	D_Va = D - dot(D, Va) * Va;
	OC_Va = (O - obj->dir) - dot((O - obj->dir), Va) * Va;
	k1 = dot(D_Va, D_Va);
	k2 = 2.0F * dot(D_Va, OC_Va);
	k3 = dot(OC_Va, OC_Va) - obj->rad * obj->rad;

	descr = k2 * k2 - 4.0F * k1 * k3;
	if (descr < 0)
		return ((float2){INFINITY, INFINITY});
	T = (float2){
		(-k2 - sqrt(descr)) / (2.0F * k1),
		(-k2 + sqrt(descr)) / (2.0F * k1)};

	Va = fast_normalize(obj->dir - obj->pos);
*/

bool		cylinder_intersect(void *data, t_vec3f ray_start, t_vec3f ray,
				t_vec3f *intersect)
{
	const t_cylind	*cyl = data;
	const t_vec3f	center = *cyl->center;
	const double	r = cyl->radius;
	const t_vec3f	dir = cyl->dir;

	t_vec3f		CD = get_vec3f(center, dir);
	t_vec3f		Va = vec3f_normalize(&CD);
	double		d_rayVa = vec3f_dot(ray, Va);
	t_vec3f		m_Va = vec3f_
	
	if (d < 0)
		return (false);
	double sqrtd = sqrt(d);
	const double		t1 = (-b + sqrtd);
	const double		t2 = (-b - sqrtd);
	const double		t = (MIN(t1, t2) >= 0) ? MIN(t1, t2) : MAX(t1, t2);
	*intersect = (t_vec3f){	ray_start.x + t * ray.x,
							ray_start.y + t * ray.y,
							ray_start.z + t * ray.z};
	return (t > 0);
}

SDL_Color	cylinder_color(void *data, t_vec3f intersect)
{
	const t_cylind	*cylinder = data;

	(void)intersect;
	return (cylinder->color);
}

t_vec3f		cylinder_normalvec(void *data, t_vec3f intersect)
{
	const t_cylind	*cyl = data;
	t_vec3f			n;
	t_vec3f			dir;
	t_vec3f			x;

	dir = cyl->dir;
	n = vec3f_cross(get_vec3f(*cyl->center, intersect), dir);
	vec3f_normalize(&n);
	return (n);
}

void		cylinder_cleanup(void *data)
{
	t_cylind *cylinder;

	cylinder = data;
	free(cylinder);
}

t_obj		*new_cylinder(t_vec3f *center, t_vec3f dir, double radius, SDL_Color color)
{
	t_cylind	*cyl;
	t_obj		*obj;

	if (!(cyl = malloc(sizeof(t_cylind))))
		return (NULL);
	cyl->center = center;
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