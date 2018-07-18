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


static void		solve_quartic(double a, double b, double c,
							double d, double e, double roots[4])
{
		(void)a;	
	double complex b2 = b * b;
	double complex b3 = b * b2;
	double complex b4 = b2 * b2;

	double complex alpha = (-3.0/8.0)*b2 + c;
	double complex beta  = b3/8.0 - b*c/2.0 + d;
	double complex gamma = (-3.0/256.0)*b4 + b2*c/16.0 - b*d/4.0 + e;

	double complex alpha2 = alpha * alpha;
	double complex t = -b / 4.0;

	if (cabs(beta) <= EPSILON)
	{
		double complex rad = csqrt(alpha2 - 4.0*gamma);
		double complex r1 = csqrt((-alpha + rad) / 2.0);
		double complex r2 = csqrt((-alpha - rad) / 2.0);

		roots[0] = t + r1;
		roots[1] = t - r1;
		roots[2] = t + r2;
		roots[3] = t - r2;
	}
	else
	{
		double complex alpha3 = alpha * alpha2;
		double complex P = -(alpha2/12.0 + gamma);
		double complex Q = -alpha3/108.0 + alpha*gamma/3.0 - beta*beta/8.0;
		double complex R = -Q/2.0 + csqrt(Q*Q/4.0 + P*P*P/27.0);
		double complex U = pow(R, (1.0/3.0));
		double complex y = (-5.0/6.0)*alpha + U;
		if (cabs(U) <= EPSILON)
		{
			y -= pow(Q, (1.0/3.0));
		}
		else
		{
			y -= P/(3.0 * U);
		}
		double complex w = csqrt(alpha + 2.0*y);
		double complex r1 = csqrt(-(3.0*alpha + 2.0*y + 2.0*beta/w));
		double complex r2 = csqrt(-(3.0*alpha + 2.0*y - 2.0*beta/w));

		roots[0] = t + ( w - r1)/2.0;
		roots[1] = t + ( w + r1)/2.0;
		roots[2] = t + (-w - r2)/2.0;
		roots[3] = t + (-w + r2)/2.0;
	}
}
/* 
	A = tor->radius
	B = tor->tube_radius
	D = o
	E = dir
*/
// /double	epsi = gamm - R2 - r2;
//	double	alph = vec3f_dot(dir, dir) = 1;
//	a = SQR(alph) = 1;
//	c = 2 * alph * delt - 4 * R2 * (SQR(dir.x) + SQR(dir.y)) + 4 * SQR(beta);
//	d = 8 * R2 * o.z * dir.z + 4 * beta * epsi;
/*	e = SQR(alph) + (SQR(R2 - r2)) + 2 * (SQR(dir.x) * SQR(dir.y) + SQR(dir.z) *
		(R2 - r2) + (SQR(dir.x) + SQR(dir.y)) * (SQR(dir.z) - R2 - r2));*/
bool			torus_intersect(void *data, t_vec3f o, t_vec3f dir,
				t_vec3f *intersect)
{
	t_torus			*tor = data;
	double			a4, a3, a2, a1, a0;
	double			t[4];
	double			R = tor->radius;
	double			r = tor->tube_radius;
	double			R2 = SQR(R);
	double			r2 = SQR(r);

	t_vec3f			oc = get_vec3f(*tor->center, o);
	double	alph = 1.0;
	double	beta = vec3f_dot(dir, oc);
	double	gamm = vec3f_dot(oc, oc);
	double	delt = gamm + R2 - r2;
	//double	epsi = gamm - R2 - r2;

	a4 = 1.0;
	a3 = 4 * alph * beta;
	a2 = 2 * alph * delt + 4*SQR(beta) - 4*R2*(SQR(dir.x) + SQR(dir.y));
	a1 = 4 * beta * delt - 8*R2*(oc.x*dir.x + oc.y*dir.y);
	a0 = SQR(delt) - 4*R2*(SQR(oc.x) + SQR(oc.y));
	
	solve_quartic(a4, a3, a2, a1, a0, t);
	double m_len = INF;
	double t_min;
	for (int i = 0; i < 4; ++i)
	{
		t_vec3f p = (t_vec3f){
			o.x + t[i] * dir.x,
			o.y + t[i] * dir.y,
			o.z + t[i] * dir.z
		};
		double	len = vec3f_dot(p, p);
		if (len < m_len)
		{
			m_len = len;
			t_min = t[i];
		}
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

	free(torus);
}