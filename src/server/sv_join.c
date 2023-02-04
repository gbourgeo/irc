/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_join.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/06/06 17:26:15 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/12/31 22:26:48 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sv_main.h"

static int	in_channel(char *name, t_client *client)
{
	t_listing	*chans;

	chans = client->chans;
	while (chans)
	{
		if (!sv_strcmp(((t_chan *)chans->is)->name, name))
			return (1);
		chans = chans->next;
	}
	return (0);
}

static void	sv_leaveall_channels(t_client *client, t_server *server)
{
	t_listing	*chans;
	char		*cmd[2];

	chans = client->chans;
	cmd[1] = NULL;
	while (chans)
	{
		cmd[0] = ((t_chan *)chans->is)->name;
		sv_leave(cmd, server, client);
		chans = chans->next;
	}
}

/*
** Si on implemente /NAMES, l'appeler au lieu de /WHO.
*/
static void	sv_check_channels(char **chans, char **keys, t_client *client, t_server *server)
{
	int		i;

	i = 0;
	while (chans[i])
	{
		if (ft_strcmp(chans[i], "0") == 0)
		{
			if (i == 0)
				sv_leaveall_channels(client, server);
			else
				sv_err(ERR_ILLEGALNAME, client, chans[i]);
		}
		if (!ISCHAN(*chans[i]) || !chans[i][1])
			sv_err(ERR_NOSUCHCHANNEL, client, chans[i]);
		else if (ft_strchr(chans[i], 7))
			sv_err(ERR_ILLEGALNAME, client, chans[i]);
		else if (client->chansnb >= CHAN_LIMIT)
			sv_err(ERR_TOOMANYCHANNELS, client);
		else if (!in_channel(chans[i], client))
			sv_join_chan(chans[i], &keys, client, server);
		i++;
	}
}

/**
 * @brief Server command to join a channel
 * 
 * @param cmds Channels(/key) to join, on the form: <chan>{,<chan>} [<key>{,<key}]
 * @param server Server
 * @param client Client
 */
void		sv_join(char **cmds, t_server *server, t_client *client)
{
	char	**chans;
	char	**keys;

	if (!cmds[0] || *cmds[0] == '\0')
		return (sv_err(ERR_NEEDMOREPARAMS, client, "JOIN"));
	if ((chans = ft_strsplit(cmds[0], ',')) == NULL)
		return (sv_error(LOG_LEVEL_FATAL, "Out of memory", server));
	if (cmds[1])
	{
		if ((keys = ft_strsplit(cmds[1], ',')) == NULL)
		{
			ft_free(&chans);
			return (sv_error(LOG_LEVEL_FATAL, "Out of memory", server));
		}
	}
	sv_check_channels(chans, keys, client, server);
	ft_free(&chans);
	ft_free(&keys);
}
