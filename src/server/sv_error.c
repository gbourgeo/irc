/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/16 07:34:29 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/12/31 21:17:14 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <sys/socket.h>
// #include <stdio.h>
// #include <string.h>
// #include <errno.h>
#include "sv_main.h"

// static void	sv_kill_channels(t_chan *chan, t_server *e)
// {
// 	t_listing	*next;

// 	if (chan == NULL)
// 		return ;
// 	if (chan->next)
// 		sv_kill_channels(chan->next, e);
// 	if (e->verbose)
// 		printf("\n\e[31mCHAN\e[0m %s \e[31mdeleted\e[0m\n", chan->name);
// 	ft_bzero(chan->name, CHANNAME_LEN);
// 	ft_bzero(chan->topic, TOPIC_LEN);
// 	while (chan->users)
// 	{
// 		next = chan->users->next;
// 		free(chan->users);
// 		chan->users = next;
// 	}
// 	free(chan);
// 	chan = NULL;
// }

// static void	sv_kill_connections(t_client *client, t_server *e)
// {
// 	t_listing	*next;

// 	if (client == NULL)
// 		return ;
// 	if (client->next)
// 		sv_kill_connections(client->next, e);
// 	close(client->socket.fd);
// 	if (e->verbose)
// 		printf("CLIENT %s:%s killed\n", client->socket.addr, client->socket.port);
// 	if (!client->inf->pass)
// 	{
// 		ft_free(&client->inf->realname);
// 		free(client->inf);
// 	}
// 	if (client->away)
// 		free(client->away);
// 	while (client->chans)
// 	{
// 		next = client->chans->next;
// 		free(client->chans);
// 		client->chans = next;
// 	}
// 	ft_bzero(client, sizeof(*client));
// 	free(client);
// 	client = NULL;
// }

void		sv_error(e_log_level level, char *str, t_server *server)
{
	sv_log(level, LOG_TYPE_SYSTEM, str, strerror(errno));
	server->running = 0;
	// sv_kill_connections(server->clients, server);
	// sv_kill_channels(server->chans, server);
	// close(server->v4.fd);
	// close(server->v6.fd);
	// ft_memset(server, 0, sizeof(*server));
	// exit(EXIT_FAILURE);
}
