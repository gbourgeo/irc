/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_ringbuff.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 00:20:04 by gbourgeo          #+#    #+#             */
/*   Updated: 2023/06/10 13:41:22 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

void		ft_init_ringbuf(t_buf *buffer)
{
	ft_memset(buffer->buffer, 0, sizeof(buffer->buffer));
	buffer->start = buffer->buffer + 0;
	buffer->end = buffer->buffer + BUFF;
	buffer->head = buffer->buffer + 0;
	buffer->tail = buffer->buffer + 0;
	buffer->len = 0;
}
