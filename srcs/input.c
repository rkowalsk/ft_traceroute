#include "ft_ping.h"

void	print_usage(void)
{
	printf("Usage: ft_ping [OPTION...] HOST ...\n");
	printf("Send ICMP ECHO_REQUEST packets to network hosts.\n");
	printf("\nOptions:\n");
	printf("  -h, -?\tprint help and exit\n");
	printf("  -v    \tverbose output\n");
}

void	print_unknown_param(char unknown_param)
{
	dprintf(2, "ping : invalid option -- '%c'\n", unknown_param);
	dprintf(2, "Try 'ping -h' or 'ping -?' for more information.\n");
}

void	print_missing_host()
{
	dprintf(2, "ping: missing host operand\n");
	dprintf(2, "Try 'ping -h' or 'ping -?' for more information.\n");
}

int	parse_parameter(char *arg, struct s_input *params)
{
	int	i;
	int	ret;

	ret = 0;
	i = 1;
	while (arg[i])	
	{
		if (arg[i] == '?' || arg[i] == 'h')
			params->help = 1;
		else if (arg[i] == 'v')
			params->verbose = 1;
		else
		{
			print_unknown_param(arg[i]);
			params->help = 1;
			ret = 1;
			break;
		}
		i++;
	}
	if (i == 1)
		params->help = 1;
	// dprintf(1, "parameter: '%s'\n", arg);
	return (ret);
}

void	parse_destination(char *arg, struct s_input *params)
{
	if (params->destination == NULL)
		params->destination = arg;
	dprintf(1, "destination: '%s'\n", arg);
}

int	parse_input(int argc, char **argv, struct s_input *params)
{
	int	parse_ret;
	int	i;

	parse_ret = 0;
	params->destination = NULL;
	params->verbose = 0;
	params->help = 0;
	i = 1;
	while (i < argc)
	{
		if (argv[i][0] == '-')
			parse_ret = parse_parameter(argv[i], params);
		else
			parse_destination(argv[i], params);
		if (parse_ret)
			return (1);
		i++;
	}
	if (!(params->help) && !(params->destination))
	{
		print_missing_host();
		return (1);
	}
	return (0);
}