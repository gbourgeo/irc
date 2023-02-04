/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_chan.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/26 18:34:44 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/12/31 21:17:35 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sv_main.h"

static void	rpl_join(t_chan *ch, t_client *to, t_client *cl)
{
	sv_cl_write(":", to);
	sv_cl_write((ch->cmode & CHFL_ANON) ? "anonymous" : cl->inf->nick, to);
	sv_cl_write("!~", to);
	sv_cl_write((ch->cmode & CHFL_ANON) ? "anonymous" : cl->inf->username, to);
	sv_cl_write("@", to);
	if (*cl->socket.host)
		sv_cl_write((ch->cmode & CHFL_ANON) ? "anonymous" : cl->socket.host, to);
	else
		sv_cl_write((ch->cmode & CHFL_ANON) ? "anonymous" : cl->socket.addr, to);
	sv_cl_write(" JOIN ", to);
	sv_cl_write(ch->name, to);
	sv_cl_write(END_CHECK, to);
}

void		send_joinmsg_toothers(t_chan *chan, t_client *client)
{
	t_listing	*other;
	t_client		*to;

	other = chan->users;
	while (other)
	{
		to = (t_client *)other->is;
		if (!(chan->cmode & CHFL_QUIET) || to->socket.fd == client->socket.fd)
			rpl_join(chan, to, client);
		other = other->next;
	}
}

t_listing	*sv_add_usertochan(t_client *client, t_chan *chan, t_server *server)
{
	t_listing	*new;

	(client->inf->umode & USR_INVISIBL) ? chan->invisibl++ : chan->nbusers++;
	if ((new = malloc(sizeof(*new))) == NULL)
	{
		sv_error(LOG_LEVEL_FATAL, "Out of memory", server);
		return (NULL);
	}
	new->prev = NULL;
	new->is = client;
	new->mode = 0;
	new->next = chan->users;
	if (new->next)
		new->next->prev = new;
	return (new);
}

t_listing	*sv_add_chantouser(t_chan *chan, t_client *client, t_server *server)
{
	t_listing	*new;

	if ((new = malloc(sizeof(*new))) == NULL)
	{
		sv_error(LOG_LEVEL_FATAL, "Out of memory", server);
		return (NULL);
	}
	new->prev = NULL;
	new->is = chan;
	new->mode = 0;
	new->next = client->chans;
	if (new->next)
		new->next->prev = new;
	return (new);
}
