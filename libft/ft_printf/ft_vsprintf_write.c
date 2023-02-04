/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vsprintf_write.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 13:51:59 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/12/11 13:53:14 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_vsprintf.h"

void		ft_vsprintf_write(t_dt *data)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (data->str[i])
		i++;
	while (j < data->pos)
	{
		data->str[i + j] = data->buff[j];
		j++;
	}
	data->str[i + j] = '\0';
	data->ret += j;
}
