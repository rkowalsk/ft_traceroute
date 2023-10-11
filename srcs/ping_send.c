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

// with timeval in data
void	send_echo_request(int signal)
{
	char			packet[sizeof(struct icmphdr) + sizeof(struct timeval)];
	ssize_t			ret;

	(void) signal;
	if (g_stats.transmitted == 0)
		dprintf(1, "%ld data bytes", sizeof(struct timeval));
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

// without data
// ssize_t	send_echo_request(int sock_fd, struct addrinfo servinfos,
// 															uint16_t sequence)
// {
// 	struct icmphdr	icmp_hdr;
// 	ssize_t			ret;
// 	char			packet[sizeof(icmp_hdr)];

// 	ft_memset(&icmp_hdr, 0, sizeof(icmp_hdr));
// 	icmp_hdr.type = ICMP_ECHO;
// 	icmp_hdr.un.echo.sequence = htons(sequence);
// 	ft_memcpy(packet, &icmp_hdr, sizeof(icmp_hdr));
// 	ret = sendto(sock_fd, packet, sizeof(icmp_hdr), 0, servinfos.ai_addr, servinfos.ai_addrlen);
// 	dprintf(1, "sendto success: ret: %zd\nseq: %d\n", ret, sequence);
// 	if (ret < 0)
// 		dprintf(2, "sendto error: %s", strerror(errno));
// 	return (ret);
// }