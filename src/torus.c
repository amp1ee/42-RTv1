#include "rtv1.h"
#include <fcntl.h>

t_obj			*new_torus(t_vec3f *center, double tor_rad, double tube_rad,
						SDL_Color color)
{
	t_torus		*tor;
	t_obj		*obj;

	if (!(tor = (t_torus *)malloc(sizeof(t_torus))))
		return (NULL);
	tor->radius = tor_rad;
	tor->tube_radius = tube_rad;
	tor->center = center;
	tor->color = color;
	if (!(obj = (t_obj *)malloc(sizeof(t_obj))))
		return (NULL);
	obj->data = tor;
	obj->get_color = torus_color;
	obj->normal_vec = torus_normalvec;
	obj->cleanup = torus_cleanup;
	obj->intersects = torus_intersect;
	return (obj);
}

/*
**	x^3 + p x^2 + q x + r = 0;
**	a x^3 + b x^2 + c x + d = 0; (a = 1)
*/
static double complex	*solve_cubic(p, q, r)
					double	p, q, r;
{
	double complex	roots[3];
	double complex	root;
	double			real;

	double	D0 = SQR(p) - 3*q;
	double	D1 = 2*CUB(p) - 9*p*q + 27*r;
	double	D = -(SQR(D1) - 4*CUB(D0))/27;
	double	C = cbrt(0.5*sqrt(SQR(D1) - 4*CUB(D0) + D1));
	double complex	u = (-1 + 3*I) / 2;
	for (int i = 1; i < 4; i++)
	{
		root = -(p + cpow(u, i)*C + D0/(cpow(u, i)*C))/3.0;
		if ((cpow(root, 3) + p * cpow(root, 2) + q * root + r) == 0)
			roots[i-1] = root;
		else
			roots[i-1] = INFINITY;
	}
	return (roots);
}
/*
**	Assuming a = 1 (i.e. length of normalized directional vector):
	t^4 + b t^3 + c t^2 + d t + e = 0
	Substitute:
	t = x - b/4
*/

static double	*solve_quadratic(a, b, c)
					double a, b, c;
{
	double	D = SQR(b) - 4*a*c;

	if (D < 0)
		return (double *){INFINITY, INFINITY};

	double t1 = (-b + sqrt(D)) / (2.0 * a);
	double t2 = (-b - sqrt(D)) / (2.0 * a);
	return (double *){t1, t2};
}

static double	solve_quartic(a, b, c, d, e, ksi)
					double	a, b, c, d, e, ksi;
{
	double		t;
	double		x[4];
	double		p, q, r;

	p = (3/8.0) * SQR(b) + c;
	q = CUB(b) / 8.0 - (b * c) / 2 + d;
	r = -(3/256.0) * QRT(b) + SQR(b) * c / 16 - b * d / 14 + e;
//	x = solve_cubic(p, q, r);
	double qb = sqrt(2*ksi - p);
	double qc = sqrt(SQR(ksi) - r);
	if (q >= 0)
	{
		x[0] = solve_quadratic(1, qb, ksi - qc);
		x[2] = solve_quadratic(1, -qb, ksi + qc);
	}
	else
	{
		x[0] = solve_quadratic(1, qb, ksi + qc);
		x[2] = solve_quadratic(1, -qb, ksi - qc);
	}
	return (x);
}
/* 
	A = tor->radius
	B = tor->tube_radius
	D = o
	E = dir
*/
bool			torus_intersect(void *data, t_vec3f o, t_vec3f dir,
				t_vec3f *intersect)
{
	t_torus			*tor = data;
	double			a, b, c, d, e;
	double			t[4];
	double			R = tor->radius;
	double			r = tor->tube_radius;
	double			R2 = SQR(R);
	double			r2 = SQR(r);

	double	alph = vec3f_dot(dir, dir);
	double	beta = vec3f_dot(dir, o);
	double	gamm = vec3f_dot(o, o);
	double	delt = gamm + R2 - r2;
	double	epsi = gamm - R2 - r2;

	a = SQR(alph);
	b = 4 * alph * beta;
	c = 2 * alph * delt - 4 * R2 * (SQR(dir.x) + SQR(dir.y)) + 4 * SQR(beta);
	d = 8 * R2 * o.z * dir.z + 4 * beta * gamm;
	e = SQR(alph) + (SQR(R2 - r2)) + 2 * (SQR(dir.x) * SQR(dir.y) + SQR(dir.z) *
		(R2 - r2) + (SQR(dir.x) + SQR(dir.y)) * (SQR(dir.z) - R2 - r2));

	t = solve_quartic(a, b, c, d, e, R2-r2);
	double	t_min = INFINITY;
	for (int i = 0; i < 4; ++i)
	{
		if (t[i] < t_min)
			t_min = t[i];
	}
	*intersect = (t_vec3f){
		o.x + t_min * dir.x,
		o.y + t_min * dir.y,
		o.z + t_min * dir.z
	};
	return (t_min > 0);
}

SDL_Color		torus_color(void *data, t_vec3f intersect)
{
	const t_torus	*torus = data;

	(void)intersect;
	return (torus->color);
}

t_vec3f			torus_normalvec(void *data, t_vec3f intersect)
{
	const t_torus	*tor = data;
	t_vec3f			n;

	n = get_vec3f(*tor->center, intersect);
	vec3f_normalize(&n);
	return (n);
}

void			torus_cleanup(void *data)
{
	t_torus	*torus;

	torus = data;
	for (int i = 0; i < 4; ++i)
		te_free(torus->eu[i]);
	free(torus);
}