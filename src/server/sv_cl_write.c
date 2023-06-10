/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_cl_write.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/27 18:43:00 by gbourgeo          #+#    #+#             */
/*   Updated: 2023/06/03 18:25:34 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sv_main.h"

static void		check_queue(t_client *cl)
{
	char		*ptr;

	if (!(ptr = cl->queue))
		return ;
	while (*ptr)
	{
		*cl->wr.tail = *ptr;
		cl->wr.tail++;
		if (cl->wr.tail == cl->wr.end)
			cl->wr.tail = cl->wr.start;
		cl->wr.len++;
		ptr++;
		if (cl->wr.len == BUFF)
		{
			ptr = ft_strdup(ptr);
			free(cl->queue);
			cl->queue = ptr;
			break ;
		}
	}
	if (!*ptr)
	{
		free(cl->queue);
		cl->queue = NULL;
	}
}

/**
 * @brief Fonction d'envoi au client
 * 
 * @param client Structure du client
 */
void			sv_cl_send(t_client *client)
{
	ssize_t		ret;

	if (client->wr.len <= 0)
		check_queue(client);
	if (client->wr.len <= 0)
		return ;
	if (client->wr.tail <= client->wr.head)
		ret = send(client->socket.fd, client->wr.head, client->wr.end - client->wr.head, MSG_DONTWAIT | MSG_NOSIGNAL);
	else
		ret = send(client->socket.fd, client->wr.head, client->wr.tail - client->wr.head, MSG_DONTWAIT | MSG_NOSIGNAL);
	char buf[BUFF + 1];
	if (client->wr.tail <= client->wr.head)
	{
		ft_strncpy(buf, client->wr.head, client->wr.end - client->wr.head);
		buf[client->wr.end - client->wr.head] = 0;
	}
	else
	{
		ft_strncpy(buf, client->wr.head, client->wr.tail - client->wr.head);
		buf[client->wr.tail - client->wr.head] = 0;
	}
	sv_log(LOG_LEVEL_DEBUG, LOG_TYPE_CLIENT, "[%s] Send: %s", client->uid, buf);
	ft_move_head(ret, &client->wr);
	// Cas d'erreur ??
}

/**
 * @brief Fonction d'écriture dans le ringbuffer du client
 * 
 * @param str Chaîne de caractères à copier
 * @param client Structure du client
 */
void			sv_cl_write(char *str, t_client *client)
{
	char		*ptr;

	while (str && *str)
	{
		if (client->wr.len == BUFF)
		{
			ptr = client->queue;
			client->queue = ft_strjoin(ptr, str);
			free(ptr);
			break ;
		}
		*client->wr.tail = *str;
		ft_move_tail(1, &client->wr);
		str++;
	}
}
