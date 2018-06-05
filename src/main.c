#include "rtv1.h"

void	game_quit(t_main *m)
{
	SDL_DestroyWindow(m->window);
	SDL_Quit();
	exit(0);
}

void	handle_events(t_main *m)
{
	SDL_Event	e;

	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE)
			game_quit(m);
	}
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
	m->objects[0] = new_sphere(&(t_vec4f){5.0, 2.0, 3.0, 0.0}, 2.0,
		&(SDL_Color){255, 0, 0, 255});
	m->objects[1] = new_sphere(&(t_vec4f){1.0, 3.0, 5.0, 2.0}, 3.0,
		&(SDL_Color){255, 0, 0, 255});
/*	m->objects[2] = new_triangle(&(t_vec4f){3.0, 4.0, 1.0, 0.0},
		&(t_vec4f){7.0, 8.0, 9.0, 0.0},
		&(t_vec4f){12.0, 9.0, 7.0, 0.0},
		&(SDL_Color){0, 255, 0, 255});*/
	m->camera = &(t_vec4f){0, 0, -100, 0};
	m->ray = &(t_vec4f){0, -1, 0, 0};
	m->bpp = m->screen->format->BytesPerPixel;
	while (true)
	{
		render(m);
		handle_events(m);
	}

	SDL_PixelFormat* pixelFormat = m->screen->format;
	Uint32 pixelFormatEnum = pixelFormat->format;
	const char* surfacePixelFormatName = SDL_GetPixelFormatName(pixelFormatEnum);
	SDL_Log("The surface's pixelformat is %s", surfacePixelFormatName);
	
	return (0);
}