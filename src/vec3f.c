#include "rtv1.h"

double		vec3f_length(t_vec3f vec)
{
	double	res;

	res = sqrt(SQ(vec[0]) + SQ(vec[1]) + SQ(vec[2]));
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

double		vec3f_dot(t_vec3f a, t_vec3f b)
{
	double	dprod;

	dprod = a[0] * b[0] +
			a[1] * b[1] +
			a[2] * b[2];
	return (dprod);
}

t_vec3f		get_vec3f(t_vec3f p0, t_vec3f p1)
{
	t_vec3f	v;

	v = (t_vec3f){
		p1[0] - p0[0],
		p1[1] - p0[1],
		p1[2] - p0[2]
	};
	return (v);
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
