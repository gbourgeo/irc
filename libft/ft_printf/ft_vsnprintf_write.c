/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vsnprintf_write.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 13:50:09 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/12/11 13:50:34 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_base_printf.h"

void		ft_vsnprintf_write(t_dt *data)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (data->str_size <= 1)
		return ;
	while (data->str[i])
		i++;
	while (j < data->pos && data->str_size > 1)
	{
		data->str[i + j] = data->buff[j];
		j++;
		data->str_size--;
	}
	data->str[i + j] = '\0';
	data->ret += j;
}
