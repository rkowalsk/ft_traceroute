#include "ft_ping.h"

void	print_verbose(struct msghdr msg)
{
	struct cmsghdr				*cmsg;
	struct sock_extended_err	*error;

	cmsg = CMSG_FIRSTHDR(&msg);
	while (cmsg != NULL)
	{
		if (cmsg->cmsg_level == IPPROTO_IP && cmsg->cmsg_type == IP_RECVERR)
		{
			error = (struct sock_extended_err *) CMSG_DATA(cmsg);
			if (error->ee_origin == SO_EE_ORIGIN_ICMP)
				dprintf(2, "type=%d code=%d ", error->ee_type, error->ee_code);
			break;
		}
		cmsg = CMSG_NXTHDR(&msg, cmsg);
	}
}

void	print_recv_error(struct msghdr msg, int verbose, ssize_t ret)
{
	char				p_ip[INET6_ADDRSTRLEN];
	struct icmphdr		icmp_hdr;
	uint16_t			seq;

	if (msg.msg_iovlen < 1 || msg.msg_iov[0].iov_len < sizeof(icmp_hdr))
	{
		dprintf(2, "error: iov received too small (wtf)\n");
		return ;
	}
	ft_memcpy(&icmp_hdr, msg.msg_iov[0].iov_base, sizeof(icmp_hdr));
	seq = ntohs(icmp_hdr.un.echo.sequence);
	verbose_first_line(ntohs(icmp_hdr.un.echo.id), seq, verbose);
	get_ip_from_msghdr(msg, p_ip);
	dprintf(1, "%ld bytes from %s: ", ret, p_ip);
	if (verbose)
		print_verbose(msg);
	dprintf(2, "%s\n", strerror(errno));
	errno = 0;
}