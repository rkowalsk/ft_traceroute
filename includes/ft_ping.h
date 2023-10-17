#pragma once

#include "libft.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <errno.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <linux/errqueue.h>

#define RECV_BUFF_SIZE 1024

struct		s_input
{
	char	*destination;
	int		verbose;
	int		help;
};

struct				s_stats
{
	size_t	transmitted;
	size_t	received;
	float	times;
	float	times_sq;
	float	min;
	float	max;
};

int		parse_input(int argc, char **argv, struct s_input *params);
void	print_usage(void);
int		create_socket(struct s_input params);
void	print_addrinfo(struct addrinfo servinfos);
int		ping_loop(int verbose);
float	receive_echo_reply(int verbose);
void	send_echo_request(int signal);
double	sqrt(double input);
int		perror_ret(const char *func);
int		free_all(int sock_fd, struct addrinfo *servinfo);
void	stats_and_exit(int signal);
float	print_recv_success(struct msghdr msg, int verbose, ssize_t ret,
													struct timeval tv_after);
void	print_recv_error(struct msghdr msg, int verbose, ssize_t ret);
void	verbose_first_line(uint16_t id, uint16_t sequence, int verbose);
void	get_ip_from_msghdr(struct msghdr msg, char *p_ip);

extern int				g_sock_fd;
extern int				g_verbose;
extern struct addrinfo	*g_servinfos;
extern struct s_stats	g_stats;