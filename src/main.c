#include "rtv1.h"

void	free_mem(t_main *m)
{
	int		obj_num;

	obj_num = m->obj_num;
	while (obj_num--)
	{
		(m->objects[obj_num])->cleanup((m->objects[obj_num])->data);
		ft_memdel((void **)&(m->objects[obj_num]));
	}
	ft_memdel((void **)&(m->objects));
	ft_memdel((void **)&(m->cam));
}

void	game_quit(t_main *m)
{
	m->running = false;
	free_mem(m);
	SDL_PixelFormat* pixelFormat = m->screen->format;
	Uint32 pixelFormatEnum = pixelFormat->format;
	const char* surfacePixelFormatName = SDL_GetPixelFormatName(pixelFormatEnum);
	SDL_Log("The surface's pixelformat is %s", surfacePixelFormatName);
	SDL_FreeSurface(m->screen);
	SDL_DestroyWindow(m->window);
	SDL_Quit();
	exit(0);
}

#define STEP	(50)
#define ANG		(8 * (M_PI) / 180.0)
#define DOT(a, b) v3_dot((a), (b))
#define XPROD(a, b) v3_cross((a), (b))
#define INV(a, b) sqrt(1 - SQ(DOT((a), (b))))
#define UI ((t_v3){1, 0, 0})
#define UJ ((t_v3){0, -1, 0})
#define UK ((t_v3){0, 0, 1})

#define KEY_ISMOVE(k) (k == SDLK_w || k == SDLK_a || k == SDLK_s || \
						k == SDLK_d || k == SDLK_q || k == SDLK_e)
#define KEY_ISROT(k) (k == SDLK_DOWN || k == SDLK_UP || k == SDLK_x || \
						k == SDLK_RIGHT || k == SDLK_LEFT || k == SDLK_z)

static inline void	cam_move(t_main *m, SDL_Keycode key)
{
	const t_cam		*cam = m->cam;
	const t_v3		ray = cam->ray;
	const t_v3		perpray = XPROD(m->cam->ray, UJ);

	if (key == SDLK_w)
		(*cam->pos) += v3_multsc((t_v3){ DOT(ray, UI), -DOT(ray, UJ), DOT(ray, UK) }, STEP);
	else if (key == SDLK_s)
		(*cam->pos) -= v3_multsc((t_v3){ DOT(ray, UI), -DOT(ray, UJ), DOT(ray, UK) }, STEP);
	else if (key == SDLK_a)
		(*cam->pos) -= v3_multsc((t_v3){ DOT(perpray, UI), 0, DOT(perpray, UK) }, STEP);
	else if (key == SDLK_d)
		(*cam->pos) += v3_multsc((t_v3){ DOT(perpray, UI), 0, DOT(perpray, UK) }, STEP);
	else if (key == SDLK_q)
		(*cam->pos)[1] -= STEP;
	else
		(*cam->pos)[1] += STEP;
}

static inline void	cam_rotate(t_main *m, SDL_Keycode key)
{
	const t_v3		angle = m->cam->angle;
	t_cam			*cam;

	cam = m->cam;
	if (key == SDLK_DOWN)
		cam->angle = (t_v3){ angle[0] + ANG, angle[1], angle[2]};
	else if (key == SDLK_UP)
		cam->angle = (t_v3){ angle[0] - ANG, angle[1], angle[2]};
	else if (key == SDLK_RIGHT)
		cam->angle = (t_v3){ angle[0], (angle[1] - ANG), angle[2]};
	else if (key == SDLK_LEFT)
		cam->angle = (t_v3){ angle[0], (angle[1] + ANG), angle[2]};
	else if (key == SDLK_x)
		cam->angle = (t_v3){ angle[0], angle[1], angle[2] + ANG};
	else
		cam->angle = (t_v3){ angle[0], angle[1], angle[2] - ANG};
}

void				handle_events(t_main *m, SDL_Event e)
{
	if (e.key.keysym.sym == SDLK_ESCAPE ||
		(e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE))
		game_quit(m);
	else if (KEY_ISROT(e.key.keysym.sym))
		cam_rotate(m, e.key.keysym.sym);
	else if (KEY_ISMOVE(e.key.keysym.sym))
		cam_move(m, e.key.keysym.sym);
	else
		return ;
	render(m);
}

t_cam				*init_cam(t_v3 *pos, t_v3 angle)
{
	t_cam			*cam;

	if (!(cam = malloc(sizeof(t_cam))))
		return (NULL);
	cam->pos = pos;
	cam->angle = angle;
	cam->rot_mtx = init_matrix(angle);
	matrix_apply(cam->pos, cam->rot_mtx);
	cam->focus = FOCUS;
	return (cam);
}

int					main(int argc, char *argv[])
{
	t_main			*m;
	SDL_Event		e;

	if (SDL_Init(SDL_INIT_VIDEO) != 0
		|| !(m = malloc(sizeof(t_main)))
		|| !(m->window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, W, H, 0))
		|| !(m->screen = SDL_GetWindowSurface(m->window))
		|| !(m->cam = init_cam(&(t_v3){0, 0, -160}, (t_v3){0, 0, 0})))
	{
		//printf("Error\n");
		return (1);
	}
	if (argc < 2)
		return (10);
	m->objects = parse_scene(m, argv[1]);
	m->recur_depth = (argv[2]) ? ft_atoi(argv[2]) : 1;
	SDL_RaiseWindow(m->window);
	m->running = true;
	render(m);
	while (m->running)
	{
		while (SDL_PollEvent(&e))
			handle_events(m, e);
	}
	ft_memdel((void **)&m);
	//system("leaks RTv1");
	return (0);
}
