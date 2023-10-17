#include "ft_ping.h"

int				g_sock_fd;
int				g_verbose;
struct addrinfo	*g_servinfos;
struct s_stats	g_stats;

int main(int argc, char **argv)
{
	struct s_input	params;
	int				ret;

	if (parse_input(argc, argv, &params))
		return (1);
	if (params.help)
	{
		print_usage();
		return (0);
	}
	ret = create_socket(params);
	if (ret)
		return (ret);
	g_verbose = params.verbose;
	print_addrinfo(*g_servinfos);
	if (signal(SIGINT, &stats_and_exit) == SIG_ERR)
	{
		free_all(g_sock_fd, g_servinfos);
		return (perror_ret("signal"));
	}
	ret = ping_loop(params.verbose);
	if (free_all(g_sock_fd, g_servinfos))
		return (2);
	return (ret);
}