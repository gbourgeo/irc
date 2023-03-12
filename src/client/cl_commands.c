/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_commands.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/06/02 03:00:44 by gbourgeo          #+#    #+#             */
/*   Updated: 2023/03/12 15:39:35 by gbourgeo         ###   ########.fr       */
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
		cl_log(CL_LOG_ERROR, cl_geterror(ERR_NONICKNAMEGIVEN, client, cmds[0]), client);
	}
	else if (err == 2)
	{
		cl_log(CL_LOG_ERROR, cl_geterror(ERR_ERRONEUSNICKNAME, client, cmds[0]), client);
	}
	else
		ft_strncpy(client->nick, cmds[1], NICK_LEN);
}

void			cl_help(char **cmds, t_client *cl)
{
	(void)cmds;
	(void)cl;
	ft_putstr("The commands described here are used to register a connection");
	ft_putstr(" with an \nIRC server as a user as well as to correctly ");
	ft_putstr("disconnect.\n\n");
	ft_putstr("A \"PASS\" command is not required for a client connection ");
	ft_putstr("to be \nregistered, but it MUST precede the latter of the ");
	ft_putstr("NICK/USER \ncombination (for a user connection) or the SERVICE");
	ft_putstr(" command (for a\nservice connection). The RECOMMENDED order ");
	ft_putstr("for a client to register\nis as follows:\n\n");
	ft_putstr("                           1. Pass message\n");
	ft_putstr("           2. Nick message                2. Service message\n");
	ft_putstr("           3. User message\n\n");
	ft_putstr("Upon success, the client will receive an RPL_WELCOME ");
	ft_putstr("(for users) or\nRPL_YOURESERVICE (for services) message ");
	ft_putstr("indicating that the\nconnection is now registered and known ");
	ft_putstr("the to the entire IRC network.\nThe reply message MUST contain");
	ft_putstr(" the full client identifier upon which\nit was registered.\n\n");
	ft_putendl("CONNECT <target_server> [port]");
	ft_putendl("HELP");
	ft_putendl("NICK <nickname>");
	ft_putendl("PASS <passwordhere>");
	ft_putendl("USER <user> <mode> <unused> <realname>");
	ft_putendl("QUIT");
}

void			cl_quit(char **cmds, t_client *client)
{
	ft_free(&cmds);
	if (client->sock > 0)
		close(client->sock);
	ft_strdel(&client->pass);
	ft_free(&client->user);
	tcsetattr(STDIN_FILENO, TCSANOW, &client->tattr);
	ft_memset(client, 0, sizeof(*client));
}

void			cl_connect(char **cmds, t_client *client)
{
	char		*port;

	if (!cmds[1])
		return (cl_log(CL_LOG_ERROR, cl_geterror(ERR_NEEDMOREPARAMS, client, cmds[0]), client));
	port = (cmds[2]) ? cmds[2] : ft_strrchr(cmds[1], ':');
	if (port && !cmds[2])
		*port++ = 0;
	if (port == NULL || *port == 0)
		port = DEF_PORT;
	if (cl_getaddrinfo(cmds[1], port, client))
		return ;
	if (client->pass && sleep(1))
		cl_send(client->sock, "PASS ", client->pass, NULL);
	if (*client->nick && sleep(1))
		cl_send(client->sock, "NICK ", client->nick, NULL);
	if (client->user && sleep(1))
		cl_send(client->sock, "USER ", *client->user, client->user);
}
