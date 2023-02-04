/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_pass.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/16 10:02:38 by gbourgeo          #+#    #+#             */
/*   Updated: 2023/01/01 00:46:17 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sv_main.h"

/**
 * @brief set a ‘connection password’.
 * If set, the password must be set before any attempt to register the
 * connection is made. This requires that clients send a PASS command before
 * sending the NICK / USER combination.
 * 
 * @param cmds <password>
 * @param server Server
 * @param client Client
 */
void		sv_pass(char **cmds, t_server *server, t_client *client)
{
	(void)server;
	if (!cmds[0] || !*cmds[0])
		return (sv_err(ERR_NEEDMOREPARAMS, client, "PASS"));
	if (client->inf->registered > 0)
		return (sv_err(ERR_ALREADYREGISTERED, client));
	if (client->inf->pass)
		free(client->inf->pass);
	client->inf->pass = ft_strdup(*cmds);
}
