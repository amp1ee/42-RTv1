#include "rtv1.h"

t_obj		*new_sphere(t_vec4f *center, float radius, SDL_Color *color)
{
	t_sphere	*sph;
	t_obj		*obj;

	if (!(sph = (t_sphere *)malloc(sizeof(t_sphere))))
		return (NULL);
	sph->center = center;
	sph->radius = radius;
	sph->color = color;
	if (!(obj = (t_obj *)malloc(sizeof(t_obj))))
		return (NULL);
	obj->data = sph;
	obj->intersects = sphere_intersect;
	obj->get_color = sphere_color;
	obj->normal_vec = sphere_normalvec;
	obj->cleanup = sphere_cleanup;
	return (obj);
}

bool		sphere_intersect(void *data, t_vec4f ray_start, t_vec4f ray,
				t_vec4f *intersect)
{
	const t_sphere	*sph = data;
	const t_vec4f	center = *sph->center;
	const float		a = ray[0] * ray[0] + ray[1] * ray[1] + ray[2] * ray[2];
	const float		b = 2 * (ray[0] * (ray_start[0] - center[0]))
					+ ray[1] * (ray_start[1] - center[1])
					+ ray[2] * (ray_start[2] - center[2]);
	const float		c = SQR(center[0]) + SQR(center[1]) + SQR(center[2])
					+ SQR(ray_start[0]) + SQR(ray_start[1]) + SQR(ray_start[2])
					- 2 * (ray_start[0] * center[0] + ray_start[1] * center[1]
					+ ray_start[2] * center[2]);
	const float		d = SQR(b) - 4 * a * c;

	if (d < 0)
		return (false);
	const float		t1 = (-b + sqrt(d)) / (2 * a);
	const float		t2 = (-b + sqrt(d)) / (2 * a);
	const float		t = (MIN(t1, t2) > EPSILON) ? MIN(t1, t2) : MAX(t1, t2);
	if (t < EPSILON)
		return (false);
	*intersect = (t_vec4f){ray_start[0] + t * ray[0],
					ray_start[1] + t * ray[1],
					ray_start[2] + t * ray[2], 0};
	return (true);
}

SDL_Color	*sphere_color(void *data, t_vec4f intersect)
{
	const t_sphere	*sphere = data;

	(void)intersect;
	return (sphere->color);
}

t_vec4f		sphere_normalvec(void *data, t_vec4f intersect)
{
	const t_sphere	*sphere = data;
	t_vec4f			n;

	n = get_vec4f(*sphere->center, intersect);
	vec4f_normalize(&n);
	return (n);
}

void		sphere_cleanup(void *data)
{
	t_sphere *sphere;

	sphere = data;
	free(sphere);
}