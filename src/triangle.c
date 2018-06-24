#include "rtv1.h"

t_obj			*new_triangle(t_vec3f *a, t_vec3f *b, t_vec3f *c,
				SDL_Color *color)
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
	obj->data = tri;
	obj->get_color = triangle_color;
	obj->normal_vec = triangle_normalvec;
	obj->cleanup = triangle_cleanup;
	obj->intersects = triangle_intersect;
	return (obj);
}

bool			triangle_intersect(void *data, t_vec3f ray_start, t_vec3f ray,
				t_vec3f *intersect)
{
	const t_triang	*tri = data;
	float			d = vec3f_dotprod(tri->normal_vec, *tri->a);
	float			nr = vec3f_dotprod(tri->normal_vec, ray);
	if (nr == 0)
		return (false);
	float			t = -(vec3f_dotprod(tri->normal_vec, ray_start) + d) / nr;
	*intersect = (t_vec3f){ ray_start.x + t * ray.x,
							ray_start.y + t * ray.y,
							ray_start.z + t * ray.z };
	t_vec3f			edge0 = get_vec3f(*tri->a, *tri->b);
	t_vec3f			edge1 = get_vec3f(*tri->b, *tri->c);
	t_vec3f			edge2 = get_vec3f(*tri->c, *tri->a);
	t_vec3f			c0 = get_vec3f(*tri->a, *intersect);
	t_vec3f			c1 = get_vec3f(*tri->b, *intersect);
	t_vec3f			c2 = get_vec3f(*tri->c, *intersect);
	if (t > 0 && vec3f_dotprod(tri->normal_vec, vec3f_cross(edge0, c0)) > 0 &&
			vec3f_dotprod(tri->normal_vec, vec3f_cross(edge1, c1)) > 0 &&
			vec3f_dotprod(tri->normal_vec, vec3f_cross(edge2, c2)) > 0)
		return (true);
	return (false);
}

SDL_Color		*triangle_color(void *data, t_vec3f intersect)
{
	const t_triang	*triang = data;

	(void)intersect;
	return (triang->color);
}

t_vec3f			triangle_normalvec(void *data, t_vec3f intersect)
{
	const t_triang	*tri = data;
	t_vec3f			v1;
	t_vec3f			v2;
//	double			wrk;

	(void)intersect;
	v1 = get_vec3f(*tri->a, *tri->b);
	v2 = get_vec3f(*tri->b, *tri->c);
	/*wrk = sqrt(SQR(v1.y * v2.z - v1.z * v2.y) +
				SQR(v1.z * v2.x - v1.x * v2.z) +
				SQR(v1.x * v2.y - v1.y * v2.x));
	return ((t_vec3f){
		v1.y * v2.z - v1.z * v2.y / wrk,
		v1.z * v2.x - v1.x * v2.z / wrk,
		v1.x * v2.y - v1.y * v2.x / wrk
	});*/
	return (vec3f_cross(v1, v2));
}

void			triangle_cleanup(void *data)
{
	t_triang	*triang;

	triang = data;
	free(triang);
}