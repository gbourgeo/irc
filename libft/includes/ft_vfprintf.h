/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vfprintf.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 13:38:14 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/12/11 13:40:30 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_VFPRINTF_H
# define FT_VFPRINTF_H

# include <stdio.h>
# include "ft_base_printf.h"

int			ft_vfprintf(FILE *stream, const char *format, va_list ap);
void		ft_vfprintf_write(t_dt *data);

#endif
