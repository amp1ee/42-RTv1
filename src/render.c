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
	m.m[1] = (t_vec3f){m.sxcz * m.sy - m.cxsz, m.sxsz * m.sy + m.cxcz, m.sx * m.cy};
	m.m[2] = (t_vec3f){m.cxcz * m.sy + m.sxsz, m.cxsz * m.sy - m.sxcz, m.cx * m.cy};
	return (m);
}

void				matrix_apply(t_vec3f *vec, t_matrix m)
{
	t_vec3f			t;

	t = *vec;
	*vec = (t_vec3f){ vec3f_dot(m.m[0], t), vec3f_dot(m.m[1], t), vec3f_dot(m.m[2], t) };
}

double				shed_lights(t_main *m, t_vec3f P, t_vec3f N)
{
	int				j;
	t_light			*light;
	t_vec3f			light_dir;
	double			diffuse_k;
	double			atten;
	double			dist;

	diffuse_k = 0.0;
	j = -1;
	while (++j < m->obj_num)
	{
		if ((m->objects[j])->type == LIGHT_SOURCE)
		{
			light = (t_light *)m->objects[j]->data;
			light_dir = *(light->loc) - P;
			dist = vec3f_length(light_dir);
			vec3f_normalize(&light_dir);
			atten = 1 + SQ(dist / 34.0);
			diffuse_k += MAX(0.0, vec3f_dot(N, light_dir)) / atten;
/*			if (diffuse_k > 1e-3)
			{
				diffuse_light += light->color * diffuse_k;
			}*/
		}
	}
	return (diffuse_k);
}

unsigned int		trace(t_main *m, t_vec3f rdir)
{
	SDL_Color		color;
	t_obj			*o;
	int				i;
	t_vec3f			P;
	t_vec3f			N;
	double			t = INFINITY;
	const t_cam		*cam = m->cam;

	color = BGCOLOR;
	i = -1;
	while (++i < m->obj_num)
	{
		double k = 0.0f;
		o = m->objects[i];
		if (o->type != LIGHT_SOURCE && o->intersects(o->data, *(cam->loc), rdir, &P))
		{
			if (vec3f_length(P - *(cam->loc)) < t)
			{
				N = o->normal_vec(o->data, P);
				color = o->get_color(o->data, P);
				k = shed_lights(m, P, N);
				color.r *= k;
				color.g *= k;
				color.b *= k;
				t = vec3f_length(P - *(cam->loc));
			}
		}
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
	//printf(" Frame #%u\tCamera @ (%.2f, %.2f, %.2f)\n", ++frames, (*m->cam->loc)[0],
	//	   (*m->cam->loc)[1], (*m->cam->loc)[2]);
	SDL_UpdateWindowSurface(m->window);
}