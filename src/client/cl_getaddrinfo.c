/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_getaddrinfo.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/13 08:51:24 by gbourgeo          #+#    #+#             */
/*   Updated: 2023/01/01 22:57:59 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "ft_printf.h"
#include "cl_main.h"

static int	cl_init_addrinfo(char *addr, char *port, struct addrinfo **res)
{
	struct addrinfo	hints;

	ft_memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;
	ft_printf("* \e[33mConnecting to \e[0m%s\e[33m on port \e[0m%s\e[33m... \e[0m",
		addr, port);
	if (getaddrinfo(addr, port, &hints, res))
	{
		ft_printf("\e[31mFailed\e[0m :%s\n", strerror(errno));
		return (1);
	}
	return (0);
}

int			cl_getaddrinfo(char *addr, char *port, t_client *client)
{
	struct addrinfo	*results;
	struct addrinfo	*tmp;

	if (cl_init_addrinfo(addr, port, &results))
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
		ft_putendl("\e[31mServer not found\e[0m");
	else
		ft_putendl("\e[32mOK\e[0m");
	return (tmp == NULL);
}
