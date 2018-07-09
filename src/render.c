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
				const float sin_al,
				const float cos_al) {
	const float y = p.y * cos_al - p.z * sin_al;
	const float z = p.y * sin_al + p.z * cos_al;
	return (t_vec3f){p.x, y, z};
}

static inline t_vec3f
rot_vec_y(const t_vec3f p,
				const float sin_al,
				const float cos_al) {
	const float x = p.x * cos_al - p.z * sin_al;
	const float z = p.x * sin_al + p.z * cos_al;
	return (t_vec3f){x, p.y, z};
}

static inline t_vec3f
rot_vec_z(const t_vec3f p,
				const float sin_al,
				const float cos_al) {
	const float x = p.x * cos_al - p.y * sin_al;
	const float y = p.x * sin_al + p.y * cos_al;
	return (t_vec3f){x, y, p.z};
}

unsigned int		trace(t_main *m, t_vec3f ray)
{
	t_vec3f 		r_vec;
	SDL_Color		color;
	t_obj			*o;
	int				i;
	t_vec3f			intersect;
	const t_cam		*cam = m->cam;

	vec3f_normalize(&ray);
	r_vec = rot_vec_x(ray, cam->xsin, cam->xcos);
	r_vec = rot_vec_z(r_vec, cam->zsin, cam->zcos);
	r_vec = rot_vec_y(r_vec, cam->ysin, cam->ycos);
	color = (SDL_Color){255,192,128,255};
	i = -1;
	while (++i < OBJ)
	{
		o = m->objects[i];
		if (o->intersects(o->data, *cam->loc, r_vec, &intersect))
		{
			t_vec3f norm = o->normal_vec(o->data, intersect);
			color = o->get_color(o->data, intersect);
			int j = -1;
			while (++j < LIGHT)
			{
				t_vec3f light_dir = get_vec3f(intersect, *m->lights[j]->loc);
				t_vec3f L = (t_vec3f){
					light_dir.x * -1,
					light_dir.y * -1,
					light_dir.z * -1
				};
				/*0.18 == object's average albedo)*/
				float k = ((0.18 / 255)*M_PI) * m->lights[j]->brightness *
					(MAX(0.0f, -vec3f_dotprod(norm, L)));
				color.r *= k;
				color.g *= k;
				color.b *= k;
			}
		}
	}
	return (color.r << 16 | color.g << 8 | color.b);
}

void				render(t_main *m)
{
	int				i, j;
	float			x, y;
	unsigned int	rgb;
	//SDL_Color		*color;

	SDL_FillRect(m->screen, NULL, 0x000000);
	i = 0;
	while (i < W)
	{
		j = 0;
		while (j < H)
		{
			x = (i - (W + m->cam->loc->x) / 2.0);
			y = (j - (H + m->cam->loc->y) / 2.0);
			t_vec3f ray = (t_vec3f){x, y, 320};
			rgb = trace(m, ray);
			set_pixel(m, i, j, rgb);
			j++;
		}
		i++;
	}
	SDL_UpdateWindowSurface(m->window);
}