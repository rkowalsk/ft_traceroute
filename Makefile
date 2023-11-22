NAME = ft_traceroute

HEADERS_DIR = includes

HEADERS =	$(HEADERS_DIR)/libft.h \
			$(HEADERS_DIR)/ft_traceroute.h

LIBFT_FILE = $(HEADERS_DIR)/libft.a

SRCS =	srcs/main.c \
		srcs/create_socket.c \
		srcs/utils.c \
		srcs/trace_the_route.c \
		srcs/traceroute_send.c \
		srcs/traceroute_recv.c \
		srcs/cli_prints.c \
		srcs/cli.c
		# srcs/signal.c \
		# srcs/ping.c \
		# srcs/ping_recv.c \
		# srcs/ping_recv_success.c \
		# srcs/ping_recv_error.c \
		# srcs/ping_recv_utils.c \
		# srcs/ping_send.c \

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
