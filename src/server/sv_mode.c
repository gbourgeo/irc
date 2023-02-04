/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_mode.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/18 05:18:25 by gbourgeo          #+#    #+#             */
/*   Updated: 2023/01/01 00:05:13 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_snprintf.h"
#include "sv_main.h"

static void	rpl_cmodeis(t_chan *chan, t_client *client, t_server *server)
{
	static int	cvalues[] = { CH_MODS1, CH_MODS2, CH_MODS3 };
	char		msg[BUFF];
	char		*cmodes;
	size_t		i;

	ft_snprintf(msg,
		sizeof(msg),
		":%s 221 %s :+",
		server->name,
		client->inf->nick);
	cmodes = CHAN_MODES;
	i = 0;
	while (i < sizeof(cvalues) / sizeof(cvalues[0]))
	{
		if (chan->cmode & cvalues[i])
			ft_strncat(msg, &cmodes[i], 1);
		i++;
	}
	ft_strncat(msg, END_CHECK, sizeof(msg) - ft_strlen(msg));
	sv_cl_write(msg, client);
}

static void	search_channel(char **channel, t_client *client, t_server *server)
{
	t_chan	*chan;

	chan = server->chans;
	while (chan)
	{
		if (!sv_strcmp(chan->name, *channel))
		{
			if (!channel[1])
				return (rpl_cmodeis(chan, client, server));
			return (sv_channel_mode(channel + 1, chan, client, server));
		}
		chan = chan->next;
	}
	sv_err(ERR_NOSUCHCHANNEL, client, *channel);
}

static void	rpl_umodeis(t_client *client, t_server *server)
{
	static int	uvalues[] = { US_MODS1, US_MODS2 };
	char		*umodes;
	char		msg[BUFF];
	size_t		i;

	ft_snprintf(msg,
		sizeof(msg),
		":%s 221 %s :+",
		server->name,
		client->inf->nick);
	umodes = USER_MODES;
	i = 0;
	while (i < sizeof(uvalues) / sizeof(uvalues[0]))
	{
		if (client->inf->umode & uvalues[i])
			ft_strncat(msg, &umodes[i], 1);
		i++;
	}
	ft_strncat(msg, END_CHECK, sizeof(msg) - ft_strlen(msg));
	sv_cl_write(msg, client);
}

/**
 * @brief Set or remove options from a given target
 * 
 * @param cmds <target> [<modestring> [<mode arguments>...]]
 * @param server Server
 * @param client Client
 */
void		sv_mode(char **cmds, t_server *server, t_client *client)
{
	if (!cmds[0])
		sv_err(ERR_NEEDMOREPARAMS, client, "MODE");
	else if (client->inf->umode & USR_RESTRICT)
		sv_err(ERR_RESTRICTED, client);
	else if (ISCHAN(**cmds))
		search_channel(cmds, client, server);
	else if (sv_strcmp(client->inf->nick, *cmds))
	{
		if (cmds[1])
			sv_err(ERR_USERSDONTMATCH, client, client->inf->nick, *cmds);
		else
			sv_err(ERR_USERSMODE, client);
	}
	else if (!cmds[1])
		rpl_umodeis(client, server);
	else
		sv_user_mode(cmds + 1, client);
}
