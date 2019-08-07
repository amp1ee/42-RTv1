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

#define STEP	(50)
#define DOT(a, b) vec3f_dot((a), (b))
#define INV(a, b) sqrt(1 - SQ(DOT((a), (b))))
#define uI ((t_vec3f){1, 0, 0})
#define uJ ((t_vec3f){0, -1, 0})
#define uK ((t_vec3f){0, 0, 1})
/*
**	presses w... angle (0.25, -0.5, 0)
**	loc.x += 10 * dot(angle, (t_vec3f){1, 0, 0})
*/
void	handle_events(t_main *m, SDL_Event e)
{
	char		c = 0;
	t_vec3f		angle;
	t_cam		*cam;

	cam = m->cam;
	angle = cam->angle;
	t_vec3f ray = cam->ray;
	t_vec3f perpray = vec3f_copy(ray);
	matrix_apply(&perpray, init_matrix(angle+(t_vec3f){M_PI/2, -M_PI/2, M_PI/2}));

	double angdelta = 8 * M_PI / 180.0;
	if (e.key.keysym.sym == SDLK_DOWN && ++c)
		cam->angle = (t_vec3f){ angle[0] + angdelta, angle[1], angle[2]};
	else if (e.key.keysym.sym == SDLK_UP && ++c)
		cam->angle = (t_vec3f){ angle[0] - angdelta, angle[1], angle[2]};
	else if (e.key.keysym.sym == SDLK_RIGHT && ++c)
		cam->angle = (t_vec3f){ angle[0], (angle[1] - angdelta), angle[2]};
	else if (e.key.keysym.sym == SDLK_LEFT && ++c)
		cam->angle = (t_vec3f){ angle[0], (angle[1] + angdelta), angle[2]};
	else if (e.key.keysym.sym == SDLK_x && ++c)
		cam->angle = (t_vec3f){ angle[0], angle[1], angle[2] + angdelta};
	else if (e.key.keysym.sym == SDLK_z && ++c)
		cam->angle = (t_vec3f){ angle[0], angle[1], angle[2] - angdelta};
	else if (e.key.keysym.sym == SDLK_w && ++c)
		(*cam->loc) += vec3f_multsc((t_vec3f){ DOT(ray, uI), -DOT(ray, uJ), DOT(ray, uK) }, STEP);
	else if (e.key.keysym.sym == SDLK_s && ++c)
		(*cam->loc) -= vec3f_multsc((t_vec3f){ DOT(ray, uI), -DOT(ray, uJ), DOT(ray, uK) }, STEP);
	else if (e.key.keysym.sym == SDLK_a && ++c)
		(*cam->loc) -= vec3f_multsc((t_vec3f){ DOT(perpray, uI), 0, DOT(perpray, uK) }, STEP);
	else if (e.key.keysym.sym == SDLK_d && ++c)
		(*cam->loc) += vec3f_multsc((t_vec3f){ DOT(perpray, uI), 0, DOT(perpray, uK) }, STEP);
	else if (e.key.keysym.sym == SDLK_q && ++c)
		(*cam->loc)[1] -= 10;
	else if (e.key.keysym.sym == SDLK_e && ++c)
		(*cam->loc)[1] += 10;
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
		|| !(m->cam = init_cam(&(t_vec3f){0, 0, -160}, (t_vec3f){0, 0, 0})))
	{
		//printf("Error\n");
		return (1);
	}
	if (argc < 2)
		return (10);
	m->objects = parse_scene(m, argv[1]);

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