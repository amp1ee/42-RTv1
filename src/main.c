#include "rtv1.h"

bool	game_quit(t_main *m)
{
	SDL_PixelFormat* pixelFormat = m->screen->format;
	Uint32 pixelFormatEnum = pixelFormat->format;
	const char* surfacePixelFormatName = SDL_GetPixelFormatName(pixelFormatEnum);
	SDL_Log("The surface's pixelformat is %s", surfacePixelFormatName);

	SDL_DestroyWindow(m->window);
	SDL_Quit();
	return (false);
}

bool	handle_events(t_main *m)
{
	SDL_Event	e;

	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE)
			return (game_quit(m));
	}
	return (true);
}

int		main(void)
{
	t_main		*m;

	if (SDL_Init(SDL_INIT_VIDEO) != 0
		|| !(m = malloc(sizeof(t_main)))
		|| !(m->window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, W, H, 0))
		|| !(m->screen = SDL_GetWindowSurface(m->window)))
	{
		printf("Error\n");
		return (1);
	}
	m->objects[0] = new_sphere(&(t_vec3f){5.0, 2.0, 3.0}, 2.0,
		&(SDL_Color){255, 0, 0, 255});
	m->objects[1] = new_sphere(&(t_vec3f){1.0, 3.0, 5.0}, 3.0,
		&(SDL_Color){255, 0, 0, 255});
/*	m->objects[2] = new_triangle(&(t_vec3f){3.0, 4.0, 1.0, 0.0},
		&(t_vec3f){7.0, 8.0, 9.0, 0.0},
		&(t_vec3f){12.0, 9.0, 7.0, 0.0},
		&(SDL_Color){0, 255, 0, 255});*/
	m->camera = &(t_vec3f){0, 0, -100};
	m->ray = &(t_vec3f){0, -1, 0};
	m->bpp = m->screen->format->BytesPerPixel;
	SDL_RaiseWindow(m->window);
	m->running = true;
	while (m->running)
	{
		render(m);
		m->running = handle_events(m);
	}
	exit(0);
	return (0);
}