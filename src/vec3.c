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

double		v3_squared(t_v3 v)
{
	double	res;

	res = v3_dot(v, v);
	return (res);
}

double		v3_length(t_v3 vec)
{
	double	res;

	res = sqrt(v3_squared(vec));
	return (res);
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
	t_v3	res;

	res = (t_v3){ v[0] * scalar, v[1] * scalar, v[2] * scalar };
	return (res);
}

void		v3_normalize(t_v3 *vec)
{
	const double	vlen = v3_length(*vec);
	t_v3 v = *vec;

	if (vlen <= EPSILON)
		return ;
	v[0] /= vlen;
	v[1] /= vlen;
	v[2] /= vlen;
	*vec = v;
}

t_v3		v3_reflected(t_v3 vec, t_v3 n)
{
	t_v3	refl;
	t_v3	norm;
	double	v;

	norm = n;
	v3_normalize(&norm);
	v = v3_dot(vec, norm);
	refl = v3_mult_scalar(norm, v + v) - vec;
	return (refl);
}
