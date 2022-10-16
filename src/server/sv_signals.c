/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/12 00:27:16 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/10/17 00:02:36 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include "sv_main.h"

extern t_env	g_server;

static void		sv_server_killed(int sig)
{
	if (sig == SIGTERM)
		sv_error("Server Killed By SIGTERM", &g_server);
	if (sig == SIGINT)
		sv_error("Server Killed By SIGINT", &g_server);
	if (sig == SIGBUS)
		sv_error("Server Killed By SIGBUS", &g_server);
	sv_error("Server Killed By A Signal xD", &g_server);
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
