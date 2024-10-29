NAME = miniRT_bonus
HEADER = minirt.h
CC = cc
#CFLAGS = -g -Wall -Wextra -Werror -I/usr/local/lib #-fsanitize=thread
CFLAGS = -g -Wall -Wextra -Werror -I$(MLX_PATH)

LIBDIR = ../libft #check whose libft
OBJDIR = ./obj
LIBFT = $(LIBDIR)/libft.a

MLX_PATH = ./minilibx-linux #added for linux on mac
MLX_NAME = mlx
MLX = $(MLX_PATH)$(MLX_NAME)
MLX_LIBS = -L$(MLX_PATH) -lmlx -L/usr/lib -lXext -lX11 -lm -lbsd #added for linux on mac

SOURCES = main_bonus.c \
			render_bonus.c \
			vector_operations_1_bonus.c \
			vector_operations_2_bonus.c \
			utils_bonus.c \
			ray_trace_bonus.c \
			intersection_bonus.c \
			camera_bonus.c \
			light_computation_bonus.c \
			light_computation_utils_bonus.c \
			intersect_cylinder_utils_bonus.c \
			intersect_cylinder_bonus.c \
			parsing_bonus.c \
			parsing_objects_bonus_1.c \
			parsing_objects_bonus_2.c \
			parsing_lights_bonus.c \
			parsing_utils_bonus.c \
			input_check_rest_bonus.c \
			free_functions_bonus.c \
			input_check_color_bonus.c \
			init_bonus.c \
			color_handling_bonus.c \

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