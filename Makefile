CC	= gcc
CFLAGS = -Werror -Wextra -Wall
NAME	= pipex

SRC_PATH = src/
OBJ_PATH = obj/
LIBFT_PATH = libft/

SRC	= pipex.c \
	env_parsing.c \
	utils.c \
	init.c
SRCS = $(addprefix $(SRC_PATH), $(SRC))
OBJ = $(SRC:.c=.o)
OBJS = $(addprefix $(OBJ_PATH), $(OBJ))
LIBFT = $(LIBFT_PATH)libft.a

all: $(NAME)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS): $(OBJ_PATH)

$(OBJ_PATH):
	mkdir $(OBJ_PATH)

$(LIBFT):
	make -C $(LIBFT_PATH) all

$(NAME): $(LIBFT) $(OBJS)
	cp $(LIBFT) $(NAME)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT)

clean:
	make -C $(LIBFT_PATH) clean
	rm -rf $(OBJ_PATH)

fclean: clean
	make -C $(LIBFT_PATH) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all re clean fclean