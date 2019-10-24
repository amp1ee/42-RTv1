#include "rtv1.h"

#define AMBIENT_COEF	(0.07)
#define SPEC_SMOOTHNESS	32

t_obj					*get_obstacle(t_main *m, t_v3 rdir, t_v3 *p, double t)
{
	t_obj				*o;
	t_obj				*closest;
	t_v3				lp;
	double				dist;
	int					i;

	closest = NULL;
	i = -1;
	while (++i < m->obj_num)
	{
		o = m->objects[i];
		if (o->type != LIGHT_SOURCE &&
			o->intersects(o->data, m->refl_point, rdir, &lp))
		{
			dist = v3_length(lp - m->refl_point);
			if (dist < t)
			{
				*p = lp;
				t = dist;
				closest = o;
			}
		}
	}
	return (closest);
}

static inline t_v3		spec_light(t_main *m, t_shedlight *l, t_trace t)
{
	const t_v3			lrgb = l->light->color;
	t_v3				spec_rgb;
	t_v3				h;
	double				dot;

	h = l->light_dir - m->rdir;
	v3_normalize(&h);
	dot = v3_dot(t.n, h);
	l->spec_k = l->light->brightness * 0.7 * pow(MAX(0, dot), SPEC_SMOOTHNESS);
	spec_rgb = color_lerp(t.color, lrgb, 0.5);
	return (v3_mult_scalar(spec_rgb, l->spec_k));
}

static inline double	shed_lights(t_main *m, t_shedlight *l, t_trace t)
{
	t_trace				tm;

	ft_memcpy(&tm, &t, sizeof(tm));
	l->spec_light = v3_get(0, 0, 0);
	l->diffuse_k = 0.0;
	l->j = -1;
	while (++l->j < m->obj_num)
	{
		if ((m->objects[l->j])->type == LIGHT_SOURCE)
		{
			l->light = (t_light *)m->objects[l->j]->data;
			l->light_dir = l->light->pos - t.p;
			l->dist = v3_length(l->light_dir);
			v3_normalize(&(l->light_dir));
			l->spot = t.p + v3_mult_scalar(l->light_dir, EPSILON);
			if (!(get_obstacle(m, l->light_dir, &l->spot, l->dist - EPSILON)))
			{
				l->atten = 1 + SQ(l->dist / 34.0);
				l->diffuse_k += l->light->brightness *
							(MAX(0.0, v3_dot(t.n, l->light_dir))) / l->atten;
				l->spec_light += spec_light(m , l, tm);
			}
		}
	}
	return (l->diffuse_k);
}

t_v3					trace(t_main *m, t_v3 rdir, int depth)
{
	t_trace				t;
	t_shedlight			*l;
	t_obj				*o;

	if (!(l = malloc(sizeof(t_shedlight))))
		return (v3_get(0, 0, 0));
	t.color = BGCOLOR;
	t.t = INFINITY;
	if ((o = get_obstacle(m, rdir, &t.p, t.t)) != NULL)
	{
		t.n = o->normal_vec(o->data, t.p);
		t.color = o->get_color(o->data, t.p);
		l->ambi_light = v3_mult_scalar(t.color, AMBIENT_COEF);
		m->refl_point = t.p + v3_mult_scalar(t.n, EPSILON);
		t.k = shed_lights(m, l, t);
		if (depth > 1)
		{
			t.refl = v3_reflected(-rdir, t.n);
			m->refl_point = t.p + v3_mult_scalar(t.refl, EPSILON);
			t.color += trace(m, t.refl, depth - 1);
		}
		t.color = v3_mult_scalar(t.color + l->spec_light, t.k) + l->ambi_light;
	}
	ft_memdel((void **)&l);
	return (t.color);
}

#include <time.h>

void					render(t_main *m)
{
	int					i;
	int					j;
	double				x;
	double				y;

	struct timespec	s, e;
	clock_gettime(CLOCK_MONOTONIC_RAW, &s);

	SDL_FillRect(m->screen, NULL, 0x000000);
	m->cam->rot_mtx = init_matrix(m->cam->angle);
	m->cam->ray = (t_v3){ 0, 0, m->cam->focus };
	matrix_apply(&(m->cam->ray), m->cam->rot_mtx);
	j = -1;
	while (++j < H)
	{
		i = -1;
		while (++i < W)
		{
			x = (unsigned)i / (double)W - 0.5;
			y = (unsigned)j / (double)H - 0.5;
			m->rdir = (t_v3){x * ASPECT, y, m->cam->focus};
			v3_normalize(&(m->rdir));
			matrix_apply(&(m->rdir), m->cam->rot_mtx);
			m->refl_point = m->cam->pos;
			set_pixel(m, i, j, clamp(trace(m, m->rdir, m->recur_depth)));
		}
	}

	clock_gettime(CLOCK_MONOTONIC_RAW, &e);
	double elapsed_t = (e.tv_sec - s.tv_sec) + (e.tv_nsec - s.tv_nsec) / 1e9;
	printf("Frame rendered in %f seconds\n", elapsed_t);

	SDL_UpdateWindowSurface(m->window);
}
