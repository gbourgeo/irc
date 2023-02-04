/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_leave_check.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/22 17:41:53 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/12/31 23:11:31 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_snprintf.h"
#include "sv_main.h"

static void	rpl_leave(char **leave_reason, t_chan *ch, t_client *to, t_client *cl)
{
	char	msg[BUFF];

	if (ch->cmode & CHFL_ANON)
		ft_snprintf(msg,
			sizeof(msg),
			":anonymous!~anonymous@anonymous LEAVE %s",
			ch->name);
	else
		ft_snprintf(msg,
			sizeof(msg),
			":%s!~%s@%s LEAVE %s",
			cl->inf->nick,
			cl->inf->username,
			(*cl->socket.host) ? cl->socket.host : cl->socket.addr,
			ch->name);
	if (leave_reason && *leave_reason)
	{
		ft_strncat(msg, " :", sizeof(msg) - ft_strlen(msg));
		ft_strncat(msg, *leave_reason, sizeof(msg) - ft_strlen(msg));
		while (*++leave_reason)
		{
			ft_strncat(msg, " ", sizeof(msg) - ft_strlen(msg));
			ft_strncat(msg, *leave_reason, sizeof(msg) - ft_strlen(msg));
		}
	}
	ft_strncat(msg, END_CHECK, sizeof(msg) - ft_strlen(msg));
	sv_cl_write(msg, to);
}

static void	sv_send_leavemsg(char **leave_reason, t_chan *chan, t_client *client)
{
	t_listing	*user;

	user = chan->users;
	while (user)
	{
		rpl_leave(leave_reason, chan, (t_client *)user->is, client);
		user = user->next;
	}
}

static void	sv_find_chaninuser(t_chan *chan, t_client *client)
{
	t_listing	*channel;

	channel = client->chans;
	while (channel && sv_strcmp(((t_chan *)channel->is)->name, chan->name))
		channel = channel->next;
	if (channel == NULL)
		return (sv_err(ERR_NOTONCHANNEL, client, chan->name));
	if (channel->prev)
		channel->prev->next = channel->next;
	else
		client->chans = channel->next;
	if (channel->next)
		channel->next->prev = channel->prev;
	channel->is = NULL;
	free(channel);
	channel = NULL;
}

void		sv_find_userinchan(char **leave_reason, t_chan *chan, t_client *client)
{
	t_listing	*user;

	user = chan->users;
	while (user && sv_strcmp(((t_client *)user->is)->inf->nick, client->inf->nick))
		user = user->next;
	if (user == NULL)
		return (sv_err(ERR_NOTONCHANNEL, client, chan->name));
	(client->inf->umode & USR_INVISIBL) ? chan->invisibl-- : chan->nbusers--;
	client->chansnb--;
	if (!(chan->cmode & CHFL_QUIET))
		sv_send_leavemsg(leave_reason, chan, client);
	else
		rpl_leave(leave_reason, chan, client, client);
	if (user->prev)
		user->prev->next = user->next;
	else
		chan->users = user->next;
	if (user->next)
		user->next->prev = user->prev;
	user->is = NULL;
	free(user);
	user = NULL;
	sv_find_chaninuser(chan, client);
}
