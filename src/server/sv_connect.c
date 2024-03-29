/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_connect.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/15 00:30:13 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/12/31 19:53:57 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sv_main.h"

/*
** CONNECT is not implemented in this version (1.0)
*/

void			sv_connect(char **cmds, t_server *e, t_client *cl)
{
	(void)e;
	if (!cmds[0] || !*cmds[0] || !cmds[1] || !*cmds[1])
		return (sv_err(ERR_NEEDMOREPARAMS, cl, "CONNECT"));
	if (cl->inf->umode & USR_OP || cl->inf->umode & USR_LOCALOP)
		return (sv_notice("You are already connected to a server.", cl, e));
	sv_err(ERR_NOPRIVILEGES, cl, "CONNECT");
}
