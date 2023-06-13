/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_accept.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/21 17:16:50 by gbourgeo          #+#    #+#             */
/*   Updated: 2023/06/12 21:30:45 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "sv_main.h"

typedef struct sockaddr_in	sockaddr_in_t;
typedef struct sockaddr_in6	sockaddr_in6_t;

#define V4_ADDR(csin) &((sockaddr_in_t *)&csin)->sin_addr.s_addr
#define V6_ADDR(csin) &((sockaddr_in6_t *)&csin)->sin6_addr.s6_addr

static int		sv_check_clone(t_client *clients, struct sockaddr *csin)
{
	size_t		clones;
	size_t		addr_len;

	clones = 0;
	addr_len = (csin->sa_family == AF_INET) ?
		sizeof(((sockaddr_in_t *)csin)->sin_addr.s_addr) :
		sizeof(((sockaddr_in6_t *)csin)->sin6_addr.s6_addr);
	while (clients)
	{
		if (clients->socket.csin.sa_family == csin->sa_family)
		{
			if (csin->sa_family == AF_INET)
				clones += ft_memcmp(V4_ADDR(clients->socket.csin),
									V4_ADDR(*csin), addr_len) == 0;
			else if (csin->sa_family == AF_INET6)
				clones += ft_memcmp(V6_ADDR(clients->socket.csin),
									V6_ADDR(*csin), addr_len) == 0;
		}
		clients = clients->next;
	}
	return (clones);
}

static void		sv_send_info(int fd, char *server_name, char *str)
{
	char		buf[248];

	ft_strcpy(buf, ":");
	ft_strcat(buf, server_name);
	ft_strcat(buf, " NOTICE * :*** ");
	ft_strcat(buf, str);
	ft_strcat(buf, END_CHECK);
	send(fd, buf, ft_strlen(buf), MSG_DONTWAIT | MSG_NOSIGNAL);
	sv_log(LOG_LEVEL_INFO, LOG_TYPE_SERVER, "%s", str);
}

static int		sv_accept_failed(char *str, t_socket *socket)
{
	char		buf[248];

	ft_strcpy(buf, "Closing Link: ");
	ft_strcat(buf, socket->addr);
	ft_strcat(buf, "@");
	ft_strcat(buf, socket->host);
	ft_strcat(buf, ":");
	ft_strcat(buf, socket->port);
	ft_strcat(buf, " ");
	ft_strcat(buf, str);
	sv_log(LOG_LEVEL_INFO, LOG_TYPE_SERVER, "%s", str);
	ft_strcat(buf, END_CHECK);
	send(socket->fd, buf, ft_strlen(buf), MSG_DONTWAIT | MSG_NOSIGNAL);
	close(socket->fd);
	return (0);
}

int				sv_accept_v4(t_server *server)
{
	t_socket	socket;
	socklen_t	len;

	sv_log(LOG_LEVEL_INFO, LOG_TYPE_SERVER, "Accepting new connection");
	len = sizeof(socket.csin);
	if ((socket.fd = accept(server->v4.fd, &socket.csin, &len)) < 0)
		return (sv_log(LOG_LEVEL_ERROR, LOG_TYPE_SYSTEM,
			"Accept failed: %s", strerror(errno)));
	sv_send_info(socket.fd, server->name, "Looking up your ident...");
	if (getsockname(socket.fd, &socket.csin, &len))
		sv_send_info(socket.fd, server->name, "Couldn't retreive your ident");
	sv_send_info(socket.fd, server->name, "Looking up your hostname...");
	if (getnameinfo(&socket.csin, sizeof(socket.csin), socket.host, NI_MAXHOST, socket.port, NI_MAXSERV, NI_NUMERICSERV))
		sv_send_info(socket.fd, server->name, "Couldn't look up your hostname");
	inet_ntop(AF_INET, V4_ADDR(socket.csin), socket.addr, sizeof(socket.addr));
	if (server->members + 1 >= MAX_CLIENT)
		return (sv_accept_failed("Maximum clients reached.", &socket));
	if (sv_check_clone(server->clients, &socket.csin) >= MAX_CLIENT_BY_IP)
		return (sv_accept_failed("Max Clients per IP reached.", &socket));
	if (sv_allowed(&socket, server->conf.allowed_user)
	|| sv_allowed(&socket, server->conf.pass_user))
		return (sv_new_client(&socket, &server->clients));
	return (sv_accept_failed("Not allowed to login to the server.", &socket));
}

int				sv_accept_v6(t_server *server)
{
	t_socket	socket;
	socklen_t	len;

	sv_log(LOG_LEVEL_INFO, LOG_TYPE_SERVER, "Accepting new connection");
	len = sizeof(socket.csin);
	if ((socket.fd = accept(server->v6.fd, &socket.csin, &len)) < 0)
		return (sv_log(LOG_LEVEL_ERROR, LOG_TYPE_SYSTEM,
			"Accept failed: %s", strerror(errno)));
	sv_send_info(socket.fd, server->name, "Looking up your ident...");
	if (getsockname(socket.fd, &socket.csin, &len))
		sv_send_info(socket.fd, server->name, "Couldn't retreive your ident");
	sv_send_info(socket.fd, server->name, "Looking up your hostname...");
	if (getnameinfo(&socket.csin, sizeof(socket.csin), socket.host, NI_MAXHOST,
					socket.port, NI_MAXSERV, NI_NUMERICSERV))
		sv_send_info(socket.fd, server->name, "Couldn't look up your hostname");
	inet_ntop(AF_INET6, V6_ADDR(socket.csin), socket.addr, sizeof(socket.addr));
	if (server->members + 1 >= MAX_CLIENT)
		return (sv_accept_failed("Maximum clients reached.", &socket));
	if (sv_check_clone(server->clients, &socket.csin) >= MAX_CLIENT_BY_IP)
		return (sv_accept_failed("Max Clients per IP reached.", &socket));
	if (sv_allowed(&socket, server->conf.allowed_user)
	|| sv_allowed(&socket, server->conf.pass_user))
		return (sv_new_client(&socket, &server->clients));
	return (sv_accept_failed("Not allowed to login to the server.", &socket));
}
