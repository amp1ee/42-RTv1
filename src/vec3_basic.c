#include "rtv1.h"

t_v3		v3_get(double a, double b, double c)
{
	return ((t_v3){ a, b, c });
}

double		v3_dot(t_v3 a, t_v3 b)
{
	double	dprod;

	dprod = a[0] * b[0] +
			a[1] * b[1] +
			a[2] * b[2];
	return (dprod);
}

t_v3		v3_cross(t_v3 a, t_v3 b)
{
	t_v3	xprod;

	xprod[0] = a[1] * b[2] - a[2] * b[1];
	xprod[1] = a[2] * b[0] - a[0] * b[2];
	xprod[2] = a[0] * b[1] - a[1] * b[0];
	return (xprod);
}

t_v3		v3_mult_scalar(t_v3 v, double scalar)
{
	return ((t_v3){ v[0] * scalar, v[1] * scalar, v[2] * scalar });
}

void		v3_normalize(t_v3 *vec)
{
	double	vlen;
	t_v3	v;

	vlen = v3_length(*vec);
	if (vlen <= EPSILON)
		return ;
	v = *vec;
	v[0] /= vlen;
	v[1] /= vlen;
	v[2] /= vlen;
	*vec = v;
}
