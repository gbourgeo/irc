/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_new_client.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/26 17:26:04 by gbourgeo          #+#    #+#             */
/*   Updated: 2023/01/03 00:21:07 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sv_main.h"
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

/**
 * @brief Set the client an unique identifier (uid).
 * The uid is on the form:
 * 	- 4 digits from '0' to '9'
 * 	- 5 uppercase letters from 'A' to 'Z'
 * 
 * @param client The client to set the uid
 */
static void		set_client_uid(t_client *client)
{
	static char	userid[UID_LEN + 1] = { 0 };
	int			i;

	if (userid[0] == '\0')
	{
		ft_strncpy(userid, "1234AAAAA", UID_LEN + 1);
		userid[UID_LEN] = '\0';
	}
	i = UID_LEN;
	ft_strcpy(client->uid, userid);
	while (i > 3 && userid[i] >= 'Z')
	{
		userid[i] = 'A';
		i--;
	}
	if (i < 4)
	{
		while (i > 0 && userid[i] >= '9')
		{
			userid[i] = '0';
			i--;
		}
	}
	if (i >= 0)
		userid[i] += 1;
}

int				sv_new_client(t_socket *socket, t_client **clients)
{
	t_client	*cl;

	if ((cl = (t_client *)malloc(sizeof(*cl))) == NULL)
		return (sv_log(LOG_LEVEL_ERROR, LOG_TYPE_SYSTEM, "Malloc failed"));
	ft_memset(cl, 0, sizeof(*cl));
	ft_memcpy(&cl->socket, socket, sizeof(*socket));
	set_client_uid(cl);
	if ((cl->inf = (t_file *)malloc(sizeof(*cl->inf))) == NULL)
		return (sv_log(LOG_LEVEL_ERROR, LOG_TYPE_SYSTEM, "Malloc failed"));
	ft_memset(cl->inf, 0, sizeof(*cl->inf));
	cl->type = FD_CLIENT;
	cl->fct_read = sv_cl_read;
	cl->fct_write = sv_cl_send;
	ft_init_ringbuf(&cl->rd, cl->buf_read, BUFF);
	ft_init_ringbuf(&cl->wr, cl->buf_write, 0);
	if (*clients != NULL)
		(*clients)->prev = cl;
	cl->next = *clients;
	*clients = cl;
	sv_log(LOG_LEVEL_INFO, LOG_TYPE_SERVER, "New client from %s(%s):%s",
		cl->socket.addr, cl->socket.host, cl->socket.port);
	return (0);
}
