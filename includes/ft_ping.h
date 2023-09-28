#pragma once

#include "libft.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

struct	s_input
{
	char	*destination;
	int		verbose;
	int		help;
};

int		parse_input(int argc, char **argv, struct s_input *params);
void	print_usage(void);