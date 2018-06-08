NAME	=rtv1
CC		=gcc
CFLAGS	=-Wall -Wextra -Werror -g -O3 -mavx

SRCDIR	=./src/
OBJDIR	=./obj/
INCLDIR	=./include/
LIBFTDIR=./libft/
SDLDIR	=./sdl2/

SRC		=$(addprefix $(SRCDIR),	\
			main.c				\
			render.c			\
			sphere.c			\
			\
			cube.c				\
			vec3f.c)
OBJ		=$(SRC:$(SRCDIR)%.c=$(OBJDIR)%.o)
INCLUDE	=-I$(INCLDIR) -I$(LIBFTDIR)
HEADERS	=$(addprefix $(INCLDIR), rtv1.h)
LIBFT	=$(addprefix $(LIBFTDIR), libft.a)
LFLAGS	=-L$(LIBFTDIR) -lft -lm

OSNAME	=$(shell uname -s)
ifeq ($(OSNAME), Darwin)
	CG_LIBS	=-framework OpenGL -framework AppKit -framework OpenCL	\
				-framework SDL2 -F$(SDLDIR)
	INCLUDES+=-F$(SDLDIR)							\
				-I$(SDLDIR)SDL2.framework/Headers/
else
	CG_LIBS = `sdl2-config --libs`
endif

.PHONY:	all clean fclean re

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(OBJ) -o $(NAME) $(LFLAGS) $(CG_LIBS)

$(OBJDIR)%.o: $(SRCDIR)%.c $(HEADERS)
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)

$(LIBFT):
	make -sC $(LIBFTDIR)

clean:
	rm -rf $(OBJDIR)
	make -sC $(LIBFTDIR) clean

fclean: clean
	rm -rf $(NAME)
	make -sC $(LIBFTDIR) fclean

re: fclean all
