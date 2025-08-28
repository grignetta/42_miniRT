NAME = miniRT
NAME_BONUS = miniRT_bonus

CC = cc
CFLAGS = -g -Wall -Wextra -Werror -MMD -MP
INCS = -Iinclude -Ilib/libft -Ilib/minilibx_linux -I$(GNL_DIR)

LIBFT_DIR = lib/libft
MLX_DIR = lib/minilibx_linux
GNL_DIR  = lib/getnextline/gnl_mandatory

LIBS = -L$(LIBFT_DIR) -lft -L$(MLX_DIR) -lmlx -lXext -lX11 -lm #-lz

M_ROOT = miniRT_mandatory/src/
B_ROOT = miniRT_BONUS/src/

M_SRCS = $(shell find $(M_ROOT) -type f -name '*.c')
B_SRCS = $(shell find $(B_ROOT) -type f -name '*.c')

BUILD_M = build/mandatory
BUILD_B = build/bonus

GNL_SRCS = $(GNL_DIR)/get_next_line.c $(GNL_DIR)/get_next_line_utils.c
GNL_M_OBJS := $(patsubst %.c,$(BUILD_M)/%.o,$(GNL_SRCS))
GNL_B_OBJS := $(patsubst %.c,$(BUILD_B)/%.o,$(GNL_SRCS))

M_OBJS = $(patsubst %.c,$(BUILD_M)/%.o,$(M_SRCS)) $(GNL_M_OBJS)
B_OBJS = $(patsubst %.c,$(BUILD_B)/%.o,$(B_SRCS)) $(GNL_B_OBJS)

M_DEPS = $(M_OBJS:.o=.d)
B_DEPS = $(B_OBJS:.o=.d)

all: $(NAME)

bonus: $(NAME_BONUS)

$(NAME): $(LIBFT_DIR)/libft.a $(MLX_DIR)/libmlx.a $(M_OBJS)
	$(CC) $(CFLAGS) $(M_OBJS) $(LIBS) -o $@

$(NAME_BONUS): $(LIBFT_DIR)/libft.a $(MLX_DIR)/libmlx.a $(B_OBJS)
	$(CC) $(CFLAGS) $(B_OBJS) $(LIBS) -o $@

$(BUILD_M)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

$(BUILD_B)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -DBONUS $(INCS) -c $< -o $@

$(LIBFT_DIR)/libft.a:
	$(MAKE) -C $(LIBFT_DIR)

$(MLX_DIR)/libmlx.a:
	$(MAKE) -C $(MLX_DIR)

clean:
	$(MAKE) -C $(LIBFT_DIR) clean || true
	$(MAKE) -C $(MLX_DIR) clean || true
	rm -rf build

fclean: clean
	rm -f $(NAME) $(NAME_BONUS)
	$(MAKE) -C $(LIBFT_DIR) fclean || true

re: fclean all

run: $(NAME)
	./$(NAME) scenes/mandatory/file.rt

run-bonus: $(NAME_BONUS)
	./$(NAME_BONUS) scenes/bonus/file_bonus.rt

.PHONY: all bonus clean fclean re run run-bonus

-include $(M_DEPS) $(B_DEPS)