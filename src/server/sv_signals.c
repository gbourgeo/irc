/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/12 00:27:16 by gbourgeo          #+#    #+#             */
/*   Updated: 2023/01/01 01:52:40 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include "sv_main.h"

extern t_server	g_server;

static void		sv_server_killed(int sig)
{
	if (sig == SIGTERM)
		sv_error(LOG_LEVEL_FATAL, "Server Killed By SIGTERM", &g_server);
	else if (sig == SIGINT)
		sv_error(LOG_LEVEL_FATAL, "Server Killed By SIGINT", &g_server);
	else if (sig == SIGBUS)
		sv_error(LOG_LEVEL_FATAL, "Server Killed By SIGBUS", &g_server);
	else
		sv_error(LOG_LEVEL_FATAL, "Server Killed By A Signal xD", &g_server);
}

void			sv_signals(void)
{
	signal(SIGWINCH, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGALRM, SIG_IGN);
	signal(SIGINT, sv_server_killed);
	signal(SIGTERM, sv_server_killed);
	signal(SIGBUS, sv_server_killed);
	signal(SIGUSR1, SIG_IGN);
}
