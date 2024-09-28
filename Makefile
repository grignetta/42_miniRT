NAME = miniRT
HEADER = minirt.h
CC = cc
CFLAGS = -g -Wall -Wextra -Werror -I/usr/local/lib #-fsanitize=thread

LIBDIR = ./libft #check whose libft
OBJDIR = ./obj
LIBFT = $(LIBDIR)/libft.a

LDFLAGS := -L/usr/local/lib -lmlx -lXext -lX11 -lm
#LDFLAGS := -Llib -lmlx -lXext -lX11 -lm

SOURCES = main.c \
			render.c \
			vector_operations.c \
			utils.c \
			ray_trace.c \
			intersection.c \
			camera.c \
			light_computation.c \
			intersect_cylinder.c \
			intersect_cylinder_utils.c \

OBJ = $(addprefix $(OBJDIR)/, $(SOURCES:.c=.o))

all: lib $(NAME)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -o $@ -c $< -I.

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -L$(LIBDIR) -lft $(LDFLAGS) -o $(NAME)

lib:
	@$(MAKE) -C $(LIBDIR) all --no-print-directory

.PHONY: clean fclean all re

clean:
	$(MAKE) -C $(LIBDIR) clean
	rm -rf $(OBJDIR)

fclean: clean
	$(MAKE) -C $(LIBDIR) fclean
	rm -rf $(NAME)

re: fclean all