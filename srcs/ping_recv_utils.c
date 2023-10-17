#include "ft_ping.h"

void	get_ip_from_msghdr(struct msghdr msg, char *p_ip)
{
	struct sockaddr_in	*net_ip;

	net_ip = (struct sockaddr_in *)(msg.msg_name);
	inet_ntop(net_ip->sin_family, &(net_ip->sin_addr), p_ip, INET6_ADDRSTRLEN);
}

void	verbose_first_line(uint16_t id, uint16_t sequence, int verbose)
{
	if (sequence == 0 && verbose)
		dprintf(1, ", id 0x%x = %d\n", id, id);
}
