/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_err.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/11 04:01:19 by gbourgeo          #+#    #+#             */
/*   Updated: 2023/06/10 15:20:26 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sv_main.h"
#include "ft_snprintf.h"
#include "ft_vsnprintf.h"
#include <sys/socket.h>

extern t_server		g_server;
extern err_list_t	g_err_list[];
extern size_t		g_err_list_size;

/**
 * @brief Sends an error notification to the client
 * 
 * @param errnum Error number
 * @param client Client to send
 * @param ... List of parameters to pass to the error string (see err_list.h)
 */
void			sv_err(errnum_list_e errnum, t_client *client, ...)
{
	err_list_t	*err;
	char		errstr[1024];
	va_list		ap;
	size_t		pos;

	pos = errnum - 400;
	if (pos > 0 && pos < g_err_list_size)
	{
		err = &g_err_list[pos];
		if (err->errnum != ERR_NOT_DEFINED)
		{
			ft_snprintf(errstr,
				sizeof(errstr),
				":%s %d %s ",
				g_server.name,
				errnum,
				client->inf->nick);
			pos = ft_strlen(errstr);
			va_start(ap, client);
			ft_vsnprintf(errstr + pos, sizeof(errstr) - pos, err->errstr, ap);
			va_end(ap);
			ft_strncat(errstr, "\r\n", sizeof(errstr) - 3);
			sv_cl_write(errstr, client);
		}
	}
}
