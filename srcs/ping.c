#include "ft_ping.h"

float	sub_timevals(struct timeval before, struct timeval after)
{
	float	diff;

	diff = (after.tv_sec - before.tv_sec) * 1000;
	diff += (float)(after.tv_usec - before.tv_usec) / 1000;
	return (diff);
}

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
	struct timeval	tv_before;
	struct timeval	tv_after;
	float			diff;

	g_stats = initialize_stats();
	while (1)
	{
		tv_before = send_echo_request(g_stats.transmitted);
		// dprintf(1, "sent\n");
		// if (tv_before.tv_sec == 0)
		// 	return (1);
		g_stats.transmitted++; // used as sequence
		// dprintf(1, "sent successful\n");
		tv_after = receive_echo_reply(verbose);
		// dprintf(1, "received\n");
		if (tv_after.tv_sec > 0)
		{
			// dprintf(1, "received successful\n");
			diff = sub_timevals(tv_before, tv_after);
			store_time(diff);
			dprintf(1, "time=%.3fms\n", diff);
		}
		if (tv_after.tv_sec != 0)
			usleep(1000000);
	}
	return (0);
}
