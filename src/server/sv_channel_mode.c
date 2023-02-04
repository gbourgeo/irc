/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_channel_mode.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/19 04:20:45 by gbourgeo          #+#    #+#             */
/*   Updated: 2023/01/01 00:00:19 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sv_main.h"

static void			sv_send_channel(t_grp *group, int limit)
{
	t_listing		*list;

	list = group->on->users;
	while (list)
	{
		group->to = (t_client *)list->is;
		rpl_cmode(group, (limit) ? ft_itoa(group->on->limit) : NULL);
		list = list->next;
	}
}

static void			sv_change_more(t_grp *group, char ***cmd)
{
	if (group->c && *(*cmd + 1) == NULL)
		return ;
	if (!group->c)
		group->on->cmode &= (*group->ptr == 'l') ? ~(CHFL_LIMIT) : ~(CHFL_KEY);
	else
	{
		group->on->cmode |= (*group->ptr == 'l') ? CHFL_LIMIT : CHFL_KEY;
		(*cmd)++;
		if (*group->ptr == 'l')
		{
			if ((group->on->limit = ft_atoi(**cmd)) > 0)
				return (sv_send_channel(group, 1));
		}
		else if (*group->ptr == 'k')
			ft_strncpy(group->on->key, **cmd, CHANKEY_LEN);
	}
	sv_send_channel(group, 0);
}

static void			sv_change_mode(t_grp *group)
{
	static int		chan_nbr[] = { CH_MODS1, CH_MODS2, CH_MODS3 };
	char			*tmp;

	tmp = ft_strchr(CHAN_MODES, *group->ptr);
	if ((group->c && group->on->cmode & chan_nbr[tmp - CHAN_MODES]) ||
		(!group->c && !(group->on->cmode & chan_nbr[tmp - CHAN_MODES])) ||
		(group->c && (*group->ptr == 'p' && group->on->cmode & CHFL_SECRET)) ||
		(group->c && (*group->ptr == 's' && group->on->cmode & CHFL_PRIV)))
		return ;
	if (group->c)
		group->on->cmode |= chan_nbr[tmp - CHAN_MODES];
	else
		group->on->cmode &= ~(chan_nbr[tmp - CHAN_MODES]);
	sv_send_channel(group, 0);
}

static void			sv_get_mode(t_grp *group, char **cmd, t_server *server)
{
	group->c = 1;
	group->ptr = *cmd;
	group->mdr[1] = 0;
	while (*group->ptr)
	{
		group->mdr[0] = *group->ptr;
		if (*group->ptr == '+')
			group->c = 1;
		else if (*group->ptr == '-')
			group->c = 0;
		else if (!ft_strchr(CHAN_MODES, *group->ptr))
			sv_err(ERR_UNKNOWNMODE, group->from, group->mdr, group->on->name);
		else if (*group->ptr == 'o' || *group->ptr == 'v')
			sv_chan_user_mode(group, &cmd, server);
		else if (*group->ptr == 'l' || *group->ptr == 'k')
			sv_change_more(group, &cmd);
		else if (*group->ptr != 'O' &&
				(*group->ptr != 'a' || *group->on->name == '&' ||
				(*group->on->name == '!' && group->list->mode & CHFL_CREATOR)))
			sv_change_mode(group);
		group->ptr++;
	}
}

void				sv_channel_mode(char **cmds, t_chan *channel, t_client *client, t_server *server)
{
	t_grp			group;

	if (*channel->name == '+')
		return (sv_err(ERR_NOCHANMODES, client, channel->name));
	group.list = channel->users;
	while (group.list)
	{
		if (((t_client *)group.list->is)->socket.fd == client->socket.fd)
			break ;
		group.list = group.list->next;
	}
	if (group.list == NULL)
		return (sv_err(ERR_USERNOTINCHANNEL, client, client->inf->nick, channel->name));
	if (!(group.list->mode & CHFL_CREATOR) && !(group.list->mode & CHFL_CHANOP))
		return (sv_err(ERR_CHANOPRIVSNEEDED, client, channel->name));
	group.from = client;
	group.on = channel;
	group.to = NULL;
	sv_get_mode(&group, cmds, server);
}
