/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vsprintf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 13:51:55 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/12/11 13:52:36 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_vsprintf.h"

int			ft_vsprintf(char *str, const char *restrict format, va_list ap)
{
	t_dt	data;
	int		ret;

	ft_memset(&data, 0, sizeof(data));
	*str = '\0';
	data.str = str;
	data.tail = (char *)format;
	data.writeto = ft_vsprintf_write;
	va_copy(data.ap, ap);
	ret = pf_routine(&data);
	va_end(data.ap);
	return (ret);
}
