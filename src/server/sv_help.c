/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_help.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/02 10:00:20 by gbourgeo          #+#    #+#             */
/*   Updated: 2023/01/01 02:48:01 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_snprintf.h"
#include "commands.h"
#include "sv_main.h"

extern t_command	g_commands[];
extern size_t		g_commands_size;

static void	rpl_help(char *num, char *txt, t_client *client, t_server *server)
{
	char	msg[BUFF];

	ft_snprintf(msg, sizeof(msg), ":%s %s %s %s :%s" END_CHECK,
		server->name, num, client->inf->nick, server->ptr, txt);
	sv_cl_write(msg, client);
}

static void	sv_explain(int nb, t_client *client, t_server *server)
{
	static char	*msg[][80] = { HELP1, HELP2, HELP3 };
	int			pos;

	pos = 0;
	while (msg[nb][pos])
	{
		rpl_help("705", msg[nb][pos], client, server);
		pos++;
	}
	rpl_help("705", "", client, server);
}

static void	sv_specific_help(char **cmds, t_client *client, t_server *server)
{
	static char	*s[] = { SYNTAX1, SYNTAX2 };
	size_t		nb;

	while (*cmds)
	{
		nb = 0;
		while (nb < g_commands_size && sv_strcmp(g_commands[nb].name, *cmds))
			nb++;
		if (nb == g_commands_size)
			rpl_help("524", ":Help not found", client, server);
		else
		{
			server->ptr = *cmds;
			rpl_help("704", s[nb], client, server);
			rpl_help("705", "", client, server);
			sv_explain(nb, client, server);
		}
		cmds++;
	}
}

void		sv_help(char **cmds, t_server *server, t_client *client)
{
	size_t	nb;

	nb = 0;
	if (!cmds[0] || !*cmds[0])
	{
		server->ptr = "index";
		rpl_help("704", "Help commands available to users:", client, server);
		rpl_help("705", "", client, server);
		while (nb < g_commands_size)
		{
			if (g_commands[nb].privilege == 0 || client->inf->registered)
				rpl_help("705", g_commands[nb].name, client, server);
			nb++;
		}
		rpl_help("705", "Type HELP [<command>] to get help about a ", client, server);
		rpl_help("705", "specific command.", client, server);
	}
	else
		sv_specific_help(cmds, client, server);
	rpl_help("706", "End of /HELP", client, server);
	server->ptr = NULL;
}
