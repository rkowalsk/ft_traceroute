#include "ft_ping.h"

double	sqrt(double input)
{
	double	root = input / 4;
	size_t	i = 0;

	while (i < 32)
	{
		root = (root + (input / root)) / 2;
		i++;
	}
	return (root);
}

int	perror_ret(const char *func)
{
	dprintf(2, "%s: %s\n", func, strerror(errno));
	return(2);
}

int	free_all(int sock_fd, struct addrinfo *servinfos)
{
	freeaddrinfo(servinfos);
	if (close(sock_fd))
		return (perror_ret("close"));
	return (0);
}