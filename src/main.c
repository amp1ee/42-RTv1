#include "rtv1.h"

void	game_quit(t_main *m)
{
	m->running = false;
	SDL_PixelFormat* pixelFormat = m->screen->format;
	Uint32 pixelFormatEnum = pixelFormat->format;
	const char* surfacePixelFormatName = SDL_GetPixelFormatName(pixelFormatEnum);
	SDL_Log("The surface's pixelformat is %s", surfacePixelFormatName);
	SDL_DestroyWindow(m->window);
	SDL_Quit();
}

void	handle_events(t_main *m)
{
	SDL_Event	e;

	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE)
			game_quit(m);
		else if (e.key.keysym.sym == SDLK_LEFT)
			m->cam->xang += 2 * M_PI / 180.0;
		else if (e.key.keysym.sym == SDLK_RIGHT)
			m->cam->xang -= 2 * M_PI / 180.0;
		else if (e.key.keysym.sym == SDLK_UP)
			m->cam->yang += 2 * M_PI / 180.0;
		else if (e.key.keysym.sym == SDLK_DOWN)
			m->cam->yang -= 2 * M_PI / 180.0;
		else if (e.key.keysym.sym == SDLK_w)
			m->cam->zang += 2 * M_PI / 180.0;
		else if (e.key.keysym.sym == SDLK_s)
			m->cam->zang -= 2 * M_PI / 180.0;
		else if (e.key.keysym.sym == SDLK_KP_4)
			m->cam->loc->x += 10;
		else if (e.key.keysym.sym == SDLK_KP_6)
			m->cam->loc->x -= 10;
		else if (e.key.keysym.sym == SDLK_KP_2)
			m->cam->loc->y += 10;
		else if (e.key.keysym.sym == SDLK_KP_8)
			m->cam->loc->y -= 10;
		else if (e.key.keysym.sym == SDLK_z)
			m->cam->loc->z += 10;
		else if (e.key.keysym.sym == SDLK_x)
			m->cam->loc->z -= 10;
		m->cam->xcos = cos(m->cam->xang);
		m->cam->xsin = sin(m->cam->xang);
		m->cam->ycos = cos(m->cam->yang);
		m->cam->ysin = sin(m->cam->yang);
		m->cam->zcos = cos(m->cam->zang);
		m->cam->zsin = sin(m->cam->zang);
	}
}

t_cam	*init_cam(t_vec3f *loc, float xang, float yang, float zang)
{
	t_cam	*cam;

	if (!(cam = malloc(sizeof(t_cam))))
		return (NULL);
	cam->loc = loc;
	cam->xang = xang;
	cam->xcos = cos(xang);
	cam->xsin = sin(xang);
	cam->yang = yang;
	cam->ycos = cos(yang);
	cam->ysin = sin(yang);
	cam->zang = zang;
	cam->zcos = cos(zang);
	cam->zsin = sin(zang);
	cam->focus = FOCUS;
	return (cam);
}

int		main(void)
{
	t_main		*m;

	if (SDL_Init(SDL_INIT_VIDEO) != 0
		|| !(m = malloc(sizeof(t_main)))
		|| !(m->window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, W, H, 0))
		|| !(m->screen = SDL_GetWindowSurface(m->window))
		|| !(m->cam = init_cam(&(t_vec3f){0, 0, 0},
					-M_PI / 2.0,
					0,
					-M_PI / 2.0)))
	{
		printf("Error\n");
		return (1);
	}
	t_vec3f	light_pos = (t_vec3f){200.0, 200.0, -50.0};

	m->objects[0] = new_sphere(&light_pos, 3,
		(SDL_Color){255, 0, 0, 255});
	m->objects[1] = new_sphere(&(t_vec3f){0.0, 300.0, 5.0}, 40,
		(SDL_Color){0, 255, 0, 255});
	m->objects[2] = new_sphere(&(t_vec3f){0.0, 50.0, 0.0}, 40,
		(SDL_Color){255, 255, 0, 255});
	m->objects[3] = new_sphere(&(t_vec3f){0.0, 425.0, 0.0}, 40,
		(SDL_Color){0, 255, 255, 255});
	m->objects[4] = new_sphere(&(t_vec3f){0.0, 800.0, 0.0}, 40,
		(SDL_Color){255, 0, 255, 255});
	m->objects[5] = new_triangle(&(t_vec3f){3.0, 4.0, 100.0},
		&(t_vec3f){7.0, 80.0, 9.0},
		&(t_vec3f){120.0, 9.0, 7.0},
		(SDL_Color){0, 255, 0, 255});
	
	m->lights[0] = &(t_light){
		&light_pos,
		1.0,
		(SDL_Color){255,255,255,255}
	};
	m->ray = &(t_vec3f){0, 1, 0};
	m->bpp = m->screen->format->BytesPerPixel;
	SDL_RaiseWindow(m->window);
	m->running = true;
	while (m->running)
	{
		render(m);
		handle_events(m);
	}
	exit(0);
	return (0);
}