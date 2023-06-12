/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_commands.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/06/02 03:00:44 by gbourgeo          #+#    #+#             */
/*   Updated: 2023/06/12 14:14:48 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <unistd.h>
#include "err_list.h"
#include "cl_main.h"

extern err_list_t g_err_list[];

static int		cl_nick_check(char *nick)
{
	int			i;

	if (!nick || !*nick)
		return (1);
	if (sv_strcmp(nick, "anonymous") || ft_isalpha(*nick) || ISSPECIAL(*nick))
	{
		i = 1;
		while (nick[i] && i <= NICK_LEN)
		{
			if (ft_isalpha(nick[i]) || ft_isdigit(nick[i]) ||
				ISSPECIAL(nick[i]) || nick[i] == '-')
				i++;
			else
				return (2);
		}
		return (0);
	}
	return (2);
}

void			cl_nick(char **cmds, t_client *client)
{
	int			err;

	err = cl_nick_check(cmds[1]);
	if (err == 1)
	{
		cl_log(CL_LOG_ERROR, client, cl_geterror(ERR_NONICKNAMEGIVEN, client, cmds[0]));
	}
	else if (err == 2)
	{
		cl_log(CL_LOG_ERROR, client, cl_geterror(ERR_ERRONEUSNICKNAME, client, cmds[0]));
	}
	else
		ft_strncpy(client->nick, cmds[1], NICK_LEN);
}

void			cl_help(char **cmds, t_client *client)
{
	(void)cmds;
	wprintw(client->windows.chatwin, "The commands described here are used to register a connection with an \n");
	wprintw(client->windows.chatwin, "IRC server as a user as well as to correctly disconnect.\n\n");
	wprintw(client->windows.chatwin, "A \"PASS\" command is not required for a client connection to be \n");
	wprintw(client->windows.chatwin, "registered, but it MUST precede the latter of the NICK/USER\n");
	wprintw(client->windows.chatwin, "combination (for a user connection) or the SERVICE command (for a\n");
	wprintw(client->windows.chatwin, "service connection). The RECOMMENDED order for a client to register\n");
	wprintw(client->windows.chatwin, "is as follows:\n\n");
	wprintw(client->windows.chatwin, "                           1. Pass message\n");
	wprintw(client->windows.chatwin, "           2. Nick message                2. Service message\n");
	wprintw(client->windows.chatwin, "           3. User message\n\n");
	wprintw(client->windows.chatwin, "Upon success, the client will receive an RPL_WELCOME (for users) or\n");
	wprintw(client->windows.chatwin, "RPL_YOURESERVICE (for services) message indicating that the\n");
	wprintw(client->windows.chatwin, "connection is now registered and known the to the entire IRC network.\n");
	wprintw(client->windows.chatwin, "The reply message MUST contain the full client identifier upon which\n");
	wprintw(client->windows.chatwin, "it was registered.\n\n");
	wprintw(client->windows.chatwin, "PASS <passwordhere>\n");
	wprintw(client->windows.chatwin, "NICK <nickname>\n");
	wprintw(client->windows.chatwin, "USER <user> <mode> <unused> <realname>\n");
	wprintw(client->windows.chatwin, "CONNECT <target_server> [port]\n");
	wprintw(client->windows.chatwin, "QUIT\n");
	wprintw(client->windows.chatwin, "HELP\n");
	wrefresh(client->windows.chatwin);
}

void			cl_quit(char **cmds, t_client *client)
{
	(void)cmds;
	cl_log(CL_LOG_FATAL, client, "client quit");
	client->stop = true;
}

void			cl_connect(char **cmds, t_client *client)
{
	if (!cmds[1])
		return (cl_log(CL_LOG_ERROR, client, cl_geterror(ERR_NEEDMOREPARAMS, client, cmds[0])));
	if (cl_getaddrinfo(cmds, client))
		return ;
	if (client->pass && sleep(1))
		cl_send(client->sock, "PASS ", client->pass, NULL);
	if (*client->nick && sleep(1))
		cl_send(client->sock, "NICK ", client->nick, NULL);
	if (client->user && sleep(1))
		cl_send(client->sock, "USER ", *client->user, client->user);
}
