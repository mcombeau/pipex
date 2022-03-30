CC		= gcc
CFLAGS	= -Werror -Wextra -Wall
NAME	= pipex

SRC_PATH = src/
OBJ_PATH = obj/

SRC	= pipex.c \
	env_parsing.c \
	utils.c \
	init.c \
	file_handler.c \
	ft_putchar_fd.c \
	ft_putendl_fd.c \
	ft_putstr_fd.c \
	ft_split.c \
	ft_strjoin.c \
	ft_strlen.c \
	ft_strncmp.c \
	ft_strnstr.c \
	ft_substr.c \
	ft_strlcpy.c \
	get_next_line.c \
	get_next_line_utils.c
SRCS	= $(addprefix $(SRC_PATH), $(SRC))
OBJ		= $(SRC:.c=.o)
OBJS	= $(addprefix $(OBJ_PATH), $(OBJ))

all: $(NAME)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS): $(OBJ_PATH)

$(OBJ_PATH):
	mkdir $(OBJ_PATH) $(OBJ_PATH)get_next_line/

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT)

bonus: all

clean:
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all re clean fclean