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

// struct icmphdr
// {
//   uint8_t type;         /* message type */
//   uint8_t code;         /* type sub-code */
//   uint16_t checksum;
//   union
//   {
//     struct
//     {
//       uint16_t  id;
//       uint16_t  sequence;
//     } echo;                     /* echo datagram */
//     uint32_t    gateway;        /* gateway address */
//     struct
//     { 
//       uint16_t  __glibc_reserved;
//       uint16_t  mtu;
//     } frag;                     /* path mtu discovery */
//   } un;
// };

int		parse_input(int argc, char **argv, struct s_input *params);
void	print_usage(void);
int		create_socket(struct s_input params);
void	print_addrinfo(struct addrinfo servinfos);
int		ping_loop(int verbose);
struct timeval	receive_echo_reply(int verbose);
struct timeval	send_echo_request(uint16_t sequence);
double	sqrt(double input);
int		perror_ret(const char *func);
int		free_all(int sock_fd, struct addrinfo *servinfo);
void	stats_and_exit(int signal);
void	print_recv(struct msghdr msg, int verbose, ssize_t ret);

extern int				g_sock_fd;
extern struct addrinfo	*g_servinfos;
extern struct s_stats	g_stats;