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

void				render(t_main *m)
{
	int				i;
	int				x, y;
	unsigned int	rgb;
	t_obj			*o;
	SDL_Color		*color;
	t_vec3f			intersect;

	//ft_bzero(m->screen->pixels, W * H * m->bpp);
	SDL_FillRect(m->screen, NULL, 0x000000);
	y = 0;
	while (y < H)
	{
		x = 0;
		while (x < W)
		{
			i = -1;
			while (++i < OBJ)
			{
				o = m->objects[i];
				if (o->intersects(o->data, *m->camera, *m->ray, &intersect))
				{
					color = o->get_color(o->data, intersect);
					rgb = (color->r << 16) | (color->g << 8) | color->b;
					set_pixel(m, x, y, rgb);
				}
			}
			x++;
		}
		y++;
	}
	SDL_UpdateWindowSurface(m->window);
}