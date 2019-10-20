#include "rtv1.h"

double			v3_squared(t_v3 v)
{
	return (v3_dot(v, v));
}

double			v3_length(t_v3 vec)
{
	return (sqrt(v3_squared(vec)));
}

t_v3			v3_reflected(t_v3 vec, t_v3 n)
{
	t_v3		refl;
	t_v3		norm;
	double		v;

	norm = n;
	v3_normalize(&norm);
	v = v3_dot(vec, norm);
	refl = v3_mult_scalar(norm, v + v) - vec;
	return (refl);
}

t_matrix		init_matrix(t_v3 angle)
{
	t_matrix	m;

	m.cx = cos(angle[0]);
	m.cy = cos(angle[1]);
	m.cz = cos(angle[2]);
	m.sx = sin(angle[0]);
	m.sy = sin(angle[1]);
	m.sz = sin(angle[2]);
	m.sxsz = m.sx * m.sz;
	m.cxsz = m.cx * m.sz;
	m.cxcz = m.cx * m.cz;
	m.sxcz = m.sx * m.cz;
	m.m[0] = (t_v3){m.cy * m.cz, m.cy * m.sz, -m.sy };
	m.m[1] = (t_v3){m.sxcz * m.sy - m.cxsz,
		m.sxsz * m.sy + m.cxcz, m.sx * m.cy};
	m.m[2] = (t_v3){m.cxcz * m.sy + m.sxsz,
		m.cxsz * m.sy - m.sxcz, m.cx * m.cy};
	return (m);
}

void			matrix_apply(t_v3 *vec, t_matrix m)
{
	t_v3		t;

	t = *vec;
	*vec = (t_v3){
		v3_dot(m.m[0], t),
		v3_dot(m.m[1], t),
		v3_dot(m.m[2], t)
	};
}
