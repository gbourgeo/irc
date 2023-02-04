/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_nick.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/10 13:43:30 by gbourgeo          #+#    #+#             */
/*   Updated: 2023/01/01 01:58:15 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <ft_snprintf.h>
#include "sv_main.h"

int				sv_check_name_valid(char *name)
{
	char		*tmp;

	if (!name || !*name)
		return (1);
	if (!sv_strcmp(name, "anonymous"))
		return (2);
	if (ft_isalpha(*name) || ISSPECIAL(*name))
	{
		tmp = name + 1;
		while (*tmp && tmp - name < NICK_LEN)
		{
			if (ft_isalpha(*tmp) || ft_isdigit(*tmp) || ISSPECIAL(*tmp) ||
				*tmp == '-')
				tmp++;
			else
				return (2);
		}
		return (0);
	}
	return (2);
}

static int		is_connected_nick(char *nick, t_client *client, t_server *server)
{
	t_file		*user;
	t_client	*cls;

	cls = server->clients;
	while (cls)
	{
		if (cls->socket.fd != client->socket.fd &&
			!sv_strncmp(nick, cls->inf->nick, NICK_LEN))
			return (1);
		cls = cls->next;
	}
	user = server->users;
	while (client->inf->registered > 0 && user)
	{
		if (!sv_strncmp(nick, user->nick, NICK_LEN))
			return (1);
		user = user->next;
	}
	return (0);
}

static void		rpl_nick(char *nick, t_client *to, t_client *client)
{
	char	msg[BUFF];

	ft_snprintf(msg,
		sizeof(msg),
		":%s!~%s@%s NICK :%s" END_CHECK,
		client->inf->nick,
		client->inf->username,
		(*client->socket.host) ? client->socket.host : client->socket.addr,
		nick);
	sv_cl_write(msg, to);
}

static void		send_to_chans(char *nick, t_client *cl)
{
	t_listing	*ch;
	t_listing	*us;

	ch = cl->chans;
	while (ch)
	{
		if (!(((t_chan *)ch->is)->cmode & CHFL_QUIET) &&
			!(((t_chan *)ch->is)->cmode & CHFL_ANON))
		{
			us = ((t_chan *)ch->is)->users;
			while (us)
			{
				if (((t_client *)us->is)->socket.fd != cl->socket.fd)
					rpl_nick(nick, us->is, cl);
				us = us->next;
			}
		}
		ch = ch->next;
	}
}

/**
 * @brief Give the client a nickname or change the previous one.
 * 
 * @param cmds <nickname>
 * @param server Server
 * @param client Client
 */
void			sv_nick(char **cmds, t_server *server, t_client *client)
{
	int			err;

	err = sv_check_name_valid(*cmds);
	if (err == 1)
		return (sv_err(ERR_NONICKNAMEGIVEN, client, "NICK"));
	if (err == 2)
		return (sv_err(ERR_ERRONEUSNICKNAME, client, *cmds));
	if (is_connected_nick(*cmds, client, server))
		return (sv_err(ERR_NICKNAMEINUSE, client, *cmds));
	if (client->inf->umode & USR_RESTRICT)
		return (sv_err(ERR_RESTRICTED, client));
	if (client->inf->registered <= 0)
		ft_strncpy(client->inf->nick, *cmds, NICK_LEN);
	else if (sv_strcmp(client->inf->nick, *cmds))
	{
		rpl_nick(*cmds, client, client);
		send_to_chans(*cmds, client);
		ft_strncpy(client->inf->nick, *cmds, NICK_LEN);
		client->inf->must_change_nick = 0;
	}
}
