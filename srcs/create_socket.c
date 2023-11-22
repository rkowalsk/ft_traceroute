#include "ft_traceroute.h"

void	print_first_line(struct addrinfo infos, int max_hops)
{
	char				p_ip[INET6_ADDRSTRLEN];
	struct sockaddr_in	*net_ip;

	net_ip = (struct sockaddr_in *)(infos.ai_addr);
	inet_ntop(infos.ai_family, &(net_ip->sin_addr), p_ip, INET6_ADDRSTRLEN);
	dprintf(1, "traceroute to %s (%s), %d hops max, %d byte packets\n",
			infos.ai_canonname, p_ip, max_hops, SEND_HEADERS_SIZE + PAYLOAD_SIZE);
}

int	getaddrinfo_send(const char *destination, struct s_net *net)
{
	struct addrinfo	hints;

	ft_memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	hints.ai_flags = AI_CANONNAME;
	return (getaddrinfo(destination, NULL, &hints, &net->s_info));
}

// does the getaddrinfo, socket and then returns the error if any
int		create_socket(struct s_params params, struct s_net *net)
{
	int				ret;

	ret = getaddrinfo_send(params.destination, net);
	if (ret)
	{
		dprintf(2, "%s\n",  gai_strerror(ret));
		return (1);
	}
	net->r_sockfd = -1;
	net->s_sockfd = -1;
	net->s_sockfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	net->r_sockfd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (net->s_sockfd == -1 || net->r_sockfd == -1)
	{
		free_s_net(*net);
		return (perror_ret("socket"));
	}
	return (0);
}
