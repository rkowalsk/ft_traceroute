#include "ft_ping.h"

void	print_addrinfo(struct addrinfo *servinfos)
{
	char				printable_ip[INET6_ADDRSTRLEN];
	struct sockaddr_in	*net_ip;
	while (servinfos)
	{
		net_ip = (struct sockaddr_in *)(servinfos->ai_addr);
		inet_ntop(servinfos->ai_family, &(net_ip->sin_addr), printable_ip,
														sizeof(printable_ip));
		printf("adresse: %s\n", printable_ip);
		servinfos = servinfos->ai_next;
	}
}

int	create_addrinfo(struct s_input params, struct addrinfo **servinfos)
{
	struct addrinfo	hints;
	int				ret;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;
	ret = getaddrinfo(params.destination, NULL, &hints, servinfos);
	if (ret)
	{
		freeaddrinfo(*servinfos);
		*servinfos = NULL;
		dprintf(2, "error: %s\n", gai_strerror(ret));
		return (-1);
	}
	ret = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (ret == -1)
	{
		freeaddrinfo(*servinfos);
		*servinfos = NULL;
		dprintf(2, "error: %s\n", strerror(ret));
		return (-1);
	}
	return (ret);
}

int main(int argc, char **argv)
{
	struct s_input	params;
	struct addrinfo *servinfos;
	int				sock_fd;

	if (parse_input(argc, argv, &params))
		return (1);
	if (params.help)
	{
		print_usage();
		return (1);
	}
	sock_fd = create_addrinfo(params, &servinfos);
	if (sock_fd != -1)
	{
		print_addrinfo(servinfos);
		dprintf(1, "socket fd: %d\n", sock_fd);
		freeaddrinfo(servinfos);
	}
	return (0);
}