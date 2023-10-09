#include "ft_ping.h"

void	verbose_first_line(uint16_t id, uint16_t sequence, int verbose)
{
	if (sequence == 0)
	{
		if (verbose)
			dprintf(1, ", id 0x%x = %d\n", id, id);
		else
			dprintf(1, "\n");
	}
}

unsigned short	get_ttl(struct msghdr msg_hdr)
{
	int				ttl;
	struct cmsghdr	*cmsg;

	ttl = -1;
	cmsg = CMSG_FIRSTHDR(&msg_hdr);
	while (cmsg != NULL)
	{
		if (cmsg->cmsg_level == IPPROTO_IP && cmsg->cmsg_type == IP_TTL)
		{
			memcpy(&ttl, CMSG_DATA(cmsg), sizeof(ttl));
			break;
		}
		cmsg = CMSG_NXTHDR(&msg_hdr, cmsg);
	}
	return (ttl);
}

void	print_recv(struct msghdr msg, int verbose, ssize_t ret)
{
	struct icmphdr		icmp_hdr;
	uint16_t			sequence;
	char				p_ip[INET6_ADDRSTRLEN];
	struct sockaddr_in	*net_ip;

	if (msg.msg_iovlen < 1 ||msg.msg_iov[0].iov_len < sizeof(icmp_hdr))
	{
		dprintf(2, "error: iov received too small\n");
		return ;
	}
	ft_memcpy(&icmp_hdr, msg.msg_iov[0].iov_base, sizeof(icmp_hdr));
	sequence = ntohs(icmp_hdr.un.echo.sequence);
	verbose_first_line(ntohs(icmp_hdr.un.echo.id), sequence, verbose);
	net_ip = (struct sockaddr_in *)(msg.msg_name);
	inet_ntop(net_ip->sin_family, &(net_ip->sin_addr), p_ip, INET6_ADDRSTRLEN);
	dprintf(1, "%ld bytes from %s: ", ret, p_ip);
	dprintf(1, "icmp_seq=%hd ttl=%hd ", sequence, get_ttl(msg));
}
