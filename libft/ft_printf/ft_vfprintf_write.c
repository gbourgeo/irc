/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vfprintf_write.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 13:41:11 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/12/11 13:41:24 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_vfprintf.h"

void		ft_vfprintf_write(t_dt *data)
{
	void	*buf;

	buf = data->buff;
	data->ret += fwrite(buf, sizeof(*data->buff), data->pos, data->stream);
}
