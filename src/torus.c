#include "rtv1.h"
#include <fcntl.h>

t_obj			*new_torus(t_vec3f *center, t_vec3f dir,
						double tor_rad, double tube_rad, SDL_Color color)
{
	t_torus		*tor;
	t_obj		*obj;

	if (!(tor = (t_torus *)malloc(sizeof(t_torus))))
		return (NULL);
	tor->radius = tor_rad;
	tor->tube_radius = tube_rad;
	tor->center = center;
	tor->dir = dir;
	vec3f_normalize(&(tor->dir));
	tor->color = color;
	if (!(obj = (t_obj *)malloc(sizeof(t_obj))))
		return (NULL);
	obj->type = TORUS;
	obj->data = tor;
	obj->get_color = torus_color;
	obj->normal_vec = torus_normalvec;
	obj->cleanup = torus_cleanup;
	obj->intersects = torus_intersect;
	return (obj);
}


static int		solve_quartic(complex double a, complex double b, complex double c,
							complex double d, complex double e, complex double roots[4])
{
	double a2 = a*a;
	double a3 = a*a2;
	double b2 = b*b;
	double b3 = b*b2;
	double b4 = b*b3;
	double c2 = c*c;
	double c3 = c*c2;
	double c4 = c*c3;
	double d2 = d*d;
	double d3 = d*d2;
	double d4 = d*d3;
	double e2 = e*e;
	double e3 = e*e2;
	double dsc, dsc0, dsc1;
	complex double S, Q;
	double D, P, R, p, q, phi;
	int		nroots = 4;

	P = 8*a*c - 3*b2;
	R = b3 + 8*d*a2 - 4*a*b*c;
	D = 64*a3*e - 16*a2*c2 + 16*a*b2*c - 16*a2*b*d - 3*b4;

	dsc = 256*a3*e3 - 192*a2*b*d*e2 - 128*a2*c2*e2 + 144*a2*c*d2*e - 27*a2*d4 +
	144*a*b2*c*e2 - 6*a*b2*d2*e - 80*a*b*c2*d*e + 18*a*b*c*d3 + 16*a*c4*e -
	4*a*c3*d2 - 27*b4*e2 + 18*b3*c*d*e - 4*b3*d3 - 4*b2*c3*e + b2*c2*d2;

	dsc0 = c2 - 3*b*d + 12*a*e;
	dsc1 = 2*c3 - 9*b*c*d + 27*b2*e + 27*a*d2 - 72*a*c*e;

	p = (8*a*c - 3*b2)/(8.0*a2);
	q = (b3 - 4*a*b*c + 8*a2*d)/(8.0*a3);
	Q = cpow((dsc1 + csqrt(-27*dsc))/2.0, 1/3.0);
	S = sqrt((-2*p)/3.0 + (Q + dsc0/Q)/(3*a))/2.0;
	if (dsc < -EPSILON)
	{
		nroots = 2;
	}
	else if (dsc > EPSILON)
	{
		if (P > 0.0f || D > 0.0f)
			return (0);
		phi = acos(dsc1/(2.0*sqrt(CB(dsc0))));
		S = sqrt(-(2*p)/3.0 + (2*sqrt(dsc0)*cos(phi/3.0))/(3*a))/2.0;
		nroots = 4;
	}
	else if (fabs(dsc) <= EPSILON)
	{
		if (P < 0.0f && D < 0.0f && fabs(dsc) > EPSILON)
			nroots = 4;
		else if (D > 0.0f || (P > 0.0f && (fabs(D) > EPSILON || fabs(R) > EPSILON)))
			nroots = 2;
		else if (fabs(dsc0) <= EPSILON && fabs(D) > EPSILON)
			nroots = 4;
		else if (fabs(D) <= EPSILON)
		{
			if (P < 0)
				nroots = 2;
			else if (P > 0 && fabs(R) <= EPSILON)
				nroots = 0;
			else if (fabs(dsc0) <= EPSILON)
			{
				roots[0] = -b/(4*a);
				return (1);
			}
		}
	}
	roots[0] = -b/(4*a) - S + sqrt(-4*S*S - 2*p + q/S)/2.0;
	roots[1] = -b/(4*a) + S + sqrt(-4*S*S - 2*p - q/S)/2.0;
	roots[2] = -b/(4*a) - S - sqrt(-4*S*S - 2*p + q/S)/2.0;
	roots[3] = -b/(4*a) + S - sqrt(-4*S*S - 2*p - q/S)/2.0;
	return nroots;
}

bool			torus_intersect(void *data, t_vec3f o, t_vec3f dir,
				t_vec3f *intersect)
{
	t_torus			*tor = data;
	double			a4, a3, a2, a1, a0;
	complex double	t[4];
	double			R = tor->radius;
	double			r = tor->tube_radius;
	double			R2 = SQ(R);
	double			r2 = SQ(r);

	t_vec3f			oc = get_vec3f(*tor->center, o);
	double	alph = 1.0;
	double	beta = vec3f_dot(dir, oc);
	double	gamm = vec3f_dot(oc, oc);
	double	delt = gamm + R2 - r2;
/*	double	v0 = vec3f_dot(dir, tor->dir);
	double	v1 = vec3f_dot(oc, tor->dir);*/

	a4 = 1.0;
	a3 = 4 * alph * beta;
	a2 = 2 * alph * delt + 4*SQ(beta) - 4*R2*(SQ(dir.x) + SQ(dir.y));
	a1 = 4 * beta * delt - 8*R2*(oc.x*dir.x + oc.y*dir.y);
	a0 = SQ(delt) - 4*R2*(SQ(oc.x) + SQ(oc.y));
	
	int nroots = solve_quartic(a4, a3, a2, a1, a0, t);
	if (nroots == 0)
		return (false);
	double t_min = INF;
	if (nroots == 2)
	{
		t_min = MIN(creal(t[1]), creal(t[3]));
	}
	else
		for (int i = 0; i < 4; ++i)
		{
			if (creal(t[i]) > EPSILON && creal(t[i]) < t_min)
				t_min = creal(t[i]);
		}

	*intersect = (t_vec3f){
		o.x + t_min * dir.x,
		o.y + t_min * dir.y,
		o.z + t_min * dir.z
	};
	return (t_min >= EPSILON);
}

SDL_Color		torus_color(void *data, t_vec3f intersect)
{
	const t_torus	*torus = data;

	(void)intersect;
	return (torus->color);
}

t_vec3f			torus_normalvec(void *data, t_vec3f P)
{
	const t_torus	*tor = data;
	t_vec3f			N;
	double	R = tor->radius;
	double	r = tor->tube_radius;
	t_vec3f C = *(tor->center);
	t_vec3f	V = tor->dir;
	t_vec3f	CP = get_vec3f(C, P);
	double	k = vec3f_dot(CP, V);
	if (k > r)
		k = r;
	t_vec3f	A = (t_vec3f) {
		P.x - V.x * k,
		P.y - V.y * k,
		P.z - V.z * k
	};
	double m = sqrt(SQ(r) - SQ(k));
	t_vec3f	AP = get_vec3f(A, P);
	t_vec3f	AC = get_vec3f(A, C);
	N = (t_vec3f) {
		AP.x - AC.x * m / (R + m),
		AP.y - AC.y * m / (R + m),
		AP.z - AC.z * m / (R + m)
	};
	vec3f_normalize(&N);
	//printf("N: %.2f %.2f %.2f\n", N.x, N.y, N.z);
	return (N);
}

void			torus_cleanup(void *data)
{
	t_torus	*torus;

	torus = data;

	free(torus);
}