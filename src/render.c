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

static inline t_vec3f
rot_vec_x(const t_vec3f p,
				const double sin_al,
				const double cos_al) {
	const double y = p[1] * cos_al - p[2] * sin_al;
	const double z = p[1] * sin_al + p[2] * cos_al;
	return (t_vec3f){p[0], y, z};
}

static inline t_vec3f
rot_vec_y(const t_vec3f p,
				const double sin_al,
				const double cos_al) {
	const double x = p[0] * cos_al - p[2] * sin_al;
	const double z = p[0] * sin_al + p[2] * cos_al;
	return (t_vec3f){x, p[1], z};
}

static inline t_vec3f
rot_vec_z(const t_vec3f p,
				const double sin_al,
				const double cos_al) {
	const double x = p[0] * cos_al - p[1] * sin_al;
	const double y = p[0] * sin_al + p[1] * cos_al;
	return (t_vec3f){x, y, p[2]};
}

double				shed_lights(t_main *m, t_vec3f P, t_vec3f N)
{
	int				j;
	t_light			*light;
	t_vec3f			light_dir;
	t_vec3f			L;
	double			k;

	k = 0.0;
	j = -1;
	while (++j < m->obj_num)
	{
		if ((m->objects[j])->type == LIGHT_SOURCE)
		{
			light = (t_light *)m->objects[j]->data;
			light_dir = get_vec3f(P, *(light->loc));
			L = (t_vec3f){
				light_dir[0] * -1,
				light_dir[1] * -1,
				light_dir[2] * -1
			};
			k += ((ALBEDO / 255) * M_PI) * light->brightness *
				(MAX(0.0f, -vec3f_dot(N, L)));
		}
	}
	return (k);
}

unsigned int		trace(t_main *m, t_vec3f rdir)
{
	SDL_Color		color;
	t_obj			*o;
	int				i;
	t_vec3f			P;
	t_vec3f			N;
	const t_cam		*cam = m->cam;

	vec3f_normalize(&rdir);
	rdir = rot_vec_y(rot_vec_z(rot_vec_x(rdir, cam->xsin, cam->xcos), cam->zsin,
										cam->zcos), cam->ysin, cam->ycos);
	color = /*(SDL_Color){255,255,255,255};//*/BGCOLOR;
	i = -1;
	while (++i < m->obj_num)
	{
		double k = 0.0f;
		o = m->objects[i];
		if (o->type != LIGHT_SOURCE && o->intersects(o->data, *(cam->loc), rdir, &P))
		{
			color = o->get_color(o->data, P);
			N = o->normal_vec(o->data, P);
			k = shed_lights(m, P, N);
			color.r *= k;
			color.g *= k;
			color.b *= k;
		}
	}
	return (color.r << 16 | color.g << 8 | color.b);
}

void				render(t_main *m)
{
	int				i, j;
	double			x, y;
	unsigned int	rgb;
	static unsigned int frames;
	//SDL_Color		*color;

	SDL_FillRect(m->screen, NULL, 0x000000);
	i = 0;
	while (i < W)
	{
		j = 0;
		while (j < H)
		{
			x = i / (double)W - 0.5;
			y = j / (double)H - 0.5;
			t_vec3f rdir = (t_vec3f){x * ASPECT, y, m->cam->focus};
			rgb = trace(m, rdir);
			set_pixel(m, i, j, rgb);
			j++;
		}
		i++;
	}
	printf(" Frame #%u\tCamera @ (%.2f, %.2f, %.2f)\n", ++frames, (*m->cam->loc)[0],
		   (*m->cam->loc)[1], (*m->cam->loc)[2]);
	SDL_UpdateWindowSurface(m->window);
}