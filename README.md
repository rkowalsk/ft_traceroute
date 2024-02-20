# ft_traceroute

## Description

ft_traceroute is a project undertaken at 42 School that involves re-coding the traceroute command.

## Features

This implementation of traceroute uses TCP packets for probes and raw sockets to receive ICMP responses and therefore must be run as root.

## How to Use

You will need `make` and `gcc` to compile.

1. Clone the repository:

   ```bash
   git clone https://github.com/rkowalsk/ft_traceroute.git
   ```

2. Navigate to the project directory:

   ```bash
   cd ft_traceroute
   ```

3. Compile the program:

   ```bash
   make
   ```

4. Run the ft_traceroute command as root:

   ```bash
   ./ft_traceroute [options] host
   ```

   Example:

   ```bash
   sudo ./ft_traceroute fsf.org
   ```

## Options

- `-f first_ttl` Start from the first_ttl hop (default is 1)
- `-m max_ttl` Set the max number of hops (max TTL to be reached) (default is 30)
- `-N squeries` Set the number of probes to be tried simultaneously (default is 16)
- `-q nqueries` Set the number of probes per each hop (default is 3)
- `-p port` Set the destination port to use (incremented by each probe, default is 33434)
- `-z sendwait` Minimal time interval between probes (default 0). If the value is more than 10, then it specifies a number in milliseconds, else it is a number of seconds
- `--help` Show the help
