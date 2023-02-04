/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_leave.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/06/06 17:37:00 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/12/31 22:44:14 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include "sv_main.h"

static void	check_normal_channel(
	char *chan_name,
	char **leave_reason,
	t_client *client,
	t_chan	*chan)
{
	while (chan && sv_strcmp(chan_name, chan->name) != 0)
		chan = chan->next;
	if (chan == NULL)
		sv_err(ERR_NOSUCHCHANNEL, client, chan_name);
	else
		sv_find_userinchan(leave_reason, chan, client);
}

static void	check_safe_channel(
	char *chan_name,
	char **leave_reason,
	t_client *client,
	t_chan	*chan)
{
	t_chan	*found;
	int		len;

	found = NULL;
	while (chan)
	{
		len = ft_strlen(chan->name);
		if (sv_strncmp(chan->name, chan_name, len - 5) == chan->name[len - 5])
		{
			if (found)
				break ;
			found = chan;
		}
		chan = chan->next;
	}
	if (found && chan)
		sv_err(ERR_TOOMANYTARGETS, client, chan_name);
	else if (chan == NULL && found == NULL)
		sv_err(ERR_NOSUCHCHANNEL, client, chan_name);
	else
		sv_find_userinchan(leave_reason, found, client);
}

/**
 * @brief Leave channel(s) with a reason message (optionnal)
 * 
 * @param cmds List of channels to leave, plus reason message to send
 * @param server Server
 * @param client Client
 */
void		sv_leave(char **cmds, t_server *server, t_client *client)
{
	char	**list;
	int		i;

	i = 0;
	if (!cmds[0] || *cmds[0] == '\0')
		return (sv_err(ERR_NEEDMOREPARAMS, client, "LEAVE"));
	if ((list = ft_strsplit(cmds[0], ',')) == NULL)
		return (sv_error(LOG_LEVEL_FATAL, "Out of memory", server));
	while (list[i])
	{
		if (*list[i] == '!')
			check_safe_channel(list[i], cmds + 1, client, server->chans);
		else
			check_normal_channel(list[i], cmds + 1, client, server->chans);
		i++;
	}
	ft_free(&list);
}
