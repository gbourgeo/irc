/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_cl_read.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/06/06 05:18:09 by gbourgeo          #+#    #+#             */
/*   Updated: 2023/06/13 13:09:07 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include "ft_snprintf.h"
#include "commands.h"
#include "sv_main.h"

extern t_command	g_commands[];
extern size_t		g_commands_size;

static void		rpl_quit_msg(t_chan *chan, t_client *to, t_client *client)
{
	char	msg[BUFF];

	if (chan->cmode & CHFL_ANON)
	{
		ft_snprintf(msg,
			sizeof(msg),
			":anonymous!~anonymous@anonymous LEAVE %s" END_CHECK,
			chan->name);
	}
	else
	{
		ft_snprintf(msg,
			sizeof(msg),
			":%s!~%s@%s QUIT :Remote host closed the connection" END_CHECK,
			client->inf->nick,
			client->inf->username,
			(*client->socket.host) ? client->socket.host : client->socket.addr);
	}
	sv_cl_write(msg, to);
}

static void		sv_cl_quit(t_client *cl)
{
	t_listing	*chan;
	t_listing	*to;

	chan = cl->chans;
	while (chan)
	{
		to = ((t_chan *)chan->is)->users;
		while (to)
		{
			if (((t_client *)to->is)->socket.fd != cl->socket.fd)
				rpl_quit_msg(chan->is, to->is, cl);
			to = to->next;
		}
		chan = chan->next;
	}
	cl->leaved = 1;
	cl->reason = "Remote host closed the connection";
}

static void		sv_cmd_client(t_server *server, t_client *client)
{
	char	**cmds;
	size_t	nb;

	nb = 0;
	if ((cmds = sv_split(&client->rd)) == NULL)
		return (sv_error(LOG_LEVEL_FATAL, "Out of memory", server));
	if (*cmds && **cmds)
	{
		while (nb < g_commands_size && sv_strcmp(g_commands[nb].name, cmds[0]))
			nb++;
		if (g_commands[nb].name)
		{
			if (client->inf->registered > 0 || g_commands[nb].privilege == 0)
				g_commands[nb].fct(cmds + 1, server, client);
			else if (client->inf->registered == 0)
			{
				sv_err(ERR_NOTREGISTERED, client);
				client->inf->registered = -1;
			}
		}
		else if (client->inf->registered > 0)
			sv_err(ERR_UNKNOWNCOMMAND, client, cmds[0]);
	}
	ft_free(&cmds);
}

void			sv_cl_read(t_server *server, t_client *client)
{
	int	ret;
	int	len;

	len = client->rd.end - client->rd.tail;
	ret = recv(client->socket.fd, client->rd.tail, len, MSG_DONTWAIT | MSG_NOSIGNAL);
	if (ret <= 0)
		return (sv_cl_quit(client));
	while (ret--)
	{
		if (*client->rd.tail == '\n')
		{
			char buf[BUFF + 1];
			if (client->rd.tail > client->rd.head)
			{
				ft_strncpy(buf, client->rd.head, client->rd.tail - client->rd.head);
				buf[client->rd.tail - client->rd.head] = 0;
			}
			else
			{
				ft_strncpy(buf, client->rd.head, client->rd.end - client->rd.head);
				buf[client->rd.end - client->rd.head] = 0;
				ft_strncat(buf, client->rd.start, client->rd.tail - client->rd.start);
				buf[(client->rd.end - client->rd.head) + (client->rd.tail - client->rd.start)] = 0;
			}
			sv_log(LOG_LEVEL_DEBUG, LOG_TYPE_CLIENT, "[%s] %s", client->uid, buf);
			sv_cmd_client(server, client);
			if ((client->rd.head = client->rd.tail + 1) >= client->rd.end)
				client->rd.head = client->rd.start;
		}
		ft_move_tail(1, &client->rd);
	}
}
