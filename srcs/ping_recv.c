#include "ft_ping.h"

struct msghdr	create_msghdr()
{
	struct msghdr	msg;
	struct iovec	iov[1];
	char			iov_buffer[RECV_BUFF_SIZE];
	char			ctl_buffer[RECV_BUFF_SIZE];

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

struct timeval	receive_echo_reply(int verbose)
{
	ssize_t			ret;
	struct msghdr	msg;
	struct timeval	tv_after;

	msg = create_msghdr();
	ret = recvmsg(g_sock_fd, &msg, 0);
	gettimeofday(&tv_after, NULL);
	if (ret > 0)
		print_recv(msg, verbose, ret);
	else
		tv_after.tv_sec = ret;
	dprintf(1, "recv ret: %ld\n", ret);
	return (tv_after);
}
