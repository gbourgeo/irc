/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_oper.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/08 03:01:56 by gbourgeo          #+#    #+#             */
/*   Updated: 2023/01/01 02:12:16 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_snprintf.h"
#include "sv_main.h"

static void		new_op(t_user *op, t_client *client)
{
	char	msg[BUFF];

	client->inf->umode |= (op->mode == 'O') ? USR_OP : USR_LOCALOP;
	ft_snprintf(msg, sizeof(msg), ":%s!~%s@%s MODE %s :+%c" END_CHECK,
		client->inf->nick, client->inf->username,
		(*client->socket.host) ? client->socket.host : client->socket.addr,
		client->inf->nick, (op->mode == 'O') ? 'O' : 'o');
	sv_cl_write(msg, client);
}

void			sv_oper(char **cmds, t_server *server, t_client *client)
{
	t_user		*oper;

	if (!cmds[0] || !cmds[1])
		return (sv_err(ERR_NEEDMOREPARAMS, client, "OPER"));
	oper = server->conf.opers;
	while (oper)
	{
		if (!ft_strcmp(oper->hostname, client->socket.host) &&
			!ft_strcmp(oper->nick, cmds[0]))
		{
			if (!ft_strcmp(oper->passwd, cmds[1]))
				return (new_op(oper, client));
			return (sv_err(ERR_PASSWDMISMATCH, NULL, NULL, client));
		}
		oper = oper->next;
	}
	sv_err(ERR_NOOPERHOST, NULL, NULL, client);
}
