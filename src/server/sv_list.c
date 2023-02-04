/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_list.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/06/02 02:42:18 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/12/31 23:29:07 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include "ft_snprintf.h"
#include "sv_main.h"

static void	sv_send_chaninfo(t_chan *chan, t_client *client, t_server *server)
{
	char	msg[BUFF];

	ft_snprintf(msg,
		sizeof(msg),
		":%s 322 %s %s %d :%s" END_CHECK,
		server->name,
		client->inf->nick,
		chan->name,
		chan->nbusers,
		chan->topic);
	sv_cl_write(msg, client);
}

static void	sv_search_safechan(char *chan_name, t_client *client, t_server *server)
{
	t_chan	*chan;
	t_chan	*found;
	int		len;

	chan = server->chans;
	found = NULL;
	while (chan)
	{
		len = ft_strlen(chan->name);
		if (sv_strncmp(chan->name, chan_name, len - 5) == chan->name[len - 5])
		{
			if (is_chan_member(chan, client)
			|| (!(chan->cmode & CHFL_PRIV) && !(chan->cmode & CHFL_SECRET)))
			{
				if (found)
					return (sv_err(ERR_TOOMANYTARGETS, client, chan_name));
			}
			found = chan;
		}
		chan = chan->next;
	}
	if (!found)
		sv_err(ERR_NOSUCHSERVER, client, chan_name);
	else
		sv_send_chaninfo(found, client, server);
}

static void	sv_search_chan(char *chan_name, t_client *client, t_server *server)
{
	t_chan	*chan;

	chan = server->chans;
	while (chan)
	{
		if (sv_strcmp(chan->name, chan_name) == 0)
		{
			if (is_chan_member(chan, client)
			|| (!(chan->cmode & CHFL_PRIV) && !(chan->cmode & CHFL_SECRET)))
				sv_send_chaninfo(chan, client, server);
			return ;
		}
		chan = chan->next;
	}
	sv_err(ERR_NOSUCHSERVER, client, chan_name);
}

static void	sv_list_specific_chan(char *chan_list, t_client *client, t_server *server)
{
	char	**chans;
	int		i;

	if ((chans = ft_strsplit(chan_list, ',')) == NULL)
		return (sv_error(LOG_LEVEL_FATAL, "ERROR: SERVER: out of memory", server));
	i = 0;
	while (chans[i])
	{
		if (*chans[i] == '!')
			sv_search_safechan(chans[i], client, server);
		else
			sv_search_chan(chans[i], client, server);
		i++;
	}
	ft_free(&chans);
}

void		sv_list(char **cmds, t_server *server, t_client *client)
{
	char	msg[BUFF];
	t_chan	*chan;

	chan = server->chans;
	if (cmds[0])
		sv_list_specific_chan(*cmds, client, server);
	else
	{
		while (chan)
		{
			if ((!(chan->cmode & CHFL_PRIV) && !(chan->cmode & CHFL_SECRET)) ||
				is_chan_member(chan, client))
				sv_send_chaninfo(chan, client, server);
			chan = chan->next;
		}
	}
	ft_snprintf(msg,
		sizeof(msg),
		":%s 323 %s :End of /LIST" END_CHECK,
		server->name,
		client->inf->nick);
	sv_cl_write(msg, client);
}
