NAME = miniRT
HEADER = minirt.h
CC = cc
#CFLAGS = -g -Wall -Wextra -Werror -I/usr/local/lib #-fsanitize=thread
CFLAGS = -g -Wall -Wextra -Werror -I$(MLX_PATH)

LIBDIR = ./libft #check whose libft
OBJDIR = ./obj
LIBFT = $(LIBDIR)/libft.a

MLX_PATH = ./minilibx-linux #added for linux on mac
MLX_NAME = mlx
MLX = $(MLX_PATH)$(MLX_NAME)
MLX_LIBS = -L$(MLX_PATH) -lmlx -L/usr/lib -lXext -lX11 -lm -lbsd #added for linux on mac

SOURCES = main.c \
			render.c \
			vector_operations_1.c \
			vector_operations_2.c \
			utils.c \
			ray_trace_1.c \
			ray_trace_2.c \
			intersection.c \
			camera.c \
			light_computation.c \
			intersect_cylinder_utils.c \
			intersect_cylinder.c \
			parsing.c \
			parsing_objects.c \
			parsing_lights.c \
			parsing_utils.c \
			input_check_rest.c \
			free_functions.c \
			input_check_color.c \
			init.c \

OBJ = $(addprefix $(OBJDIR)/, $(SOURCES:.c=.o))

all: lib $(NAME)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -o $@ -c $< -I.

$(NAME): $(OBJ)
#		$(CC) $(CFLAGS) $(OBJ) -L$(LIBDIR) -lft -lXext -lX11 -l$(MLX_NAME) -lm -o $(NAME)
		$(CC) $(CFLAGS) $(OBJ) $(MLX_LIBS) -L$(LIBDIR) -lft -o $(NAME) #added for linux on mac

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