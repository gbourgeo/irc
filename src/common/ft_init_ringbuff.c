/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_ringbuff.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 00:20:04 by gbourgeo          #+#    #+#             */
/*   Updated: 2023/01/03 00:21:31 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

void		ft_init_ringbuf(t_buf *buffer, char *buff, int len)
{
	buffer->start = &buff[0];
	buffer->end = &buff[BUFF + 1];
	buffer->head = &buff[0];
	buffer->tail = &buff[0];
	buffer->len = len;
}
