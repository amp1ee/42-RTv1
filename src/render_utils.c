#include "rtv1.h"

t_v3				clamp(t_v3 color)
{
	color[0] = (color[0] > 255) ? 255 : color[0];
	color[1] = (color[1] > 255) ? 255 : color[1];
	color[2] = (color[2] > 255) ? 255 : color[2];
	return (color);
}

t_v3				color_lerp(t_v3 a, t_v3 b, double p)
{
	t_v3			new;

	new = v3_get(p * a[0] + (1.0 - p) * b[0],
				p * a[1] + (1.0 - p) * b[1],
				p * a[2] + (1.0 - p) * b[2]);
	return (clamp(new));
}

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

