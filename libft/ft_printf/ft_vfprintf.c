/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vfprintf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 13:36:42 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/12/11 13:45:57 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include "ft_vfprintf.h"

int				ft_vfprintf(FILE *stream, const char *format, va_list ap)
{
	t_dt		data;
	int			ret;

	ft_memset(&data, 0, sizeof(data));
	data.stream = stream;
	data.tail = (char *)format;
	data.writeto = ft_vfprintf_write;
	va_copy(data.ap, ap);
	ret = pf_routine(&data);
	va_end(data.ap);
	return (ret);
}
