#include "rtv1.h"

t_vec3f				plane_normalvec(void *data, t_vec3f intersect)
{
	t_plane	*plane = (t_plane *)data;

	(void)intersect;
	vec3f_normalize(&(plane->normal));
	return (plane->normal);
}

/*
** (r(t) - pos) * n = 0
** (o + dt - pos) * n = 0
** d * n * t = (o - pos) * n
** t = ((o - pos) * n) / (d * n)
*/
/*
	float	t;
	float3 d = {plane.d.x, plane.d.y, plane.d.z};
	float3 p = {plane.p.x, plane.p.y, plane.p.z};

	d = fast_normalize(d);
	float3 oc = O - p;
	float k1 = dot(d, D);
	float k2 = dot(oc, d);
	if (k1 != 0.0f)
	{
		t = -k2 / k1;
		return (float2) {t, INFINITY};
	}
	return (float2) {INFINITY, INFINITY};
*/

bool				plane_intersect(void *data, t_vec3f eye, t_vec3f rdir,
										t_vec3f *intersect)
{
	const t_plane	*pln = data;
	const t_vec3f	pos = *pln->pos;
	const t_vec3f	dir = plane_normalvec(data, *intersect);
	double			t;

	t_vec3f			oc = pos - eye;
	double			a = vec3f_dot(rdir, dir);
	double			b = vec3f_dot(oc, dir);
	t = (b / a);
	if (fabs(a) <= EPSILON)
		return (false);
	*intersect = (t_vec3f){
		eye[0] + t * rdir[0],
		eye[1] + t * rdir[1],
		eye[2] + t * rdir[2]
	};
	return (t > 0);
}

SDL_Color			plane_color(void *data, t_vec3f intersect)
{
	const t_plane	*plane = (t_plane *)data;

	(void)intersect;
	return (plane->color);
}

void				plane_cleanup(void *data)
{
	t_plane			*plane;

	plane = (t_plane *)data;
	free(plane);
}

t_obj				*new_plane(t_vec3f *center, t_vec3f dir, double radius,
							SDL_Color color)
{
	t_plane			*plane;
	t_obj			*obj;

	if (!(plane = malloc(sizeof(*plane))))
		return (NULL);
	plane->pos = center;
	plane->normal = dir;
	(void)radius;
	plane->color = color;
	if (!(obj = malloc(sizeof(*obj))))
		return (NULL);
	obj->data = plane;
	obj->type = PLANE;
	obj->intersects = &plane_intersect;
	obj->get_color = &plane_color;
	obj->normal_vec = &plane_normalvec;
	obj->cleanup = &plane_cleanup;
	return (obj);
}