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

void	handle_events(t_main *m, SDL_Event e)
{
	char		c = 0;
	t_vec3f		angle;

	angle = m->cam->angle;
	double angdelta = 8 * M_PI / 180.0;
	if (e.key.keysym.sym == SDLK_DOWN && ++c)
		m->cam->angle = (t_vec3f){ angle[0] + angdelta, angle[1], angle[2]};
	else if (e.key.keysym.sym == SDLK_UP && ++c)
		m->cam->angle = (t_vec3f){ angle[0] - angdelta, angle[1], angle[2]};
	else if (e.key.keysym.sym == SDLK_RIGHT && ++c)
		m->cam->angle = (t_vec3f){ angle[0], (angle[1] + angdelta), angle[2]};
	else if (e.key.keysym.sym == SDLK_LEFT && ++c)
		m->cam->angle = (t_vec3f){ angle[0], (angle[1] - angdelta), angle[2]};
	else if (e.key.keysym.sym == SDLK_x && ++c)
		m->cam->angle = (t_vec3f){ angle[0], angle[1], angle[2] + angdelta};
	else if (e.key.keysym.sym == SDLK_z && ++c)
		m->cam->angle = (t_vec3f){ angle[0], angle[1], angle[2] - angdelta};
	else if (e.key.keysym.sym == SDLK_w && ++c)
		(*m->cam->loc)[2] -= 10;
	else if (e.key.keysym.sym == SDLK_s && ++c)
		(*m->cam->loc)[2] += 10;
	else if (e.key.keysym.sym == SDLK_a && ++c)
		(*m->cam->loc)[0] -= 10;
	else if (e.key.keysym.sym == SDLK_d && ++c)
		(*m->cam->loc)[0] += 10;
	else if (e.key.keysym.sym == SDLK_q && ++c)
		(*m->cam->loc)[1] -= 10;
	else if (e.key.keysym.sym == SDLK_e && ++c)
		(*m->cam->loc)[1] += 10;
	(c) ? render(m) : 0;
}

t_cam	*init_cam(t_vec3f *loc, t_vec3f angle)
{
	t_cam	*cam;

	if (!(cam = malloc(sizeof(t_cam))))
		return (NULL);
	cam->loc = loc;
	cam->angle = angle;
	cam->rot_mtx = init_matrix(angle);
	matrix_apply(cam->loc, cam->rot_mtx);
	cam->focus = FOCUS;
	return (cam);
}

int		main(int argc, char *argv[])
{
	t_main		*m;
	SDL_Event	e;

	if (SDL_Init(SDL_INIT_VIDEO) != 0
		|| !(m = malloc(sizeof(t_main)))
		|| !(m->window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, W, H, 0))
		|| !(m->screen = SDL_GetWindowSurface(m->window))
		|| !(m->cam = init_cam(&(t_vec3f){0, 0, -60}, (t_vec3f){0, 0, 0})))
	{
		//printf("Error\n");
		return (1);
	}
	if (argc < 2)
		return (10);
	m->objects = parse_scene(m, argv[1]);
	//m->ray = &(t_vec3f){0, 1, 0};

	m->bpp = m->screen->format->BytesPerPixel;
	SDL_RaiseWindow(m->window);
	m->running = true;
	render(m);
	while (m->running)
	{
		while (SDL_PollEvent(&e))
		{	
			if (e.key.keysym.sym == SDLK_ESCAPE ||
				(e.type == SDL_WINDOWEVENT &&
				e.window.event == SDL_WINDOWEVENT_CLOSE))
				game_quit(m);
			else if (e.type == SDL_KEYDOWN)
				handle_events(m, e);
		}
	}
	exit(0);
	return (0);
}