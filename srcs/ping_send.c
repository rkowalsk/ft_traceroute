#include "ft_ping.h"

// with data
// returns the timeval captured right before (or right after) the package was sent
struct timeval	send_echo_request(uint16_t sequence)
{
	char			data[] = "la bite halala des barres";
	struct timeval	tv_before;
	struct icmphdr	icmp_hdr;
	char			packet[sizeof(struct icmphdr) + ft_strlen(data)];
	ssize_t			ret;

	if (sequence == 0)
		dprintf(1, "%ld data bytes", ft_strlen(data));
	ft_memset(&icmp_hdr, 0, sizeof(icmp_hdr));
	icmp_hdr.type = ICMP_ECHO;
	icmp_hdr.un.echo.sequence = htons(sequence);
	ft_memcpy(packet, &icmp_hdr, sizeof(icmp_hdr));
	ft_memcpy(packet + sizeof(icmp_hdr), data, ft_strlen(data));
	ret = sendto(g_sock_fd, packet, sizeof(icmp_hdr) + ft_strlen(data), 0,
		g_servinfos->ai_addr, g_servinfos->ai_addrlen);
	gettimeofday(&tv_before, NULL);
	if (ret <= 0)
	{
		if (ret < 0)
			dprintf(2, "%s", strerror(errno));
		tv_before.tv_sec = 0;
	}
	return (tv_before);
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