/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_move_ringbuff.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 14:21:24 by gbourgeo          #+#    #+#             */
/*   Updated: 2023/06/10 14:06:00 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

void	ft_move_head(ssize_t len, t_buf *buf)
{
	if (len <= 0)
		return ;
	buf->len -= len;
	if (buf->head + len >= buf->end)
	{
		len = len - (buf->end - buf->head);
		buf->head = buf->start + len;
	}
	else
		buf->head += len;
}

void	ft_move_tail(ssize_t len, t_buf *buf)
{
	if (len <= 0)
		return ;
	buf->len += len;
	if (buf->tail + len >= buf->end)
	{
		len = len - (buf->end - buf->tail);
		buf->tail = buf->start + len;
	}
	else
		buf->tail += len;
}

void	ft_move_tail_forward(ssize_t len, t_buf *buf, char **ptr)
{
	if (len <= 0)
		return ;
	buf->tail += len;
	if (buf->tail >= buf->end)
		buf->tail = buf->start + (buf->tail - buf->end);
	if (ptr)
	{
		*ptr += len;
		if (*ptr >= buf->end)
			*ptr = buf->start + (*ptr - buf->end);
	}

}

void	ft_move_tail_backward(ssize_t len, t_buf *buf, char **ptr)
{
	if (len <= 0)
		return ;
	buf->tail -= len;
	if (buf->tail < buf->start)
		buf->tail = buf->end - (buf->start - buf->tail);
	if (ptr)
	{
		*ptr -= len;
		if (*ptr < buf->start)
			*ptr = buf->end - (buf->start - *ptr);
	}
}
