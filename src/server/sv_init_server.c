/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_init_server.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/13 08:49:52 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/10/20 21:46:56 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <pwd.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdio.h>
#include "sv_main.h"

static int	sv_init_server_fail(int log_level, char *error, char *arg, int fd)
{
	sv_log(log_level, LOG_TYPE_SYSTEM, error, arg);
	if (fd > 0)
		close(fd);
	return (-1);
}

static int	sv_findsocket_v4(struct addrinfo *p, t_socket *v4)
{
	int					fd;
	int					on;
	socklen_t			len;

	on = 1;
	len = sizeof(v4->csin);
	fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
	if (fd < 0)
		return (sv_init_server_fail(LOG_LEVEL_ERROR,
			"Opening socket: %s", strerror(errno), fd));
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)))
		return (sv_init_server_fail(LOG_LEVEL_ERROR,
			"Socket option: %s", strerror(errno), fd));
	if (bind(fd, p->ai_addr, p->ai_addrlen) == -1)
		return (sv_init_server_fail(LOG_LEVEL_ERROR,
			"Bind socket: %s", strerror(errno), fd));
	if (getsockname(fd, &v4->csin, &len))
		sv_init_server_fail(LOG_LEVEL_INFO,
			"Socket name: %s", strerror(errno), fd);
	if (p->ai_canonname)
		ft_strncpy(v4->host, p->ai_canonname, NI_MAXHOST);
	inet_ntop(AF_INET, p->ai_addr, v4->addr, sizeof(v4->addr));
	return (fd);
}

static int	sv_findsocket_v6(struct addrinfo *p, t_socket *v6)
{
	int					fd;
	int					on;
	socklen_t			len;

	on = 1;
	len = sizeof(v6->csin);
	fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
	if (fd < 0)
		return (sv_init_server_fail(LOG_LEVEL_ERROR,
			"Opening socket: %s", strerror(errno), fd));
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)))
		return (sv_init_server_fail(LOG_LEVEL_ERROR,
			"Socket option: %s", strerror(errno), fd));
	if (setsockopt(fd, IPPROTO_IPV6, IPV6_V6ONLY, &on, sizeof(on)))
		return (sv_init_server_fail(LOG_LEVEL_ERROR,
			"Socket option: %s", strerror(errno), fd));
	if (bind(fd, p->ai_addr, p->ai_addrlen) == -1)
		return (sv_init_server_fail(LOG_LEVEL_ERROR,
			"Bind socket: %s", strerror(errno), fd));
	if (getsockname(fd, &v6->csin, &len))
		sv_init_server_fail(LOG_LEVEL_INFO,
			"Socket name: %s", strerror(errno), fd);
	if (p->ai_canonname != NULL)
		ft_strncpy(v6->host, p->ai_canonname, NI_MAXHOST);
	inet_ntop(AF_INET6, p->ai_addr, v6->addr, sizeof(v6->addr));
	return (fd);
}

static int	sv_open_sockets(t_socket *v4, t_socket *v6, char *addr, char *port)
{
	struct addrinfo		hints;
	struct addrinfo		*res;
	struct addrinfo		*p;

	ft_memset(&hints, 0, sizeof(hints));
	hints.ai_flags = AI_PASSIVE | AI_CANONNAME;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	if (getaddrinfo(addr, port, &hints, &res))
		return (sv_init_server_fail(LOG_LEVEL_FATAL,
			"getaddrinfo: %s", strerror(errno), -1));
	p = res;
	while (p != NULL)
	{
		if (v4->fd < 0 && p->ai_family == AF_INET)
			v4->fd = sv_findsocket_v4(p, v4);
		if (v6->fd < 0 && p->ai_family == AF_INET6)
			v6->fd = sv_findsocket_v6(p, v6);
		if (v4->fd != -1 && v6->fd != -1)
			break ;
		p = p->ai_next;
	}
	freeaddrinfo(res);
	if (p == NULL && v4->fd < 0 && v6->fd < 0)
		return (sv_init_server_fail(LOG_LEVEL_FATAL,
			"Port %s invalid or unavailable", port, -1));
	return (0);
}

int			sv_init_server(t_socket *v4, t_socket *v6, char *addr, char *port)
{
	int		nb_clients;

	v4->fd = -1;
	v6->fd = -1;
	if (sv_open_sockets(v4, v6, addr, port) != 0)
		return (-1);
	if (v4->fd > 0)
	{
		nb_clients = (v6->fd >= 0) ? MAX_CLIENT / 2 : MAX_CLIENT;
		if (listen(v4->fd, nb_clients) == -1)
			return (sv_init_server_fail(LOG_LEVEL_FATAL,
				"Listening port: %s", strerror(errno), v4->fd));
		sv_log(LOG_LEVEL_INFO, LOG_TYPE_SERVER,
			"Listening for %d clients from %s (%s) on port %s",
			nb_clients, v4->host, v4->addr, port);
	}
	if (v6->fd > 0)
	{
		nb_clients = (v6->fd >= 0) ? MAX_CLIENT / 2 : MAX_CLIENT;
		if (listen(v6->fd, (v4->fd >= 0) ? MAX_CLIENT / 2 : MAX_CLIENT) == -1)
			return (sv_init_server_fail(LOG_LEVEL_FATAL,
				"Listening port: %s", strerror(errno), v6->fd));
		sv_log(LOG_LEVEL_INFO, LOG_TYPE_SERVER,
			"Listening for %d clients from %s (%s) on port %s",
			nb_clients, v6->host, v6->addr, port);
	}
	return (0);
}
