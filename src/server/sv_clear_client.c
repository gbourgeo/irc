/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_clear_client.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/16 21:57:29 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/12/31 13:59:45 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include "sv_main.h"

static void		sv_free_client_onchans(t_client *client, t_listing *chan)
{
	t_listing	*list;

	if (!chan)
		return ;
	list = ((t_chan *)chan->is)->users;
	while (list)
	{
		if (((t_client *)list->is)->socket.fd == client->socket.fd)
		{
			(client->inf->umode & USR_INVISIBL) ? ((t_chan *)chan->is)->invisibl-- :
				((t_chan *)chan->is)->nbusers--;
			if (list->prev)
				list->prev->next = list->next;
			else
				((t_chan *)client->chans->is)->users = list->next;
			if (list->next)
				list->next->prev = list->prev;
			free(list);
			break ;
		}
		list = list->next;
	}
	sv_free_client_onchans(client, chan->next);
	ft_memset(chan, 0, sizeof(*chan));
	free(chan);
}

static void		sv_free_client(t_client *client, t_server *server)
{
	if (!client->inf->pass)
	{
		ft_free(&client->inf->realname);
		free(client->inf);
		client->inf = NULL;
	}
	if (client->away)
		free(client->away);
	if (client->queue)
		free(client->queue);
	if (client->prev)
		client->prev->next = client->next;
	else
		server->clients = client->next;
	if (client->next)
		client->next->prev = client->prev;
}

static void		sv_send_reason(t_client *client)
{
	sv_cl_write("ERROR :Closing Link: ", client);
	sv_cl_write(client->inf->nick, client);
	sv_cl_write("[~", client);
	sv_cl_write(client->inf->username, client);
	sv_cl_write("@", client);
	sv_cl_write((*client->socket.host) ? client->socket.host : client->socket.addr, client);
	sv_cl_write("] (", client);
	sv_cl_write(client->reason, client);
	if (client->leaved == 2)
	{
		sv_cl_write("[~", client);
		sv_cl_write(client->inf->username, client);
		sv_cl_write("]", client);
	}
	sv_cl_write(")", client);
	sv_cl_write(END_CHECK, client);
	client->reason = NULL;
}

t_client			*sv_clear_client(t_client *client, t_server *server)
{
	t_client		*next;

	next = client->next;
	if (client->reason)
	{
		// if (server->verbose)
		// 	printf("%s(%s):%s :%s\n", client->socket.addr, client->socket.host,
		// 			client->socket.port, client->reason);
		sv_log(LOG_LEVEL_INFO, LOG_TYPE_SERVER, "%s(%s):%s :%s",
			client->socket.addr, client->socket.host,
			client->socket.port, client->reason);
		sv_send_reason(client);
		return (client);
	}
	if (client->inf->registered > 0)
		server->members--;
	sv_free_client_onchans(client, client->chans);
	sv_free_client(client, server);
	close(client->socket.fd);
	ft_memset(client, 0, sizeof(*client));
	free(client);
	client = NULL;
	return (next);
}
