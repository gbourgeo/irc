/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vsprintf.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 13:52:04 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/12/11 13:53:46 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_VSPRINTF_H
# define FT_VSPRINTF_H

# include "ft_base_printf.h"

int			ft_vsprintf(char *str, const char *restrict format, va_list ap);
void		ft_vsprintf_write(t_dt *data);

#endif
