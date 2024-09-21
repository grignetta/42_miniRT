NAME = miniRT
HEADER = minirt.h
CC = cc
CFLAGS = -g -Wall -Wextra -Werror  #-fsanitize=thread

LIBDIR = ./libft #check whose libft
OBJDIR = ./obj
LIBFT = $(LIBDIR)/libft.a

MLX_NAME = mlx
MLX = $(MLX_PATH)$(MLX_NAME)

SOURCES = main.c \
			rest.c \
			vector_operations.c \
			utils.c \
			ray_trace.c \
			intersection.c \

OBJ = $(addprefix $(OBJDIR)/, $(SOURCES:.c=.o))

all: lib $(NAME)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -o $@ -c $< -I.

$(NAME): $(OBJ)
		$(CC) $(CFLAGS) $(OBJ) -L$(LIBDIR) -lft -lXext -lX11 -l$(MLX_NAME) -lm -o $(NAME)

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