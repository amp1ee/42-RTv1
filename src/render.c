#include "rtv1.h"

void				set_pixel(t_main *m, int x, int y, unsigned int p)
{
	unsigned char	*pix;
	unsigned char	b;

	if (x < W && x >= 0 && y < H && y >= 0)
	{
		pix = (unsigned char *)m->screen->pixels;
		b = -1;
		while (++b < m->bpp)
			pix[m->bpp * (y * m->screen->w + x) + b] = (p >> (b << 3)) & 0xFF;
	}
}

t_matrix			init_matrix(t_vec3f angle)
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
	m.m[0] = (t_vec3f){m.cy * m.cz, m.cy * m.sz, -m.sy };
	m.m[1] = (t_vec3f){m.sxcz * m.sy - m.cxsz,
		m.sxsz * m.sy + m.cxcz, m.sx * m.cy};
	m.m[2] = (t_vec3f){m.cxcz * m.sy + m.sxsz,
		m.cxsz * m.sy - m.sxcz, m.cx * m.cy};
	return (m);
}

void				matrix_apply(t_vec3f *vec, t_matrix m)
{
	t_vec3f			t;

	t = *vec;
	*vec = (t_vec3f){
		vec3f_dot(m.m[0], t),
		vec3f_dot(m.m[1], t),
		vec3f_dot(m.m[2], t)
	};
}

t_obj				*get_obstacle(t_main *m, t_vec3f rdir, t_vec3f *P, double t)
{
	t_obj			*o;
	t_obj			*closest;
	t_vec3f			lP;
	double			dist;
	int				i;

	closest = NULL;
	i = -1;
	while (++i < m->obj_num)
	{
		o = m->objects[i];
		if (o->type != LIGHT_SOURCE &&
			o->intersects(o->data, *(m->cam->pos), rdir, &lP))
		{
			dist = vec3f_length(lP - *(m->cam->pos));
			if (dist < t)
			{
				*P = lP;
				t = dist;
				closest = o;
			}
		}
	}
	return (closest);
}

double				shed_lights(t_main *m, t_vec3f P, t_vec3f N)
{
	int				j;
	t_light			*light;
	t_vec3f			light_dir;
	t_vec3f			diffuse_light;
	t_vec3f			specular_light;
	double			diffuse_k;
	double			atten;
	double			dist;

	diffuse_k = 0.0;
	diffuse_light = (t_vec3f){0, 0, 0};
	specular_light = (t_vec3f){0, 0, 0};
	j = -1;
	while (++j < m->obj_num)
	{
		if ((m->objects[j])->type == LIGHT_SOURCE)
		{
			light = (t_light *)m->objects[j]->data;
			light_dir = *(light->pos) - P;
			dist = vec3f_length(light_dir);
			vec3f_normalize(&light_dir);
			atten = 1 + SQ(dist / 34.0);
			diffuse_k += MAX(0.0, vec3f_dot(N, light_dir)) / atten;
			if (diffuse_k > 1e-3)
			{
				double shadow_dist = dist - 1e-4;
				t_vec3f lp = P + vec3f_multsc(light_dir, 1e-4);
				if ((get_obstacle(m, light_dir, &lp, shadow_dist)) == NULL)
					diffuse_light += vec3f_multsc(vec3f_get(light->color.r,
						light->color.g, light->color.b), diffuse_k);
			}
		}
	}
	return (diffuse_k);
}

/*
*	for (each x)
		for (each y)
		{
			find rdir ->
			min_dist = INF;
			color = BG;
			for (each object)
			{
				if (intersecting with ray)
				{
					find dist to obj;
					if (dist < min_dist)
					{
						calculate diff_k;
						color *= diff_k;
						min_dist = dist;
					}
				}
			}
		}
*/
unsigned int		trace(t_main *m, t_vec3f rdir)
{
	SDL_Color		color;
	t_obj			*o;
	t_vec3f			P;
	t_vec3f			N;
	double			t;

	color = BGCOLOR;
	t = INFINITY;
	o = get_obstacle(m, rdir, &P, t);
	if (o != NULL)
	{
		N = o->normal_vec(o->data, P);
		color = o->get_color(o->data, P);
		double k = shed_lights(m, P, N);
		color.r *= k;
		color.g *= k;
		color.b *= k;
	}
	return (color.r << 16 | color.g << 8 | color.b);
}

void				render(t_main *m)
{
	int				i, j;
	double			x, y;
	unsigned int	rgb;
	//static unsigned int frames;

	SDL_FillRect(m->screen, NULL, 0x000000);
	m->cam->rot_mtx = init_matrix(m->cam->angle);
	// Apply rotation to move in eye direction
	m->cam->ray = (t_vec3f){ 0, 0, m->cam->focus };
	matrix_apply(&(m->cam->ray), m->cam->rot_mtx);
	j = 0;
	while (j < H)
	{
		i = 0;
		while (i < W)
		{
			x = i / (double)W - 0.5;
			y = j / (double)H - 0.5;
			t_vec3f rdir = (t_vec3f){x * ASPECT, y, m->cam->focus};
			vec3f_normalize(&rdir);
			matrix_apply(&rdir, m->cam->rot_mtx);
			rgb = trace(m, rdir);
			set_pixel(m, i, j, rgb);
			i++;
		}
		j++;
	}
	//printf(" Frame #%u\tCamera @ (%.2f, %.2f, %.2f)\n", ++frames, (*m->cam->pos)[0],
	//	   (*m->cam->pos)[1], (*m->cam->pos)[2]);
	SDL_UpdateWindowSurface(m->window);
}