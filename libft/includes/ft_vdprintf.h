/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vdprintf.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 13:42:43 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/12/11 13:43:28 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_VDPRINTF_H
# define FT_VDPRINTF_H

# include "ft_base_printf.h"

int			ft_vdprintf(int fd, const char *restrict format, va_list ap);
void		ft_vdprintf_write(t_dt *data);

#endif
