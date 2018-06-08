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
	return (obj);
}

bool			triangle_intersect(void *data, t_vec3f ray_start, t_vec3f ray,
				t_vec3f *intersect)
{
	const t_triang	*tri = data;
	const double	scal_prod = vec4f_dotprod(tri->normal_vec, ray);
	if (fabs(scal_prod) < EPSILON)
		return (false);
	const float		k = -(tri->normal_vec[0] * ray_start[0]
						+ tri->normal_vec[1] * ray_start[1]
						+ tri->normal_vec[2] * ray_start[2] + tri->)
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
	double			wrk;

	(void)intersect;
	v1 = get_vec3f(*tri->b, *tri->a);
	v2 = get_vec3f(*tri->c, *tri->b);
	wrk = sqrt(SQR(v1.y * v2.z - v1.z * v2.y) +
				SQR(v1.z * v2.x - v1.x * v2.z) +
				SQR(v1.x * v2.y - v1.y * v2.x));
	return ((t_vec3f){
		v1.y * v2.z - v1.z * v2.y / wrk,
		v1.z * v2.x - v1.x * v2.z / wrk,
		v1.x * v2.y - v1.y * v2.x / wrk
	});
}

void			triangle_cleanup(void *data)
{
	t_triang	*triang;

	triang = data;
	free(triang);
}