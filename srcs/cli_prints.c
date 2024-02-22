#include "ft_traceroute.h"

void	print_usage(void)
{
	printf("Usage:\n");
	printf("	ft_traceroute [-f first_ttl] [-m max_ttl] [-N squeries] [-p port] [-q nqueries] host\n");
	printf("\nOptions:\n");
	printf("  -f first_ttl  Start from the first_ttl hop (default is 1)\n");
	printf("  -m max_ttl    ");
	printf("Set the max number of hops (max TTL to be reached) (default is 30)\n");
	printf("  -N squeries   ");
	printf("Set the number of probes to be tried simultaneously (default is 16)\n");
	printf("  -q nqueries   ");
	printf("Set the number of probes per each hop (default is 3)\n");
	printf("  -p port       ");
	printf("Set the destination port to use (incremented by each probe, default is 33434)\n");
	printf("  -z sendwait   ");
	printf("Minimal time interval between probes (default 0).\n");
	printf("                Automatically sets squeries to 1, overriding any value set by -N.\n");
	printf("                If the value of sendwait is more than 10, then it specifies a\n");
	printf("                number in milliseconds, else it is a number of seconds\n");
}

void	print_unknown_param(char *unknown_param, int argc)
{
	dprintf(2, "Bad option `%s' (argc %d)\n", unknown_param, argc);
}

void	print_missing_host(void)
{
	dprintf(2, "Specify \"host\" missing argument.\n");
}

void	print_missing_arg(int curr_argc, const char option, const char *arg)
{
	dprintf(2, "Option `-%c' (argc %d) requires an argument: `-%c %s'\n",
												option, curr_argc, option, arg);
}

void	print_bad_arg(int curr_argc, const char option, const char *arg)
{
	dprintf(2, "Cannot handle `-%c' option with arg `%s' (argc %d)\n",
														option, arg, curr_argc);
}
