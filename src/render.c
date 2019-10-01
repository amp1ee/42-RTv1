#include "rtv1.h"

/*
**	Tracing algorhythm:
**
**	for (each x)
**		for (each y)
**		{
**			find rdir ->
**			min_dist = INF;
**			color = BG;
**			for (each object)
**			{
**				if (intersecting with ray)
**				{
**					find dist to obj;
**					if (dist < min_dist)
**					{
**						calculate diff_k;
**						color *= diff_k;
**						min_dist = dist;
**					}
**				}
**			}
**		}
*/

#define AMBIENT_COEF (0.1)

void				set_pixel(t_main *m, int x, int y, t_v3 color)
{
	const unsigned	bpp = m->screen->format->BytesPerPixel;
	unsigned char	*pixels;
	unsigned char	b;
	unsigned int	p;

	p = (255 << 24 | (int)color[0] << 16 | (int)color[1] << 8 | (int)color[2]);
	if (x < W && x >= 0 && y < H && y >= 0)
	{
		pixels = (unsigned char *)m->screen->pixels;
		b = -1;
		while (++b < bpp)
			pixels[bpp * (y * m->screen->w + x) + b] = (p >> (b << 3)) & 0xFF;
	}
}

t_matrix			init_matrix(t_v3 angle)
{
	t_matrix		m;

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

void				matrix_apply(t_v3 *vec, t_matrix m)
{
	t_v3			t;

	t = *vec;
	*vec = (t_v3){
		v3_dot(m.m[0], t),
		v3_dot(m.m[1], t),
		v3_dot(m.m[2], t)
	};
}

t_obj				*get_obstacle(t_main *m, t_v3 rdir, t_v3 *p, double t)
{
	t_obj			*o;
	t_obj			*closest;
	t_v3			lp;
	double			dist;
	int				i;

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

double				shed_lights(t_main *m, t_shedlight *l, t_trace t)
{
	l->diffuse_k = 0.0;
	l->diffuse_light = (t_v3){0, 0, 0};
	l->specular_light = (t_v3){0, 0, 0};
	l->j = -1;
	while (++l->j < m->obj_num)
	{
		if ((m->objects[l->j])->type == LIGHT_SOURCE)
		{
			l->light = (t_light *)m->objects[l->j]->data;
			l->light_dir = *(l->light->pos) - t.p;
			l->dist = v3_length(l->light_dir);
			v3_normalize(&(l->light_dir));
			l->atten = 1 + SQ(l->dist / 34.0);
			l->diffuse_k += MAX(0.0, v3_dot(t.n, l->light_dir)) / l->atten;
			if (l->diffuse_k > 1e-3)
			{
				l->spot = t.p + v3_multsc(l->light_dir, 1e-4);
				if (!(get_obstacle(m, l->light_dir, &l->spot, l->dist - 1e-4)))
					l->diffuse_light += v3_multsc(v3_get((l->light)->color.r,
					(l->light)->color.g, (l->light)->color.b), l->diffuse_k);
			}
		}
	}
	return (l->diffuse_k);
}

t_v3				trace(t_main *m, t_v3 rdir, int depth)
{
	t_trace			t;
	t_shedlight		l;
	t_obj			*o;

	t.color = BGCOLOR;
	t.t = INFINITY;
	o = get_obstacle(m, rdir, &t.p, t.t);
	if (o != NULL)
	{
		t.n = o->normal_vec(o->data, t.p);
		t.c = o->get_color(o->data, t.p);
		t.color = v3_get(t.c.r, t.c.g, t.c.b);
		l.ambient_light = v3_multsc(t.color, AMBIENT_COEF);
		t.k = shed_lights(m, &l, t);
		if (depth > 1)
		{
			t.refl = v3_reflected(-rdir, t.n);
			m->refl_point = t.p + v3_multsc(t.refl, 1e-4);
			t.color += trace(m, t.refl, depth - 1);
		}
		t.color *= v3_get(t.k, t.k, t.k);
		t.color += l.ambient_light;
	}
	return (t.color);
}

/*
**	static unsigned int frames;
**	printf(" Frame #%u\tCamera @ (%.2f, %.2f, %.2f)\n", ++frames,
**	(*m->cam->pos)[0], (*m->cam->pos)[1], (*m->cam->pos)[2]);
*/

void				render(t_main *m)
{
	int				i;
	int				j;
	double			x;
	double			y;

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
			x = i / (double)W - 0.5;
			y = j / (double)H - 0.5;
			m->rdir = (t_v3){x * ASPECT, y, m->cam->focus};
			v3_normalize(&(m->rdir));
			matrix_apply(&(m->rdir), m->cam->rot_mtx);
			m->refl_point = *(m->cam->pos);
			set_pixel(m, i, j, trace(m, m->rdir, m->recur_depth));
		}
	}
	SDL_UpdateWindowSurface(m->window);
}
