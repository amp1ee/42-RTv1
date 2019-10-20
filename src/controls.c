#include "rtv1.h"

#define KEY_ISMOVE(k)	(k == SDLK_w || k == SDLK_a || k == SDLK_s || \
						k == SDLK_d || k == SDLK_q || k == SDLK_e)
#define KEY_ISROT(k)	(k == SDLK_DOWN || k == SDLK_UP || k == SDLK_x || \
						k == SDLK_RIGHT || k == SDLK_LEFT || k == SDLK_z)
#define STEP			(50)
#define ANG				(8 * (M_PI) / 180.0)
#define DOT(a, b)		v3_dot((a), (b))

void				rtv1_quit(t_main *m)
{
	m->running = false;
	free_mem(m);
	SDL_PixelFormat* pixelFormat = m->screen->format;
	Uint32 pixelFormatEnum = pixelFormat->format;
	const char* surfacePixelFormatName = SDL_GetPixelFormatName(pixelFormatEnum);
	SDL_Log("The surface's pixelformat is %s", surfacePixelFormatName);
	SDL_FreeSurface(m->screen);
	SDL_DestroyWindow(m->window);
	ft_memdel((void **)&m);
	exit(0);
}

static inline void	cam_move(t_main *m, SDL_Keycode key)
{
	const t_v3		ray = m->cam->ray;
	const t_v3		perp_ray = v3_cross(m->cam->ray, UJ);
	t_cam			*cam;

	cam = m->cam;
	if (key == SDLK_w)
		cam->pos += v3_mult_scalar(v3_get(DOT(ray, UI),
								-DOT(ray, UJ), DOT(ray, UK)), STEP);
	else if (key == SDLK_s)
		cam->pos -= v3_mult_scalar(v3_get(DOT(ray, UI),
								-DOT(ray, UJ), DOT(ray, UK)), STEP);
	else if (key == SDLK_a)
		cam->pos -= v3_mult_scalar(v3_get(DOT(perp_ray, UI), 0,
								DOT(perp_ray, UK)), STEP);
	else if (key == SDLK_d)
		cam->pos += v3_mult_scalar(v3_get(DOT(perp_ray, UI), 0,
								DOT(perp_ray, UK)), STEP);
	else if (key == SDLK_q)
		(cam->pos)[1] -= STEP;
	else
		(cam->pos)[1] += STEP;
}

static inline void	cam_rotate(t_main *m, SDL_Keycode key)
{
	const t_v3		a = m->cam->angle;
	t_cam			*cam;

	cam = m->cam;
	if (key == SDLK_DOWN)
		cam->angle = v3_get(a[0] + ANG, a[1], a[2]);
	else if (key == SDLK_UP)
		cam->angle = v3_get(a[0] - ANG, a[1], a[2]);
	else if (key == SDLK_RIGHT)
		cam->angle = v3_get(a[0], (a[1] - ANG), a[2]);
	else if (key == SDLK_LEFT)
		cam->angle = v3_get(a[0], (a[1] + ANG), a[2]);
	else if (key == SDLK_x)
		cam->angle = v3_get(a[0], a[1], a[2] + ANG);
	else
		cam->angle = v3_get(a[0], a[1], a[2] - ANG);
}

void				handle_events(t_main *m, SDL_Event e)
{
	if (e.type == SDL_KEYDOWN)
	{
		if (KEY_ISROT(e.key.keysym.sym))
			cam_rotate(m, e.key.keysym.sym);
		else if (KEY_ISMOVE(e.key.keysym.sym))
			cam_move(m, e.key.keysym.sym);
		else if (e.key.keysym.sym == SDLK_ESCAPE)
			rtv1_quit(m);
	}
	else if (e.type == SDL_WINDOWEVENT &&
		e.window.event == SDL_WINDOWEVENT_CLOSE)
	{
		rtv1_quit(m);
	}
	else
		return ;
	render(m);
}