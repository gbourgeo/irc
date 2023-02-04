/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_check_clients.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/17 04:52:38 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/10/21 14:20:41 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sv_main.h"
#include <time.h>

static void		wrong_username(t_client *client, t_server *server)
{
	char		*ptr;

	ptr = "Your username is invalid. Please make sure that your "\
		"username contains only alphanumeric characters.";
	client->leaved = 2;
	client->reason = "Invalid username";
	sv_notice(ptr, client, server);
}

static void		error_loggin_in(char *err, t_client *client, t_server *server)
{
	char		*ptr;

	if (*err == 0)
	{
		ptr = "This nickname is registered. You have 30 sec to choose a "\
			"different nickname or it will be changed automatically.";
		sv_notice(ptr, client, server);
		time(&client->inf->must_change_nick);
	}
	else
	{
		client->leaved = 1;
		client->reason = err;
		sv_notice("Logging to the server failed. Try again.", client, server);
	}
}

static void		check_registered(t_client *client, t_server *server)
{
	t_file		*us;

	us = server->users;
	while (us)
	{
		if (!sv_strcmp(us->nick, client->inf->nick))
		{
			if (us->registered <= 0 && client->inf->pass
			&& !ft_strcmp(us->pass, client->inf->pass))
			{
				free(client->inf->pass);
				ft_free(&client->inf->realname);
				free(client->inf);
				client->inf = us;
				break ;
			}
			sv_welcome(server, client);
			return (error_loggin_in("", client, server));
		}
		us = us->next;
	}
	sv_welcome(server, client);
}

static void		check_allowed(t_client *client, t_server *server)
{
	t_user		*user;

	if (sv_allowed(&client->socket, server->conf.allowed_user))
		return (check_registered(client, server));
	if ((user = sv_allowed(&client->socket, server->conf.pass_user)))
	{
		if (!ft_strcmp(user->passwd, client->inf->pass))
			return (sv_welcome(server, client));
	}
	error_loggin_in("Not allowed to login", client, server);
}

void			sv_check_clients(t_server *server)
{
	t_client	*client;

	client = server->clients;
	while (client)
	{
		if (client->inf->must_change_nick > 0)
			sv_nick_change(client, server);
		if (client->leaved)
		{
			if (client->wr.len == 0)
				client = sv_clear_client(client, server);
		}
		else if (client->inf->registered <= 0 && *client->inf->nick
		&& *client->inf->username)
		{
			if (!ft_strisalnum(client->inf->username))
				wrong_username(client, server);
			else
				check_allowed(client, server);
		}
		if (client)
			client = client->next;
	}
}
