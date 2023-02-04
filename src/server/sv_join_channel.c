/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_join_channel.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/17 04:48:15 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/12/31 22:27:47 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <time.h>
#include "sv_main.h"

static int		sv_channel_ident(char *name, t_chan *chan, t_client *client)
{
	time_t	timed;
	u_char	*timed_ptr;
	char	*base;
	int		i[2];

	if (time(&timed) == -1)
	{
		sv_err(ERR_CREATECHANNELFAIL, client, name);
		return (0);
	}
	timed_ptr = (u_char *)&timed;
	base = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
	if ((i[0] = ft_strlen(name)) > CHANNAME_LEN - 5)
		i[0] = CHANNAME_LEN - 5;
	i[1] = i[0] + 5;
	while (i[0] < i[1])
		chan->name[i[0]++] = base[*timed_ptr++ % 36];
	return (1);
}

/*
** Afficher les nouveaux mode du chan si on implemente la commande MODE
** chan->cmode |= (*chan->name == '+') ? CHFL_TOPIC : CHFL_NOMSG | CHFL_SECRET;
*/
static t_chan	*sv_new_chan(char *name, t_client *client, t_server *server)
{
	t_chan	*chan;

	if ((chan = (t_chan *)ft_memalloc(sizeof(*chan))) == NULL)
	{
		sv_error(LOG_LEVEL_FATAL, "Out of memory", server);
		return (NULL);
	}
	ft_strncpy(chan->name, name, CHANNAME_LEN);
	if (*name == '!' && !sv_channel_ident(name, chan, client))
	{
		free(chan);
		return (NULL);
	}
	chan->users = sv_add_usertochan(client, chan, server);
	if (*chan->name == '!')
		chan->users->mode |= CHFL_CREATOR;
	if (*chan->name != '+')
		chan->users->mode |= CHFL_CHANOP;
	chan->next = server->chans;
	return (chan);
}

static int		sv_check_safe_chan(char *name, t_client *client, t_server *server)
{
	t_chan	*chans;
	int		len;

	chans = server->chans;
	while (chans)
	{
		len = ft_strlen(chans->name);
		if (*chans->name == '!' &&
			sv_strncmp(chans->name, name, len - 5) == chans->name[len - 5])
		{
			sv_err(ERR_TOOMANYTARGETS, client, name);
			return (0);
		}
		chans = chans->next;
	}
	return (1);
}

static void		sv_check_chan_modes(t_chan *chan, char ***keys, t_client *client, t_server *server)
{
	char	*cmd[2];

	cmd[0] = chan->name;
	cmd[1] = NULL;
	if (chan->cmode & CHFL_KEY)
	{
		if (!*keys || ft_strcmp(*(*keys++), chan->key) != 0)
		{
			sv_err(ERR_BADCHANNELKEY, client, chan->name);
			return ;
		}
	}
	if (chan->cmode & CHFL_LIMIT)
	{
		if (chan->nbusers + chan->invisibl >= chan->limit)
		{
			sv_err(ERR_CHANNELISFULL, client, chan->name);
			return ;
		}
	}
	chan->users = sv_add_usertochan(client, chan, server);
	client->chans = sv_add_chantouser(chan, client, server);
	send_joinmsg_toothers(chan, client);
	sv_who(cmd, server, client);
}

/**
 * @brief Join a channel if it already exist or create it.
 * 
 * @param chan_name Name of the channel
 * @param keys Key to the channel
 * @param client Client
 * @param server Server
 */
void			sv_join_chan(char *chan_name, char ***keys, t_client *client, t_server *server)
{
	t_chan	*chans;
	char	*cmd[2];

	chans = server->chans;
	while (chans)
	{
		if (!sv_strcmp(chan_name, chans->name))
			return (sv_check_chan_modes(chans, keys, client, server));
		chans = chans->next;
	}
	if (*chan_name == '!' && !sv_check_safe_chan(chan_name, client, server))
		return ;
	if ((chans = sv_new_chan(chan_name, client, server)) == NULL)
		return ;
	if (server->chans)
		server->chans->prev = chans;
	server->chans = chans;
	if ((client->chans = sv_add_chantouser(server->chans, client, server)) == NULL)
		return ;
	client->chansnb++;
	if (*chan_name == '!')
		client->chans->mode |= CHFL_CREATOR;
	if (*chan_name != '+')
		client->chans->mode |= CHFL_CHANOP;
	send_joinmsg_toothers(client->chans->is, client);
	cmd[0] = chan_name;
	cmd[1] = NULL;
	sv_who(cmd, server, client);
}
