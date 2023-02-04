/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_chan_member.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/23 12:05:46 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/10/21 13:53:21 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sv_main.h"

int				is_chan_member(t_chan *ch, t_client *client)
{
	t_listing	*list;
	t_client	*user;

	list = ch->users;
	while (list)
	{
		user = (t_client *)list->is;
		if (user->socket.fd == client->socket.fd)
			return (1);
		list = list->next;
	}
	return (0);
}

int				is_modo(t_chan *chan, t_client *client)
{
	t_listing	*list;
	t_client	*user;

	list = chan->users;
	while (list)
	{
		user = (t_client *)list->is;
		if (user->socket.fd == client->socket.fd)
			return (list->mode & CHFL_CHANOP);
		list = list->next;
	}
	return (0);
}
