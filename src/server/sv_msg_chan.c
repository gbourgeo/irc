/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_msg_chan.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/20 04:05:11 by gbourgeo          #+#    #+#             */
/*   Updated: 2023/01/01 00:32:18 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_snprintf.h"
#include "sv_main.h"

static void	rpl_msg_chan(char **text_to_send, t_chan *chan, t_client *to, t_client *client)
{
	char	msg[BUFF];

	if (chan->cmode & CHFL_ANON)
		ft_snprintf(msg,
			sizeof(msg),
			":anonymous!~anonymous@anonymous MSG %s :%s",
			chan->name,
			*text_to_send);
	else
		ft_snprintf(msg,
			sizeof(msg),
			":%s!~%s@%S MSG %s :%s",
			client->inf->nick,
			client->inf->username,
			(*client->socket.host) ? client->socket.host : client->socket.addr,
			chan->name,
			*text_to_send);
	while (*++text_to_send)
	{
		ft_strncat(msg, " ", sizeof(msg) - ft_strlen(msg));
		ft_strncat(msg, *text_to_send, sizeof(msg) - ft_strlen(msg));
	}
	ft_strncat(msg, END_CHECK, sizeof(msg) - ft_strlen(msg));
	sv_cl_write(msg, to);
}

static void	sv_sendtochan(char **text_to_send, t_chan *chan, t_client *client)
{
	t_listing	*user;
	t_client	*to;

	user = chan->users;
	while (user)
	{
		to = (t_client *)user->is;
		if (to->socket.fd != client->socket.fd)
			rpl_msg_chan(text_to_send, chan, to, client);
		user = user->next;
	}
}

static int	user_got_mod(t_chan *chan, t_client *client)
{
	t_listing	*user;

	user = chan->users;
	while (user)
	{
		if (((t_client *)user->is)->socket.fd == client->socket.fd)
		{
			if (user->mode & CHFL_CHANOP || user->mode & CHFL_VOICE)
				return (1);
			return (0);
		}
		user = user->next;
	}
	return (0);
}

/**
 * @brief Send message to channel
 * 
 * @param chan_name Channel name
 * @param text_to_send Text to send
 * @param client Client
 * @param server Server
 */
void		sv_msg_chan(char *chan_name, char **text_to_send, t_client *client, t_server *server)
{
	t_chan	*chan;

	chan = server->chans;
	while (chan)
	{
		if (!sv_strcmp(chan->name, chan_name))
		{
			if ((chan->cmode & CHFL_MOD && !user_got_mod(chan, client)) ||
				(chan->cmode & CHFL_NOMSG && !is_chan_member(chan, client)))
				break ;
			return (sv_sendtochan(text_to_send, chan, client));
		}
		chan = chan->next;
	}
	sv_err(ERR_CANNOTSENDTOCHAN, client, chan_name);
}
