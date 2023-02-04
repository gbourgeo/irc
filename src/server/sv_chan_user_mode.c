/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_chan_user_mode.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/19 05:43:46 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/12/11 15:24:31 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sv_main.h"

static t_client			*did_user_exist(char *nick, t_server *server)
{
	t_client			*clients;

	clients = server->clients;
	while (clients)
	{
		if (!sv_strcmp(clients->inf->nick, nick))
			break ;
		clients = clients->next;
	}
	return (clients);
}

static t_listing		*search_user(t_client *client, t_chan *channel)
{
	t_listing		*clients;

	clients = channel->users;
	while (clients)
	{
		if (((t_client *)clients->is)->socket.fd == client->socket.fd)
			break ;
		clients = clients->next;
	}
	return (clients);
}

static t_listing		*search_channel(t_client *client, t_chan *channel)
{
	t_listing		*channels;

	channels = client->chans;
	while (channels)
	{
		if (!sv_strcmp(((t_chan *)channels->is)->name, channel->name))
			break ;
		channels = channels->next;
	}
	return (channels);
}

static void			sv_sendtochan(t_grp *group, t_chan *channel, t_client *client)
{
	t_listing		*users;
	t_client			*to;

	users = channel->users;
	while (users)
	{
		to = (t_client *)users->is;
		rpl_umode(group, channel, to, client);
		users = users->next;
	}
}

void				sv_chan_user_mode(t_grp *group, char ***cmd, t_server *server)
{
	static int		chan_nbr[] = { CH_MODS1, CH_MODS2, CH_MODS3 };
	t_listing		*ch;
	char			*tmp;

	if (*(*cmd + 1) == NULL)
		return ;
	tmp = ft_strchr(CHAN_MODES, *group->ptr);
	(*cmd)++;
	if ((group->to = did_user_exist(**cmd, server)) == NULL)
		return (sv_err(ERR_NOSUCHNICK, group->from, **cmd));
	if ((group->list = search_user(group->to, group->on)) == NULL)
		return (sv_err(ERR_USERSDONTMATCH, group->from, **cmd, group->on->name));
	ch = search_channel(group->list->is, group->on);
	if (group->c)
	{
		group->list->mode |= chan_nbr[tmp - CHAN_MODES];
		ch->mode |= chan_nbr[tmp - CHAN_MODES];
	}
	else
	{
		group->list->mode &= ~(chan_nbr[tmp - CHAN_MODES]);
		ch->mode &= ~(chan_nbr[tmp - CHAN_MODES]);
	}
	sv_sendtochan(group, group->on, group->from);
}
