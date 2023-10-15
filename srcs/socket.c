#include "ft_ping.h"

void	print_addrinfo(struct addrinfo servinfos)
{
	char				p_ip[INET6_ADDRSTRLEN];
	struct sockaddr_in	*net_ip;

	net_ip = (struct sockaddr_in *)(servinfos.ai_addr);
	inet_ntop(servinfos.ai_family, &(net_ip->sin_addr), p_ip, INET6_ADDRSTRLEN);
	dprintf(1, "PING %s (%s): ", servinfos.ai_canonname, p_ip);
}

void	setup_hints(struct addrinfo *hints)
{
	ft_memset(hints, 0, sizeof(struct addrinfo));
	hints->ai_family = AF_INET;
	hints->ai_socktype = SOCK_RAW;
	hints->ai_protocol = IPPROTO_ICMP;
	hints->ai_flags = AI_CANONNAME;
}

int	set_socket_options(int sock_fd)
{
	// struct timeval	tv;
	int	yes;

	yes = 1;
	if (setsockopt(sock_fd, IPPROTO_IP, IP_RECVTTL, &yes, sizeof(yes)))
		return (-1);
	if (setsockopt(sock_fd, IPPROTO_IP, IP_RECVERR, &yes, sizeof(yes)))
		return (-1);
	// tv.tv_sec = 1;
	// tv.tv_usec = 0;
	// if (setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)))
	// 	return (-1);
	return (0);
}

// does the getaddrinfo, socket and then returns the error if any
int		create_socket(struct s_input params)
{
	struct addrinfo	hints;
	int				ret;

	setup_hints(&hints);
	ret = getaddrinfo(params.destination, NULL, &hints, &g_servinfos);
	if (ret)
	{
		dprintf(2, "ping: %s\n",  gai_strerror(ret));
		return (1);
	}
	g_sock_fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_ICMP);
	if (g_sock_fd == -1)
	{
		freeaddrinfo(g_servinfos);
		return (perror_ret("socket"));
	}
	if (set_socket_options(g_sock_fd))
	{
		free_all(g_sock_fd, g_servinfos);
		return (perror_ret("setsockopt"));
	}
	return (0);
}
