/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_user_mode.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/19 04:19:29 by gbourgeo          #+#    #+#             */
/*   Updated: 2017/04/11 08:36:52 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sv_main.h"

static void			modify_chan_nbusers(t_client *cl)
{
	t_listing		*ch;

	ch = cl->chans;
	while (ch)
	{
		((t_chan *)ch->is)->nbusers--;
		((t_chan *)ch->is)->invisibl++;
		ch = ch->next;
	}
}

static void			change_user_mode(char c, char mode, t_client *cl)
{
	static int		user_nbr[] = { US_MODS1, US_MODS2 };
	char			*tmp;
	char			ptr[2];

	tmp = ft_strchr(USER_MODES, mode);
	if ((c && cl->inf->umode & user_nbr[tmp - USER_MODES]) ||
		(!c && !(cl->inf->umode & user_nbr[tmp - USER_MODES])))
		return ;
	cl->inf->umode = (c) ? cl->inf->umode | user_nbr[tmp - USER_MODES] :
		cl->inf->umode & ~(user_nbr[tmp - USER_MODES]);
	if (c && *tmp == 'i')
		modify_chan_nbusers(cl);
	sv_cl_write(":", cl);
	sv_cl_write(cl->inf->nick, cl);
	sv_cl_write("!~", cl);
	sv_cl_write(cl->inf->username, cl);
	sv_cl_write("@", cl);
	sv_cl_write((*cl->socket.host) ? cl->socket.host : cl->socket.addr, cl);
	sv_cl_write(" MODE ", cl);
	sv_cl_write(cl->inf->nick, cl);
	sv_cl_write((c) ? " :+" : " :-", cl);
	*ptr = mode;
	*(ptr + 1) = 0;
	sv_cl_write(ptr, cl);
	sv_cl_write(END_CHECK, cl);
}

void				sv_user_mode(char **cmds, t_client *cl)
{
	char			*ptr;
	char			c;

	c = 1;
	while (*cmds)
	{
		ptr = *cmds;
		while (*ptr)
		{
			if (*ptr == '+')
				c = 1;
			else if (*ptr == '-')
				c = 0;
			else if (!ft_strchr(USER_MODES, *ptr))
				return (sv_err(ERR_UMODEUNKNOWNFLAG, NULL, NULL, cl));
			else if (*ptr != 'a' &&
					((c && *ptr != 'O' && *ptr != 'o') ||
					(!c && *ptr != 'r')))
				change_user_mode(c, *ptr, cl);
			ptr++;
		}
		cmds++;
	}
}
