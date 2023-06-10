/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_commands_next.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/12 03:46:51 by gbourgeo          #+#    #+#             */
/*   Updated: 2023/06/04 11:52:19 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "err_list.h"
#include "cl_main.h"

void			cl_nosuchcommand(char **cmds, t_client *client)
{
	cl_log(CL_LOG_ERROR, cl_geterror(ERR_UNKNOWNCOMMAND, client, cmds[0]), client);
}

void			cl_pass(char **cmds, t_client *client)
{
	if (!cmds[1] || !*cmds[1])
		return (cl_log(CL_LOG_ERROR, cl_geterror(ERR_NEEDMOREPARAMS, client, cmds[0]), client));
	if (client->pass)
		free(client->pass);
	client->pass = ft_strdup(cmds[1]);
}

void			cl_user(char **cmds, t_client *client)
{
	if (ft_tablen(cmds) < 4)
		return (cl_log(CL_LOG_ERROR, cl_geterror(ERR_NEEDMOREPARAMS, client, cmds[0]), client));
	if (client->user)
		ft_free(&client->user);
	client->user = ft_tabdup(&cmds[1]);
}
