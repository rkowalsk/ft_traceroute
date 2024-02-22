#include "ft_traceroute.h"

// checks if a full hop contains at least one packet from the target
int	hop_contains_target(struct s_net net, struct s_probe *results,
													const unsigned int nqueries)
{
	unsigned int	i;
	in_addr_t		target;

	target = ((struct sockaddr_in *) net.s_info->ai_addr)->sin_addr.s_addr;
	i = 0;
	while (i < nqueries)
	{
		if (results[i].src.sin_addr.s_addr == target)
			return (1);
		i++;
	}
	return (0);
}

float	get_diff(struct timeval before, struct timeval after)
{
	float	diff;

	diff = (float) (after.tv_sec - before.tv_sec) * 1000;
	diff += (float) (after.tv_usec - before.tv_usec) / 1000;
	return (diff);
}

void	print_time(struct s_probe **results, size_t batch, size_t query)
{
	if (results[batch][query].tv_after.tv_sec == 0
								&& results[batch][query].tv_after.tv_usec == 0)
		dprintf(1,"*");
	else
		dprintf(1, "%.3f ms", get_diff(results[batch][query].tv_before,
										results[batch][query].tv_after));
}

void	print_src(struct s_probe **results, size_t batch, size_t query)
{
	struct in_addr	src;
	char			*ip;
	char			hostname[HOST_NAME_MAX];
	int				ret;

	src.s_addr = results[batch][query].src.sin_addr.s_addr;
	if (src.s_addr != 0 && (query == 0
				|| src.s_addr != results[batch][query - 1].src.sin_addr.s_addr))
	{
		ip = inet_ntoa(src);
		ret = getnameinfo((struct sockaddr *) &(results[batch][query].src), 
			sizeof(struct sockaddr_in), hostname, HOST_NAME_MAX, NULL, 0, 0);
		if (ret)
			dprintf(2, "getnameinfo: %s\n", gai_strerror(ret));
		dprintf(1, "%s (%s)  ", hostname, ip);
	}
}

// returns 1 if the target batch has been printed and 0 if not
int	print_batch(struct s_probe **results, struct s_params params,
								struct s_net net, ssize_t sent, size_t queries)
{
	size_t	batch;
	size_t	query;
	size_t	i;

	i = queries - sent;
	while (i < queries)
	{
		batch = i / params.nqueries;
		query = i % params.nqueries;
		if (query == 0)
		{
			if (batch > 0 
				&& hop_contains_target(net, results[batch - 1], params.nqueries))
				return (1);
			dprintf(1, "%2ld  ", batch + params.first_ttl);
		}
		else
			dprintf(1, "  ");
		print_src(results, batch, query);
		print_time(results, batch, query);
		if (query == params.nqueries - 1)
			dprintf(1, "\n");
		i++;
	}
	return (0);
}

int	trace_the_route(struct s_probe **results, struct s_net net,
														struct s_params *params)
{
	size_t	queries;
	size_t	max_queries;
	ssize_t	sent;
	int		done;
	int		port;

	queries = 0;
	done = 0;
	max_queries = (params->max_ttl - params->first_ttl + 1) * params->nqueries;
	port = params->port;
	while (queries < max_queries && !done)
	{
		sent = send_batch(results, net, params, &queries, max_queries, &port);
		if (sent < 0)
			return (1);
		if (recv_batch(sent, results, net, *params))
			return (1);
		done = print_batch(results, *params, net, sent, queries);
	}
	return (0);
}