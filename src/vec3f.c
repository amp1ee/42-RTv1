#include "rtv1.h"

double		vec3f_length(t_vec3f vec)
{
	double	res;

	res = sqrt(SQR(vec.x) + SQR(vec.y) + SQR(vec.z));
	return (res);
}

t_vec3f		vec3f_cross(t_vec3f a, t_vec3f b)
{
	t_vec3f	xprod;

	xprod.x = a.y * b.z - a.z * b.y;
	xprod.y = a.z * b.x - a.x * b.z;
	xprod.z = a.x * b.y - a.y * b.x;
	return (xprod);
}

double		vec3f_dot(t_vec3f a, t_vec3f b)
{
	double	dprod;

	dprod = a.x * b.x +
			a.y * b.y +
			a.z * b.z;
	return (dprod);
}

t_vec3f		get_vec3f(t_vec3f p0, t_vec3f p1)
{
	t_vec3f	v;

	v = (t_vec3f){
		p1.x - p0.x,
		p1.y - p0.y,
		p1.z - p0.z
	};
	return (v);
}

void		vec3f_normalize(t_vec3f *vec)
{
	const double	vlen = vec3f_length(*vec);
	
	vec->x /= vlen;
	vec->y /= vlen;
	vec->z /= vlen;
}
