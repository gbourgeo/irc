/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vdprintf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 13:41:53 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/12/11 13:45:31 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_vdprintf.h"

int			ft_vdprintf(int fd, const char *restrict format, va_list ap)
{
	t_dt	data;
	int		ret;

	ft_memset(&data, 0, sizeof(data));
	data.fd = fd;
	data.tail = (char *)format;
	data.writeto = ft_vdprintf_write;
	va_copy(data.ap, ap);
	ret = pf_routine(&data);
	va_end(data.ap);
	return (ret);
}
