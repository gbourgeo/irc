/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_topic.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/30 10:00:47 by gbourgeo          #+#    #+#             */
/*   Updated: 2023/01/01 01:28:26 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include "ft_snprintf.h"
#include "sv_main.h"

static void		rpl_topic(t_chan *chan, t_client *client, t_server *server)
{
	char	msg[BUFF];

	ft_snprintf(msg,
		sizeof(msg),
		":%s %s %s %s :%s" END_CHECK,
		server->name,
		(*chan->topic) ? " 332 " : " 331 ",
		client->inf->nick,
		chan->name,
		(*chan->topic) ? chan->topic : "No topic is set");
	sv_cl_write(msg, client);
}

static void		sv_rpl_topic_user(t_chan *chan, t_client *client)
{
	t_listing	*user;
	t_client	*to;
	char		msg[BUFF];

	ft_snprintf(msg,
		sizeof(msg),
		":%s!~%s@%s TOPIC %s :%s" END_CHECK,
		client->inf->nick,
		client->inf->username,
		(*client->socket.host) ? client->socket.host : client->socket.addr,
		chan->name,
		chan->topic);
	user = chan->users;
	while (user)
	{
		to = (t_client *)user->is;
		sv_cl_write(msg, to);
		user = user->next;
	}
}

static void		sv_set_topic(char **topic, t_chan *chan, t_client *client, t_server *server)
{
	int		i;
	int		len;

	i = 1;
	len = TOPIC_LEN - 1;
	if (!topic || *topic[0] != ':')
		return (rpl_topic(chan, client, server));
	ft_bzero(chan->topic, TOPIC_LEN);
	ft_strncat(chan->topic, topic[0] + 1, len);
	len -= ft_strlen(topic[0] + 1);
	while (topic[i] && len > 0)
	{
		ft_strncat(chan->topic, " ", len--);
		ft_strncat(chan->topic, topic[i], len);
		len -= (ft_strlen(topic[i]));
		i++;
	}
	sv_rpl_topic_user(chan, client);
}

/**
 * @brief change or view the topic of the given channel. If <topic> is not
 * given, either RPL_TOPIC or RPL_NOTOPIC is returned specifying the current
 * channel topic or lack of one. If <topic> is an empty string, the topic for
 * the channel will be cleared.
 * 
 * @param cmds <channel> [<topic>]
 * @param server Server
 * @param client Client
 */
void			sv_topic(char **cmds, t_server *server, t_client *client)
{
	t_listing	*chan;
	t_chan		*ch;

	if (!cmds[0] || !*cmds[0])
		return (sv_err(ERR_NEEDMOREPARAMS, client, "TOPIC"));
	if (!ISCHAN(*cmds[0]))
		return (sv_err(ERR_NOSUCHCHANNEL, client, cmds[0]));
	chan = client->chans;
	while (chan)
	{
		ch = (t_chan *)chan->is;
		if (!sv_strcmp(ch->name, cmds[0]))
		{
			if (cmds[1] && (ch->cmode & CHFL_TOPIC &&
							!(chan->mode & CHFL_CHANOP)))
				return (sv_err(ERR_CHANOPRIVSNEEDED, client, cmds[0]));
			else if (!(ch->cmode & CHFL_SECRET))
				return (sv_set_topic(cmds + 1, chan->is, client, server));
		}
		chan = chan->next;
	}
	sv_err(ERR_NOTONCHANNEL, client, cmds[0]);
}
