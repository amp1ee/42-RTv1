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
	t_vec3f			k = get_vec3f(center, ray_start);
	float			b = vec3f_dotprod(k, ray);
	float			c = vec3f_dotprod(k, k) - SQR(r);
	float			d = SQR(b) - c;
	// (d>0)?printf("d=%.3f\n", d):0;
	if (d < 0)
		return (false);
	float sqrtd = sqrt(d);
	const float		t1 = (-b + sqrtd);
	const float		t2 = (-b - sqrtd);
	const float		t = (MIN(t1, t2) >= 0) ? MIN(t1, t2) : MAX(t1, t2);
	
	//printf("t=%f\n", t);
	//if (t < EPSILON)
	//	return (false);
	*intersect = (t_vec3f){	ray_start.x + t * ray.x,
							ray_start.y + t * ray.y,
							ray_start.z + t * ray.z};
	return (t > 0);
}

SDL_Color	*sphere_color(void *data, t_vec3f intersect)
{
	const t_sphere	*sphere = data;
	const SDL_Color	*scol = sphere->color;
	SDL_Color		*col;
	const float		dist = vec3f_length(intersect) / 64.0;

	if (dist >= 0.0)
	{
		col = malloc(sizeof(SDL_Color));
		col->r = scol->r * dist;
		col->g = scol->g * dist;
		col->b = scol->b * dist;
		col->a = scol->a;
		return (col);
	}
	return (sphere->color);
}

t_vec3f		*sphere_normalvec(void *data, t_vec3f intersect)
{
	const t_sphere	*sphere = data;
	t_vec3f			n;
	t_vec3f			*v;

	n = get_vec3f(*sphere->center, intersect);
	v = malloc(sizeof(t_vec3f));
	v->x = n.x;
	v->y = n.y;
	v->z = n.z;
	vec3f_normalize(v);
	return (v);
}

void		sphere_cleanup(void *data)
{
	t_sphere *sphere;

	sphere = data;
	free(sphere);
}