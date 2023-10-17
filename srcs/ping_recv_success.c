#include "ft_ping.h"

unsigned short	get_ttl(struct msghdr msg)
{
	int				ttl;
	struct cmsghdr	*cmsg;

	ttl = -1;
	cmsg = CMSG_FIRSTHDR(&msg);
	while (cmsg != NULL)
	{
		if (cmsg->cmsg_level == IPPROTO_IP && cmsg->cmsg_type == IP_TTL)
		{
			memcpy(&ttl, CMSG_DATA(cmsg), sizeof(ttl));
			break;
		}
		cmsg = CMSG_NXTHDR(&msg, cmsg);
	}
	return (ttl);
}

float	sub_timevals(struct timeval before, struct timeval after)
{
	float	diff;

	diff = (after.tv_sec - before.tv_sec) * 1000;
	diff += (float)(after.tv_usec - before.tv_usec) / 1000;
	return (diff);
}

float	print_recv_success(struct msghdr msg, int verbose, ssize_t ret,
														struct timeval tv_after)
{
	char				p_ip[INET6_ADDRSTRLEN];
	struct icmphdr		icmp_hdr;
	uint16_t			seq;
	struct timeval		tv_before;
	float				diff;

	if (msg.msg_iovlen < 1 || msg.msg_iov[0].iov_len < sizeof(icmp_hdr))
	{
		dprintf(2, "error: iov received too small (wtf)\n");
		return (0);
	}
	ft_memcpy(&icmp_hdr, msg.msg_iov[0].iov_base, sizeof(icmp_hdr));
	ft_memcpy(&tv_before, msg.msg_iov[0].iov_base + sizeof(icmp_hdr),
														sizeof(struct timeval));
	seq = ntohs(icmp_hdr.un.echo.sequence);
	verbose_first_line(ntohs(icmp_hdr.un.echo.id), seq, verbose);
	get_ip_from_msghdr(msg, p_ip);
	dprintf(1, "%ld bytes from %s: ", ret, p_ip);
	diff = sub_timevals(tv_before, tv_after);
	if (verbose)
		dprintf(1, "type=%d code=%d ", icmp_hdr.type, icmp_hdr.code);
	dprintf(1, "icmp_seq=%hd ttl=%hd time=%.3fms\n", seq, get_ttl(msg), diff);
	return (diff);
}
