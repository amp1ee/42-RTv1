#include "rtv1.h"

bool		sphere_intersect(void *data, t_vec3f ray_start, t_vec3f ray,
				t_vec3f *intersect)
{
	const t_sphere	*sph = data;
	t_vec3f			k = ray_start - *sph->pos;

	double b,c;
	b = vec3f_dot(ray, k);
	c = vec3f_squared(k) - SQ(sph->radius);
	double d = SQ(b) - c;
	if (d < 0)
		return (false);
	double sqrtd = sqrt(d);
	const double		t1 = (-b - sqrtd);
	const double		t2 = (-b + sqrtd);
	const double		t = (MIN(t1, t2) >= 0) ? MIN(t1, t2) : MAX(t1, t2);
	*intersect = (t_vec3f){	ray_start[0] + t * ray[0],
							ray_start[1] + t * ray[1],
							ray_start[2] + t * ray[2]
						};
	return (t > 0);
}

SDL_Color	sphere_color(void *data, t_vec3f intersect)
{
	const t_sphere	*sphere = data;

	(void)intersect;
	return (sphere->color);
}

t_vec3f		sphere_normalvec(void *data, t_vec3f hit)
{
	const t_sphere	*sphere = data;
	t_vec3f			n;

	n = hit - *(sphere->pos);
	vec3f_normalize(&n);
	return (n);
}

void		sphere_cleanup(void *data)
{
	t_sphere *sphere;

	sphere = data;
	ft_memdel((void **)&(sphere->pos));
	ft_memdel((void **)&sphere);
}

t_obj		*new_sphere(t_vec3f *pos, t_vec3f dir, double radius, SDL_Color color)
{
	t_sphere	*sph;
	t_obj		*obj;

	(void)dir;
	if (!(sph = malloc(sizeof(t_sphere))))
		return (NULL);
	sph->pos = pos;
	//printf("p.x: %f, p.y: %f, p.z: %f\n", (*pos)[0], (*pos)[1], (*pos)[2]);
	sph->radius = radius;
	//printf("%f\n", sph->radius);
	sph->color = color;
	//printf("c.r: %u, c.g: %u, c.b: %u, c.a: %u\n", color.r, color.g, color.b, color.a);
	if (!(obj = malloc(sizeof(t_obj))))
		return (NULL);
	obj->type = SPHERE;
	obj->data = sph;
	obj->intersects = &sphere_intersect;
	obj->get_color = &sphere_color;
	obj->normal_vec = &sphere_normalvec;
	obj->cleanup = &sphere_cleanup;
	return (obj);
}