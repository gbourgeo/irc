/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vdprintf_write.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 13:43:56 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/12/11 13:44:24 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "ft_base_printf.h"

void	ft_vdprintf_write(t_dt *data)
{
	data->ret += write(data->fd, data->buff, data->pos);
}
