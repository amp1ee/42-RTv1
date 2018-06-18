#include "rtv1.h"

t_obj		*new_sphere(t_vec3f *center, float radius, SDL_Color *color)
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

bool		sphere_intersect(void *data, t_vec3f ray_start, t_vec3f ray,
				t_vec3f *intersect)
{
	const t_sphere	*sph = data;
	const t_vec3f	center = *sph->center;
	const float		r = sph->radius;
	const float		a = SQR(ray.x) + SQR(ray.y) + SQR(ray.z);
	const float		b = 2 * (ray.x * (ray_start.x - center.x))
					+ ray.y * (ray_start.y - center.y)
					+ ray.z * (ray_start.z - center.z);
	const float		c = SQR(center.x) + SQR(center.y) + SQR(center.z)
					+ SQR(ray_start.x) + SQR(ray_start.y) + SQR(ray_start.z)
					- 2 * (ray_start.x * center.x + ray_start.y * center.y
					+ ray_start.z * center.z) - SQR(r);
	const float		d = SQR(b) - 4 * a * c;
	(d>0)?printf("d=%.3f\n", d):0;
	if (d < 0)
		return (false);
	const float		t1 = (-b + sqrt(d)) / (2 * a);
	const float		t2 = (-b - sqrt(d)) / (2 * a);
	const float		t = (MIN(t1, t2) > EPSILON) ? MIN(t1, t2) : MAX(t1, t2);
	
	printf("t=%f\n", t);
	if (t < EPSILON)
		return (false);
	*intersect = (t_vec3f){	ray_start.x + t * ray.x,
							ray_start.y + t * ray.y,
							ray_start.z + t * ray.z};
	return (true);
}

SDL_Color	*sphere_color(void *data, t_vec3f intersect)
{
	const t_sphere	*sphere = data;

	(void)intersect;
	return (sphere->color);
}

t_vec3f		sphere_normalvec(void *data, t_vec3f intersect)
{
	const t_sphere	*sphere = data;
	t_vec3f			n;

	n = get_vec3f(*sphere->center, intersect);
	vec3f_normalize(&n);
	return (n);
}

void		sphere_cleanup(void *data)
{
	t_sphere *sphere;

	sphere = data;
	free(sphere);
}