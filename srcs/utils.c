#include "ft_traceroute.h"

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

int	free_s_net(struct s_net net)
{
	int	error = 0;
	freeaddrinfo(net.s_info);
	if (net.s_sockfd >= 0)
		error += close(net.s_sockfd);
	if (net.r_sockfd >= 0)
		error += close(net.r_sockfd);
	net.r_sockfd = -1;
	net.s_sockfd = -1;
	if (error)
		return (perror_ret("close"));
	return (0);
}

// len is max_ttl - first_ttl + 1 OR until the failed malloc
void	free_results(struct s_probe **results, int len)
{
	int	i = 0;
	if (results)
	{
		while (i < len)
			free(results[i++]);
		free(results);
	}
}
