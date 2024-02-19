#include "ft_traceroute.h"

int	send_packet(struct s_net net, int ttl, int port)
{
	int ret;
	if (setsockopt(net.s_sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)))
	{
		dprintf(2, "setsockopt: %s\n", strerror(errno));
		return (-1);
	}
	((struct sockaddr_in *) net.s_info->ai_addr)->sin_port = htons(port);
	ret = sendto(net.s_sockfd, PAYLOAD, PAYLOAD_SIZE, 0, net.s_info->ai_addr,
														net.s_info->ai_addrlen);
	if (ret < 0)
	{
		dprintf(2, "sendto: %s\n", strerror(errno));
		errno = 0;
	}
	return (ret);
}

int	send_sleep(unsigned int sendwait)
{
	int	ret = 0;
	if (!sendwait)
		return (0);
	if (sendwait <= 10)
		sleep(sendwait);
	else
		ret = usleep(sendwait * 1000);
	if (ret < 0)
		dprintf(2, "usleep: %s\n", strerror(errno));
	return (ret);
}

ssize_t	send_batch(struct s_probe **results, struct s_net net,
		struct s_params *params, size_t *queries, size_t max_queries, int *port)
{
	struct timeval	tv;
	size_t			i;
	int				ttl;
	i = 0;
	gettimeofday(&tv, NULL);
	while (i < params->squeries && *queries < max_queries)
	{
		ttl = (*queries / params->nqueries) + params->first_ttl;
		if (send_sleep(params->sendwait))
			return (-1);
		if (send_packet(net, ttl, *port) < 0)
			return (-1);
		(*port)++;
		results[*queries / params->nqueries][*queries % params->nqueries]
																.tv_before = tv;
		i++;
		(*queries)++;
	}
	return (i);
}
