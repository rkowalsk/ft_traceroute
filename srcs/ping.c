#include "ft_ping.h"

void	store_time(float diff)
{
	g_stats.times += diff;
	g_stats.times_sq += (diff * diff);
	if (g_stats.received == 0 || diff < g_stats.min)
		g_stats.min = diff;
	if (g_stats.received == 0 || diff > g_stats.max)
		g_stats.max = diff;
	g_stats.received++;
}

struct s_stats	initialize_stats(void)
{
	struct s_stats	stats =
	{
		.transmitted = 0,
		.received = 0,
		.times = 0,
		.times_sq = 0,
		.min = 0,
		.max = 0,
	};
	return (stats);
}

int	ping_loop(int verbose)
{
	float	diff;

	if (signal(SIGALRM, &send_echo_request) == SIG_ERR)
		return (perror_ret("signal"));
	g_stats = initialize_stats();
	send_echo_request(666);
	while (1)
	{
		diff = receive_echo_reply(verbose);
		// dprintf(1, "received\n");
		if (diff > 0)
			store_time(diff);
	}
	return (0);
}
