/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_getaddrinfo.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/13 08:51:24 by gbourgeo          #+#    #+#             */
/*   Updated: 2023/06/10 14:27:35 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __USE_XOPEN2K
# define __USE_XOPEN2K 1
#endif
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "cl_main.h"

static int	cl_init_addrinfo(char **argv, struct addrinfo **res, t_client *client)
{
	struct addrinfo	hints;
	char			*addr;
	char			*port;

	ft_memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;
	addr = argv[1];
	if ((port = ft_strrchr(argv[1], ':')) != NULL)
		*port++ = '\0';
	else if (argv[2])
		port = argv[2];
	else
		port = DEF_PORT;
	wprintw(client->windows.chatwin,
		"*** Connecting to %s on port %s ... ", addr, port);
	wrefresh(client->windows.chatwin);
	if (getaddrinfo(addr, port, &hints, res))
	{
		wprintw(client->windows.chatwin,
			"failed : %s\n", strerror(errno));
		wrefresh(client->windows.chatwin);
		return (1);
	}
	return (0);
}

int			cl_getaddrinfo(char **argv, t_client *client)
{
	struct addrinfo	*results;
	struct addrinfo	*tmp;

	if (cl_init_addrinfo(argv, &results, client))
		return (1);
	tmp = results;
	while (tmp != NULL)
	{
		client->sock = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);
		if (client->sock == -1)
			continue ;
		if (connect(client->sock, tmp->ai_addr, tmp->ai_addrlen) == 0)
			break ;
		close(client->sock);
		client->sock = -1;
		tmp = tmp->ai_next;
	}
	freeaddrinfo(results);
	if (tmp == NULL)
		wprintw(client->windows.chatwin, "Server not found\n");
	else
		wprintw(client->windows.chatwin, "OK\n");
	wrefresh(client->windows.chatwin);
	return (tmp == NULL);
}
