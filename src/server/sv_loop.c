/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_loop.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/13 08:45:52 by gbourgeo          #+#    #+#             */
/*   Updated: 2023/01/01 22:31:06 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sv_main.h"
#include <sys/select.h>

static int		sv_init_fds(t_server *server)
{
	t_client	*cl;
	int			max;

	cl = server->clients;
	max = (server->v4.fd > server->v6.fd) ? server->v4.fd : server->v6.fd;
	FD_ZERO(&server->fd_read);
	FD_ZERO(&server->fd_write);
	if (server->v4.fd > 0)
		FD_SET(server->v4.fd, &server->fd_read);
	if (server->v6.fd > 0)
		FD_SET(server->v6.fd, &server->fd_read);
	while (cl)
	{
		if (cl->type == FD_CLIENT)
		{
			if (cl->fct_write && cl->wr.len > 0)
				FD_SET(cl->socket.fd, &server->fd_write);
			else if (cl->fct_read && !cl->leaved)
				FD_SET(cl->socket.fd, &server->fd_read);
			if (cl->socket.fd > max)
				max = cl->socket.fd;
		}
		cl = cl->next;
	}
	return (max);
}

static void		sv_check_fds(t_server *server, int ret)
{
	t_client	*cl;

	cl = server->clients;
	if (server->v4.fd > 0 && FD_ISSET(server->v4.fd, &server->fd_read))
		sv_accept_v4(server);
	if (server->v6.fd > 0 && FD_ISSET(server->v6.fd, &server->fd_read))
		sv_accept_v6(server);
	while (cl && ret > 0)
	{
		if (cl->type != FD_FREE)
		{
			if (FD_ISSET(cl->socket.fd, &server->fd_write))
				cl->fct_write(cl);
			if (FD_ISSET(cl->socket.fd, &server->fd_read))
				cl->fct_read(server, cl);
			if (FD_ISSET(cl->socket.fd, &server->fd_read)
			|| FD_ISSET(cl->socket.fd, &server->fd_write))
				ret--;
		}
		cl = cl->next;
	}
}

static void		sv_check_chans(t_server *server)
{
	t_chan		*chan;
	t_chan		*next;

	chan = server->chans;
	while (chan)
	{
		next = chan->next;
		if (chan->nbusers + chan->invisibl <= 0)
		{
			if (chan->prev)
				chan->prev->next = chan->next;
			else
				server->chans = chan->next;
			if (chan->next)
				chan->next->prev = chan->prev;
			free(chan);
		}
		chan = next;
	}
}

int				sv_loop(t_server *server)
{
	struct timeval	timeout;
	int				ret;
	int				max;

	server->running = 1;
	sv_log(LOG_LEVEL_INFO, LOG_TYPE_SERVER, "server is running");
	while (server->running)
	{
		timeout.tv_sec = 0;
		timeout.tv_usec = 500000;
		sv_check_clients(server);
		sv_check_chans(server);
		max = sv_init_fds(server);
		ret = select(max + 1, &server->fd_read, &server->fd_write, NULL, &timeout);
		if (ret > 0)
			sv_check_fds(server, ret);
		else if (ret < 0 && errno != EINTR)
			sv_log(LOG_LEVEL_FATAL, LOG_TYPE_SYSTEM, "select(): %s", strerror(errno));
	}
	return (ret);
}
