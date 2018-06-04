#include "rtv1.h"

t_obj			*new_triangle(t_vec4f *a, t_vec4f *b, t_vec4f *c,
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

bool			triangle_intersect(void *data, t_vec4f ray_start, t_vec4f ray,
				t_vec4f *intersect)
{
	const t_triang	*tri = data;
	const double	scal_prod = vec4f_dotprod(tri->normal_vec, ray);
	if (fabs(scal_prod) < EPSILON)
		return (false);
	const float		k = -(tri->normal_vec[0] * ray_start[0]
						+ tri->normal_vec[1] * ray_start[1]
						+ tri->normal_vec[2] * ray_start[2] + tri->)
}

SDL_Color		*triangle_color(void *data, t_vec4f intersect)
{
	const t_triang	*triang = data;

	(void)intersect;
	return (triang->color);
}

t_vec4f			triangle_normalvec(void *data, t_vec4f intersect)
{
	const t_triang	*tri = data;
	t_vec4f			v1;
	t_vec4f			v2;
	int				i;
	double			wrk;

	(void)intersect;
	i = -1;
	while (++i < 3)
	{
		v1[i] = (*tri->a)[i] - (*tri->b)[i];
		v2[i] = (*tri->b)[i] - (*tri->c)[i];
	}
	wrk = sqrt(SQR(v1[1] * v2[2] - v1[2] * v2[1]) +
		SQR(v1[2] * v2[0] - v1[0] * v2[2]) +
		SQR(v1[0] * v2[1] - v1[1] * v2[0]));
	return ((t_vec4f){
		v1[1] * v2[2] - v1[2] * v2[1] / wrk,
		v1[2] * v2[0] - v1[0] * v2[2] / wrk,
		v1[0] * v2[1] - v1[1] * v2[0] / wrk,
		0.0
	});
}

void			triangle_cleanup(void *data)
{
	t_triang	*triang;

	triang = data;
	free(triang);
}