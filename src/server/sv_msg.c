/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_msg.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/06/02 18:01:29 by gbourgeo          #+#    #+#             */
/*   Updated: 2023/01/01 00:22:21 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include "ft_snprintf.h"
#include "sv_main.h"

static void		rpl_msg(char **text_to_send, t_client *to, t_client *client)
{
	char	msg[BUFF];

	ft_snprintf(msg,
		sizeof(msg),
		":%s!~%s@%s MSG %s :%s",
		client->inf->nick,
		client->inf->username,
		(*client->socket.host) ? client->socket.host : client->socket.addr,
		to->inf->nick,
		*text_to_send);
	while (*++text_to_send)
	{
		ft_strncat(msg, " ", sizeof(msg) - ft_strlen(msg));
		ft_strncat(msg, *text_to_send, sizeof(msg) - ft_strlen(msg));
	}
	ft_strncat(msg, END_CHECK, sizeof(msg) - ft_strlen(msg));
	sv_cl_write(msg, to);
}

static void		sv_msg_client(char *nick, char **text_to_send, t_client *client, t_server *server)
{
	t_client	*clients;

	clients = server->clients;
	while (clients)
	{
		if (!sv_strcmp(nick, clients->inf->nick))
		{
			rpl_msg(text_to_send, clients, client);
			if (clients->inf->umode &= USR_AWAY)
				rpl_away(client, clients, server);
			return ;
		}
		clients = clients->next;
	}
	sv_err(ERR_NOSUCHNICK, client, nick);
}

/**
 * @brief Send private messages between users, as well as to send messages to channels.
 * 
 * @param cmds <target>{,<target>} <text to be sent>
 * @param server Server
 * @param client Client
 */
void			sv_msg(char **cmds, t_server *server, t_client *client)
{
	char		**targets;
	int			i;

	if (!cmds[0] || *cmds[0] == '\0')
		return (sv_err(ERR_NORECIPIENT, client, "MSG"));
	if (!cmds[1] || !*cmds[1])
		return (sv_err(ERR_NOTEXTTOSEND, client, "MSG"));
	if ((targets = ft_strsplit(cmds[0], ',')) == NULL)
		return (sv_error(LOG_LEVEL_FATAL, "Out of memory", server));
	i = 0;
	while (targets[i])
	{
		if (ISCHAN(*targets[i]))
			sv_msg_chan(targets[i], cmds + 1, client, server);
		else
			sv_msg_client(targets[i], cmds + 1, client, server);
		i++;
	}
	ft_free(&targets);
}
