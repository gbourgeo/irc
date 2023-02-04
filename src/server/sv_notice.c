/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_notice.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/13 03:16:10 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/12/31 13:04:10 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sv_main.h"

void			sv_notice(char *str, t_client *client, t_server *server)
{
	sv_cl_write(":", client);
	sv_cl_write(server->name, client);
	sv_cl_write(" NOTICE * :*** ", client);
	sv_cl_write(str, client);
	sv_cl_write(END_CHECK, client);
}
