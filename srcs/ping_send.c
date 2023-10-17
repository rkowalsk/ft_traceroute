#include "ft_ping.h"

void	create_packet(char *packet)
{
	struct timeval	tv_before;
	struct icmphdr	icmp_hdr;

	ft_memset(&icmp_hdr, 0, sizeof(icmp_hdr));
	icmp_hdr.type = ICMP_ECHO;
	icmp_hdr.un.echo.sequence = htons(g_stats.transmitted);
	ft_memcpy(packet, &icmp_hdr, sizeof(icmp_hdr));
	gettimeofday(&tv_before, NULL);
	ft_memcpy(packet + sizeof(icmp_hdr), &tv_before, sizeof(tv_before));
}

void	send_echo_request(int signal)
{
	char			packet[sizeof(struct icmphdr) + sizeof(struct timeval)];
	ssize_t			ret;

	(void) signal;
	if (g_stats.transmitted == 0)
	{
		if (g_verbose)
			dprintf(1, "%ld data bytes", sizeof(struct timeval));
		else
			dprintf(1, "%ld data bytes\n", sizeof(struct timeval));
	}
	create_packet(packet);
	ret = sendto(g_sock_fd,
		packet,
		sizeof(struct icmphdr) + sizeof(struct timeval),
		0,
		g_servinfos->ai_addr,
		g_servinfos->ai_addrlen);
	if (ret >= 0)
		g_stats.transmitted++; // used as sequence
	else
	{
		dprintf(2, "ping: %s\n", strerror(errno));
		errno = 0;
	}
	alarm(1);
}
