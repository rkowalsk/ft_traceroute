#include "ft_traceroute.h"

struct s_probe **initialize_results(struct s_params params)
{
	struct s_probe	**results;
	unsigned int	size;
	unsigned int	i;

	size = params.max_ttl - params.first_ttl + 1;
	results = malloc(sizeof(struct s_probe *) * size);
	if (!results)
	{
		dprintf(2, "malloc: %s\n", strerror(errno));
		return (NULL);
	}
	i = 0;
	while (i < size)
	{
		results[i] = calloc(params.nqueries, sizeof(struct s_probe));
		if (!results[i])
		{
			dprintf(2, "calloc: %s\n", strerror(errno));
			free_results(results, i);
			return (NULL);
		}
		i++;
	}
	return (results);
}

int main(int argc, char **argv)
{
	struct s_params	params;
	struct s_net	net;
	struct s_probe	**results;
	int				ret;

	if (parse_input(argc, argv, &params))
		return (1);
	if (params.help)
	{
		print_usage();
		return (0);
	}
	ret = create_socket(params, &net);
	if (ret)
		return (ret);
	print_first_line(*net.s_info, params.max_ttl);
	results = initialize_results(params);
	if (!results)
	{
		free_s_net(net);
		return (2);
	}
	trace_the_route(results, net, &params);
	free_results(results, params.max_ttl - params.first_ttl + 1);
	return (free_s_net(net));
}