#include "ft_ping.h"

struct msghdr	create_msghdr(struct iovec *iov, char *iov_buffer,
															char *ctl_buffer)
{
	struct msghdr	msg;

	ft_memset(&msg, 0, sizeof(msg));
	ft_memset(iov_buffer, 0, RECV_BUFF_SIZE);
	ft_memset(ctl_buffer, 0, RECV_BUFF_SIZE);
	iov[0].iov_base = iov_buffer;
	iov[0].iov_len = RECV_BUFF_SIZE;
	msg.msg_name = g_servinfos->ai_addr;
	msg.msg_namelen = g_servinfos->ai_addrlen;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	msg.msg_control = ctl_buffer;
	msg.msg_controllen = RECV_BUFF_SIZE;
	return (msg);
}

float	receive_echo_reply(int verbose)
{
	ssize_t			ret;
	struct timeval	tv_after;
	float			diff;
	struct msghdr	msg;
	struct iovec	iov[1];
	char			iov_buffer[RECV_BUFF_SIZE];
	char			ctl_buffer[RECV_BUFF_SIZE];

	msg = create_msghdr(iov, iov_buffer, ctl_buffer);
	ret = recvmsg(g_sock_fd, &msg, 0);
	if (ret > 0)
	{
		gettimeofday(&tv_after, NULL);
		diff = print_recv_success(msg, verbose, ret, tv_after);
	}
	else
	{
		diff = 0;
		ret = recvmsg(g_sock_fd, &msg, MSG_ERRQUEUE);
		if (ret <= 0)
			dprintf(2, "MSG_ERRQUEUE but je peux pas lire ????\n");
		else
			print_recv_error(msg, verbose, ret);
	}
	return (diff);
}
