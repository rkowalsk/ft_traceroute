#include "ft_traceroute.h"

int	iswhitespace(const char c)
{
	if (c == ' ' || c == '\a' || c == '\b' || c == '\t' || c == '\n'
										|| c == '\v' || c == '\f' || c == '\r')
		return (1);
	return (0);
}

int	parse_num_arg(const char *arg, int *result)
{
	int	i;
	int	start;

	if (!ft_strlen(arg))
		return (1);
	i = 0;
	while (iswhitespace(arg[i]))
		i++;
	if (!arg[i])
		return (1);
	start = i;
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (1);
		i++;
	}
	*result = ft_atoi(arg + start);
	return (0);
}

int	parse_parameter(int argc, char **argv, int *i, struct s_params *params)
{
	int	arg = 0;
	if (!ft_strcmp(argv[*i], "--help"))
	{
		params->help = 1;
		(*i)++;
		return (0);
	}
	else if (!ft_strcmp(argv[*i], "-f"))
	{
		(*i)++;
		if (*i >= argc)
		{
			print_missing_arg(*i - 1, 'f', "first_ttl");
			return (1);
		}
		if (parse_num_arg(argv[*i], &arg))
		{
			print_bad_arg(*i, 'f', argv[*i]);
			return (1);
		}
		params->first_ttl = arg;
		return (0);
	}
	else if (!ft_strcmp(argv[*i], "-m"))
	{
		(*i)++;
		if (*i >= argc)
		{
			print_missing_arg(*i - 1, 'm', "max_ttl");
			return (1);
		}
		if (parse_num_arg(argv[*i], &arg))
		{
			print_bad_arg(*i, 'm', argv[*i]);
			return (1);
		}
		params->max_ttl = arg;
		return (0);
	}
	else if (!ft_strcmp(argv[*i], "-q"))
	{
		(*i)++;
		if (*i >= argc)
		{
			print_missing_arg(*i - 1, 'q', "nqueries");
			return (1);
		}
		if (parse_num_arg(argv[*i], &arg))
		{
			print_bad_arg(*i, 'q', argv[*i]);
			return (1);
		}
		params->nqueries = arg;
		return (0);
	}
	else if (!ft_strcmp(argv[*i], "-N"))
	{
		(*i)++;
		if (*i >= argc)
		{
			print_missing_arg(*i - 1, 'N', "squeries");
			return (1);
		}
		if (parse_num_arg(argv[*i], &arg))
		{
			print_bad_arg(*i, 'N', argv[*i]);
			return (1);
		}
		params->squeries = arg;
		return (0);
	}
	else if (!ft_strcmp(argv[*i], "-p"))
	{
		(*i)++;
		if (*i >= argc)
		{
			print_missing_arg(*i - 1, 'p', "port");
			return (1);
		}
		if (parse_num_arg(argv[*i], &arg))
		{
			print_bad_arg(*i, 'p', argv[*i]);
			return (1);
		}
		params->port = arg;
		return (0);
	}
	else if (!ft_strcmp(argv[*i], "-z"))
	{
		(*i)++;
		if (*i >= argc)
		{
			print_missing_arg(*i - 1, 'z', "sendwait");
			return (1);
		}
		if (parse_num_arg(argv[*i], &arg))
		{
			print_bad_arg(*i, 'z', argv[*i]);
			return (1);
		}
		params->sendwait = arg;
		return (0);
	}
	else
	{
		print_unknown_param(argv[*i], *i);
		return (1);
	}
}

void	parse_destination(char *arg, struct s_params *params)
{
	if (params->destination == NULL)
		params->destination = arg;
}

void	default_values(struct s_params *params)
{
	params->first_ttl = 1;
	params->max_ttl = 30;
	params->squeries = 16;
	params->port = 33434;
	params->nqueries = 3;
	params->sendwait = 0;
	params->destination = NULL;
	params->help = 0;
}

int	check_params(struct s_params params)
{
	if (params.help)
		return (0);
	else if (params.first_ttl > params.max_ttl || params.first_ttl < 1)
	{
		dprintf(2, "first hop out of range\n");
		return (1);
	}
	else if (params.max_ttl > 255 || params.max_ttl < 1)
	{
		dprintf(2, "max hops cannot be more than 255\n");
		return (1);
	}
	else if (params.nqueries > 10 || params.nqueries < 1)
	{
		dprintf(2, "no more than 10 probes per hop\n");
		return (1);
	}
	return (0);
}

void	print_params(struct s_params params)
{
	dprintf(1, "dest: %s\n", params.destination);
	dprintf(1, "first_ttl: %d\n", params.first_ttl);
	dprintf(1, "max_ttl: %d\n", params.max_ttl);
	dprintf(1, "nqueries: %d\n", params.nqueries);
	dprintf(1, "help: %d\n", params.help);
}

int	parse_input(int argc, char **argv, struct s_params *params)
{
	int	parse_ret;
	int	i;

	parse_ret = 0;
	default_values(params);
	i = 1;
	while (i < argc)
	{
		if (argv[i][0] == '-')
			parse_ret = parse_parameter(argc, argv, &i, params);
		else
			parse_destination(argv[i], params);
		if (parse_ret)
			return (1);
		i++;
	}
	if (i == 1)
		params->help = 1;
	else if (params->sendwait > 0)
		params->squeries = 1;
	return (check_params(*params));
}