#include "ft_ping.h"

void	stats_and_exit(int signal)
{
	size_t	loss;
	float	stddev;
	float	avg;

	(void) signal;
	dprintf(1, "--- %s ping statistics ---\n", g_servinfos->ai_canonname);
	loss = ((float) g_stats.received / (float) g_stats.transmitted) * 100;
	dprintf(1,	"%zu packets transmitted, %zu packets received,",
										g_stats.transmitted, g_stats.received);
	dprintf(1, " %zu%% package loss\n", loss);
	if (g_stats.received > 0)
	{
		avg = g_stats.times / (float)g_stats.received;
		stddev = (g_stats.times_sq / (float) g_stats.received) - (avg * avg);
		stddev = sqrt(stddev);
		dprintf(1, "round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f\n",
			g_stats.min, avg, g_stats.max, stddev);
	}
	exit(free_all(g_sock_fd, g_servinfos));
}
