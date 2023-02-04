/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vsnprintf.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 13:48:55 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/12/11 13:51:51 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_vsnprintf.h"

int			ft_vsnprintf(char *str, size_t size,
						const char *restrict format, va_list ap)
{
	t_dt	data;
	int		ret;

	ft_memset(&data, 0, sizeof(data));
	*str = '\0';
	data.str = str;
	data.str_size = size;
	data.tail = (char *)format;
	data.writeto = ft_vsnprintf_write;
	va_copy(data.ap, ap);
	ret = pf_routine(&data);
	va_end(data.ap);
	return (ret);
}
