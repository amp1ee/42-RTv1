#include "rtv1.h"

double		vec3f_dot(t_vec3f a, t_vec3f b)
{
	double	dprod;

	dprod = a[0] * b[0] +
			a[1] * b[1] +
			a[2] * b[2];
	return (dprod);
}

double		vec3f_squared(t_vec3f v)
{
	double	res;

	res = vec3f_dot(v, v);
	return (res);
}

double		vec3f_length(t_vec3f vec)
{
	double	res;

	res = sqrt(vec3f_squared(vec));
	return (res);
}

t_vec3f		vec3f_cross(t_vec3f a, t_vec3f b)
{
	t_vec3f	xprod;

	xprod[0] = a[1] * b[2] - a[2] * b[1];
	xprod[1] = a[2] * b[0] - a[0] * b[2];
	xprod[2] = a[0] * b[1] - a[1] * b[0];
	return (xprod);
}

t_vec3f		vec3f_multsc(t_vec3f v, double scalar)
{
	t_vec3f	res;

	res = (t_vec3f){ v[0] * scalar, v[1] * scalar, v[2] * scalar };
	return (res);
}

void		vec3f_normalize(t_vec3f *vec)
{
	const double	vlen = vec3f_length(*vec);
	t_vec3f v = *vec;

	if (vlen <= EPSILON)
		return ;
	v[0] /= vlen;
	v[1] /= vlen;
	v[2] /= vlen;
	*vec = v;
}
