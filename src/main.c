#include "rtv1.h"

/*
int		main(void)
{
	t_vec4f		a;
	t_vec4f		b;
	t_vec4f		c;
	t_vec4i		d;
	t_vec4i		e;
	t_vec4i		f;

	a = (t_vec4f){0.2, 0.3, 0.4, 0.0};
	b = (t_vec4f){0.3, 0.4, 0.5, 0.0};
	c = a + b;
	printf("x=%.3f y=%.3f z=%.3f w=%.3f\n", c[0], c[1], c[2], c[3]);

	d = (t_vec4i){1, 2, 3, 4};
	e = (t_vec4i){2, 3, 4, 5};
	f = d + e;
	printf("x=%d y=%d z=%d w=%d\n", f[0], f[1], f[2], f[3]);
	return (0);
}
*/

void	game_quit(t_main *m)
{
	SDL_DestroyWindow(m->window);
	SDL_Quit();
	exit(0);
}

void	handle_events(t_main *m)
{
	SDL_Event	e;

	while (true)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE)
				game_quit(m);
		}
	}
}

int		main(void)
{
	t_main	*m;

	if (SDL_Init(SDL_INIT_VIDEO) != 0
		|| !(m = malloc(sizeof(t_main)))
		|| !(m->window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, W, H, 0)))
	{
		printf("Error\n");
		return (1);
	}
	handle_events(m);
	return (0);
}