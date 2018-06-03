#ifndef RTV1_H
# define RTV1_H

# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdbool.h>
# include "libft.h"

# ifdef __linux__
#  include <SDL2/SDL.h>
# elif __APPLE__
#  include "SDL2/SDL.h"
# endif

typedef double	t_vec4f __attribute__((vector_size(sizeof(double) * 4)));
typedef int		t_vec4i __attribute__((vector_size(sizeof(int) * 4)));

typedef double	t_vec2f __attribute__((vector_size(sizeof(double) * 2)));
typedef int		t_vec2i __attribute__((vector_size(sizeof(int) * 2)));

# define TITLE	"rtv1"
# define W		640
# define H		480

typedef struct	s_main
{
	SDL_Window	*window;

}				t_main;

#endif