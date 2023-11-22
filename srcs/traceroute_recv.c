#include "ft_traceroute.h"

int	set_timeout(int sock_fd, struct timeval tv)
{
	int ret;

	ret = setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
	if (ret == -1)
	{
		dprintf(2, "setsockopt: %s\n", strerror(errno));
		return (1);
	}
	return (0);
}

// identify the packet the original destination and port <----
// returns original packet port (to determine original ttl) or 0 if the package
// is not an interesting one
int	get_probe_port(char *buffer, int len, in_addr_t target,
														struct s_params params)
{
	struct iphdr	ip;
	struct icmphdr	icmp;
	struct udphdr	udp;
	int				max_port;
	int				host_port;
	
	max_port = params.port
			+ ((params.max_ttl - params.first_ttl + 1) * params.nqueries) - 1;
	if (len < RECV_MIN_SIZE)
		return (0);
	ft_memcpy(&ip, buffer, sizeof(struct iphdr));
	if (ip.protocol != IPPROTO_ICMP)
		return (0);
	buffer += sizeof(struct iphdr);
	ft_memcpy(&icmp, buffer, sizeof(struct icmphdr));
	if (!(icmp.type == 3 && icmp.code == 3) && !(icmp.type == 11 && icmp.code == 0))
		return (0);
	buffer += sizeof(struct icmphdr);
	ft_memcpy(&ip, buffer, sizeof(struct iphdr));
	if (ip.daddr != target || ip.protocol != IPPROTO_UDP)
		return (0);
	buffer += sizeof(struct iphdr);
	ft_memcpy(&udp, buffer, sizeof(struct udphdr));
	host_port = ntohs(udp.dest);
	if (host_port < params.port || host_port > max_port)
		return (0);
	return (host_port);
}

void	store_response(int port, struct s_probe **results, struct timeval tv,
								struct s_params params, struct sockaddr_in src)
{
	int	batch;
	int	query;

	batch = (port - params.port) / params.nqueries;
	query = (port - params.port) % params.nqueries;
	results[batch][query].tv_after = tv;
	results[batch][query].src = src;
}

// src ip, original ttl
int	handle_response(char *buffer, int len, struct s_probe **results,
					struct s_net net, struct s_params params,
					struct sockaddr_in src, struct timeval tv)
{
	(void) net;
	int			port;
	in_addr_t	target;

	target = ((struct sockaddr_in *) net.s_info->ai_addr)->sin_addr.s_addr;
	port = get_probe_port(buffer, len, target, params);
	if (!port)
		return (1);
	store_response(port, results, tv, params, src);
	return (2);
}

// returns 0 on timeout, -1 on error, 1 on received something uninteresting,
// 2 on received a response
int	recv_reply(struct s_probe **results, struct s_net net,
														struct s_params params)
{
	(void) results;
	int					ret;
	char				buffer[RECV_BUFF_SIZE];
	struct sockaddr_in	src;
	struct timeval		tv;
	socklen_t			src_len;

	src_len = sizeof(struct sockaddr_in);
	ft_memset(&src, 0, src_len);
	errno = 0;
	ret = recvfrom(net.r_sockfd, buffer, RECV_BUFF_SIZE, 0,
											(struct sockaddr *) &src, &src_len);
	gettimeofday(&tv, NULL);
	if (ret < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return (0);
		else
		{
			dprintf(2, "recvfrom: %s\n", strerror(errno));
			return (-1);
		}
	}
	return (handle_response(buffer, ret, results, net, params, src, tv));
}

int	recv_batch(size_t to_recv, struct s_probe **results, struct s_net net,
														struct s_params params)
{
	size_t			received;
	struct timeval	tv;
	int				ret;

	tv.tv_sec = 5;
	tv.tv_usec = 0;
	if (set_timeout(net.r_sockfd, tv))
		return (1);
	received = 0;
	do
	{
		ret = recv_reply(results, net, params);
		if (ret == -1)
			return (1);
		received += (ret == 2);
		if (received == to_recv && ret != 0)
		{
			tv.tv_sec = 0;
			tv.tv_usec = 200000; // 200ms
			if (set_timeout(net.r_sockfd, tv))
				return (1);
		}
	} while (ret != 0);
	return (0);
}
