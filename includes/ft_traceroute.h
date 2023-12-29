#pragma once

#include "libft.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <errno.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <linux/errqueue.h>

#define RECV_BUFF_SIZE 4096
// size of packet sent without payload
// IPv4 header (20) + UDP header (8)
#define SEND_HEADERS_SIZE 28
// size of response without payload (it's not always received)
// IPv4 header (20) + ICMP package (ICMP (8) + IP (20) + UDP(8))
#define RECV_MIN_SIZE 56
#define PAYLOAD "Fuck Big Brother"
#define PAYLOAD_SIZE 16
#define HOST_NAME_MAX 256

struct	s_params
{
	int				help;
	char			*destination;
	unsigned int	first_ttl; // default 1
	unsigned int	max_ttl; // default 30
	unsigned int	squeries; // default 16
	in_port_t		port; // default 33434
	unsigned int	nqueries; // default 3
};

struct	s_net
{
	int				s_sockfd;
	struct addrinfo	*s_info;
	int				r_sockfd;
};

struct	s_probe
{
	struct timeval		tv_before;
	struct timeval		tv_after;
	struct sockaddr_in	src;
};

int		parse_input(int argc, char **argv, struct s_params *params);
void	print_usage(void);
int		create_socket(struct s_params params, struct s_net *net);
void	print_first_line(struct addrinfo r_infos, int max_hops);
int		perror_ret(const char *func);
int		free_s_net(struct s_net net);
void	free_results(struct s_probe **results, int len);
void	print_usage(void);
void	print_unknown_param(char *unknown_param, int argc);
void	print_missing_host(void);
void	print_missing_arg(int curr_argc, const char option, const char *arg);
void	print_bad_arg(int curr_argc, const char option, const char *arg);
int		trace_the_route(struct s_probe **results, struct s_net net,
													struct s_params *params);
int		recv_batch(size_t to_recv, struct s_probe **results, struct s_net net,
														struct s_params params);
ssize_t	send_batch(struct s_probe **results, struct s_net net,
	struct s_params *params, size_t *queries, size_t max_queries, int *port);