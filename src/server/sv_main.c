/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/12 14:48:27 by gbourgeo          #+#    #+#             */
/*   Updated: 2023/06/03 11:17:44 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/resource.h>
#include <signal.h>
#include <time.h>
#include "sv_main.h"

t_server	g_server;

static int	parse_options_error(char illegal_option, char *prog_name, char *port)
{
	ft_putstr_fd(prog_name, 2);
	if (illegal_option)
	{
		ft_putstr_fd(": Illegal option -- [", 2);
		ft_putchar_fd(illegal_option, 2);
		ft_putstr_fd("]\n", 2);
	}
	else if (port == NULL)
		ft_putstr_fd(": Missing parameter\n", 2);
	else
		ft_putstr_fd(": Too many parameters\n", 2);
	return (1);
}

static void	usage(char *prog_name)
{
	ft_putstr_fd("Usage: ", 2);
	ft_putstr_fd(prog_name, 2);
	ft_putendl_fd(" [-v] [-c conf_file] PORT", 2);
	ft_putendl_fd("\t-v\t\tServer log level (default 0).", 2);
	ft_putendl_fd("\t-c\t\tConfiguration file (default "CONF_FILE").", 2);
	ft_putendl_fd("\tPORT\t\tPort for incoming connections (default "DEF_PORT").", 2);
}

static int	parse_options(char *prog_name, char **args, t_server *server)
{
	while (*args)
	{
		if (ft_strncmp((*args), "-v", 2) == 0)
		{
			if ((*args)[2] != '\0')
				server->verbose = ft_atoi(&(*args)[2]);
			else
				server->verbose = ft_atoi((*++args));
		}
		else if (ft_strcmp((*args), "-c") == 0)
			server->conf.conf_file = (*++args);
		else if (ft_isdigit((*args)[0]) && !server->port)
			server->port = (*args);
		else
			return (parse_options_error(0, prog_name, server->port));
		args++;
	}
	if (!server->port)
		server->port = DEF_PORT;
	return (0);
}

int			main(int ac, char **av)
{
	struct rlimit	rlp;
	time_t			date;
	int				ret;

	(void)ac;
	ret = 1;
	errno = 0;
	ft_memset(&g_server, 0, sizeof(g_server));
	g_server.conf.conf_file = CONF_FILE;
	if (parse_options(av[0], &av[1], &g_server) != 0)
		usage(av[0]);
	else if (getrlimit(RLIMIT_NOFILE, &rlp) == -1)
		sv_log(LOG_LEVEL_FATAL, LOG_TYPE_SYSTEM, "getrlimit: %s", strerror(errno));
	else if (MAX_CLIENT > rlp.rlim_cur)
		sv_log(LOG_LEVEL_FATAL, LOG_TYPE_SYSTEM, "MAX_CLIENT is greater than rlim_cur");
	else if (sv_parse_conf_file(&g_server) == 0
	&& sv_init_server(&g_server.v4, &g_server.v6, g_server.addr, g_server.port) == 0)
	{
		sv_signals();
		date = time(NULL);
		g_server.creation = ctime(&date);
		ret = sv_loop(&g_server);
	}
	sv_close_server(&g_server);
	return (ret);
}
