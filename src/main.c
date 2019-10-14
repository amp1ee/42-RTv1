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

void	rtv1_quit(t_main *m)
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
	const t_v3		ray = m->cam->ray;
	const t_v3		perpray = XPROD(m->cam->ray, UJ);
	t_cam			*cam;

	cam = m->cam;
	if (key == SDLK_w)
		(cam->pos) += v3_multsc((t_v3){ DOT(ray, UI), -DOT(ray, UJ), DOT(ray, UK) }, STEP);
	else if (key == SDLK_s)
		(cam->pos) -= v3_multsc((t_v3){ DOT(ray, UI), -DOT(ray, UJ), DOT(ray, UK) }, STEP);
	else if (key == SDLK_a)
		(cam->pos) -= v3_multsc((t_v3){ DOT(perpray, UI), 0, DOT(perpray, UK) }, STEP);
	else if (key == SDLK_d)
		(cam->pos) += v3_multsc((t_v3){ DOT(perpray, UI), 0, DOT(perpray, UK) }, STEP);
	else if (key == SDLK_q)
		(cam->pos)[1] -= STEP;
	else
		(cam->pos)[1] += STEP;
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

t_cam				*init_cam(t_v3 start_pos)
{
	t_cam			*cam;

	if (!(cam = malloc(sizeof(t_cam))))
		return (NULL);
	cam->angle = (t_v3){ 0, 0, 0 };
	cam->rot_mtx = init_matrix(cam->angle);
	cam->pos = start_pos;
	matrix_apply(&(cam->pos), cam->rot_mtx);
	cam->focus = FOCUS;
	return (cam);
}

t_main				*rtv1_init(char **argv)
{
	t_main			*m;

	if (!(m = malloc(sizeof(t_main))))
		return (NULL);
	if (SDL_Init(SDL_INIT_VIDEO) != 0 ||
		!(m->window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED,
										SDL_WINDOWPOS_CENTERED, W, H, 0)) ||
		!(m->screen = SDL_GetWindowSurface(m->window)))
		return (NULL);
	m->objects = parse_scene(m, argv[1]);
	if (m->objects == NULL)
	{
		ft_putendl("Error on scene parsing");
		return (NULL);
	}
	if (!(m->cam = init_cam(m->start_pos)))
		return (NULL);
	m->recur_depth = (argv[2]) ? ft_atoi(argv[2]) : 3;
	return (m);
}

void				main_loop(t_main *m)
{
	SDL_Event		e;

	SDL_RaiseWindow(m->window);
	m->running = true;
	render(m);
	while (m->running)
	{
		while (SDL_PollEvent(&e))
			handle_events(m, e);
	}
}

void				leakage(void)
{
	system("leaks RTv1");
}

int					main(int argc, char *argv[])
{
	t_main			*m;

	if (argc < 2 || argc > 3)
	{
		ft_putendl("Usage: ./RTv1 scene.scn [recursive depth]");
		return (1);
	}
	atexit(leakage);
	m = rtv1_init(argv);
	if (m == NULL)
	{
		ft_putendl("Initialization error");
		return (10);
	}
	main_loop(m);
	return (0);
}
