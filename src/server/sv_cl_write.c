/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_cl_write.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/27 18:43:00 by gbourgeo          #+#    #+#             */
/*   Updated: 2023/06/13 13:08:23 by gbourgeo         ###   ########.fr       */
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
	ssize_t	ret;
	int		send_b;

	if (client->wr.len <= 0)
		check_queue(client);
	if (client->wr.len <= 0)
		return ;
	send_b = (client->wr.tail <= client->wr.head) ?
		client->wr.end - client->wr.head :
		client->wr.tail - client->wr.head;
	sv_log(LOG_LEVEL_DEBUG, LOG_TYPE_CLIENT, "[%s] Sending %ldo", client->uid, send_b);
	ret = send(client->socket.fd, client->wr.head, send_b, MSG_DONTWAIT | MSG_NOSIGNAL);
	sv_log(LOG_LEVEL_DEBUG, LOG_TYPE_CLIENT, "[%s] Sent %ldo\n%.*s", client->uid, ret, ret, client->wr.head);
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
