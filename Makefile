NAME = ft_ping

HEADERS_DIR = includes

HEADERS =	$(HEADERS_DIR)/libft.h \
			$(HEADERS_DIR)/ft_ping.h

LIBFT_FILE = $(HEADERS_DIR)/libft.a

SRCS =	srcs/main.c \
		srcs/socket.c \
		srcs/ping.c \
		srcs/ping_recv.c \
		srcs/ping_recv_success.c \
		srcs/ping_send.c \
		srcs/utils.c \
		srcs/signal.c \
		srcs/input.c

OBJS = ${SRCS:.c=.o}

COMPILER = gcc

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS) $(HEADERS) $(LIBFT_FILE)
	$(COMPILER) $(OBJS) $(LIBFT_FILE) -o $(NAME)

$(OBJS): %.o: %.c $(HEADERS)
	$(COMPILER) -Wall -Wextra -Werror -g2 -I$(HEADERS_DIR) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
