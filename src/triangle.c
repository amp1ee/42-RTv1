#include "rtv1.h"

t_obj			*new_triangle(t_v3 *a, t_v3 *b, t_v3 *c,
				SDL_Color color)
{
	t_triang	*tri;
	t_obj		*obj;

	if (!(tri = (t_triang *)malloc(sizeof(t_triang))))
		return (NULL);
	tri->a = a;
	tri->b = b;
	tri->c = c;
	tri->color = color;
	if (!(obj = (t_obj *)malloc(sizeof(t_obj))))
		return (NULL);
	obj->type = TRIANGLE;
	obj->data = tri;
	obj->get_color = triangle_color;
	obj->normal_vec = triangle_normalvec;
	obj->cleanup = triangle_cleanup;
	obj->intersects = triangle_intersect;
	return (obj);
}

bool			triangle_intersect(void *data, t_v3 ray_start, t_v3 ray,
				t_v3 *intersect)
{
	const t_triang	*tri = data;

	t_v3			n = triangle_normalvec(data, *intersect);
	double			d = v3_dot(n, *tri->a);
	double			nr = v3_dot(n, ray);
	if (fabs(nr) <= EPSILON)
		return (false);
	double			t = -(v3_dot(n, ray_start) + d) / nr;
	*intersect = (t_v3){ ray_start.x + t * ray.x,
							ray_start.y + t * ray.y,
							ray_start.z + t * ray.z };
	t_v3			edge0 = get_v3(*tri->a, *tri->b);
	t_v3			edge1 = get_v3(*tri->b, *tri->c);
	t_v3			edge2 = get_v3(*tri->c, *tri->a);
	t_v3			c0 = get_v3(*tri->a, *intersect);
	t_v3			c1 = get_v3(*tri->b, *intersect);
	t_v3			c2 = get_v3(*tri->c, *intersect);
	if (t > 0 && v3_dot(n, v3_cross(edge0, c0)) > 0 &&
			v3_dot(n, v3_cross(edge1, c1)) > 0 &&
			v3_dot(n, v3_cross(edge2, c2)) > 0)
		return (true);
	return (false);
}

SDL_Color		triangle_color(void *data, t_v3 intersect)
{
	const t_triang	*triang = data;

	(void)intersect;
	return (triang->color);
}

t_v3			triangle_normalvec(void *data, t_v3 intersect)
{
	const t_triang	*tri = data;
	t_v3			v1;
	t_v3			v2;
	t_v3			n;

	(void)intersect;
	v1 = get_v3(*tri->a, *tri->b);
	v2 = get_v3(*tri->a, *tri->c);
	n = v3_cross(v1, v2);
	v3_normalize(&n);
	return (n);
}

void			triangle_cleanup(void *data)
{
	t_triang	*triang;

	triang = data;
	ft_memdel((void **)&(triang->a));
	ft_memdel((void **)&(triang->b));
	ft_memdel((void **)&(triang->c));
	ft_memdel((void **)&triang);
}
