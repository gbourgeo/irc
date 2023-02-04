/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vsnprintf.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 13:50:52 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/12/11 13:51:24 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_VSNPRINTF_H
# define FT_VSNPRINTF_H

# include "ft_base_printf.h"

int			ft_vsnprintf(char *str, size_t size,
						const char *restrict format, va_list ap);
void		ft_vsnprintf_write(t_dt *data);

#endif
