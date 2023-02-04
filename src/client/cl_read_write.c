/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_read_write.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/06/01 22:53:32 by gbourgeo          #+#    #+#             */
/*   Updated: 2023/01/03 21:08:35 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <unistd.h>
#include "cl_main.h"

static void	read_command(t_client *client)
{
	static t_cmd	cmds[] = { CONNECT, HELP, NICK, PASS, QUIT, USER, END };
	char			cmd[BUFF];
	char			**args;
	int				i;

	i = 0;
	while (client->read.head != client->read.tail)
	{
		cmd[i++] = *client->read.head++;
		if (client->read.head >= client->read.end)
			client->read.head = client->read.start;
	}
	cmd[i - 1] = '\0';
ft_putendl(cmd);
	if ((args = ft_split_whitespaces(cmd)) == NULL)
		return (cl_error("read_command: split_whitespaces", client));
	if (*args && **args)
	{
		i = 0;
		while (cmds[i].name && sv_strcmp(cmds[i].name, args[0]))
			i++;
		cmds[i].fct(args, client);
	}
	ft_free(&args);
}

void		read_client(t_client *client)
{
	int		ret;

ft_putendl("READ !");
	ret = read(STDIN_FILENO, client->read.tail, client->read.len);
	if (ret < 0)
		return (cl_error("read_client: read", client));
	if (ret == 0)
		return (cl_error("read_client: disconnected", client));
	while (ret--)
	{
		ft_putnbr(*client->read.tail);
		if (ft_isalnum(*client->read.tail)
		|| *client->read.tail == '\n')
		{
			if (*client->read.tail == '\n')
			{
				write(STDOUT_FILENO, client->read.tail, 1);
				if (client->sock == -1)
					read_command(client);
			}
			else
				write(STDOUT_FILENO, client->read.tail, 1);
			if (client->read.len == 1)
				break ;
			if (++client->read.tail == client->read.end)
				client->read.tail = client->read.start;
			if (--client->read.len == 0)
				client->read.len = BUFF;
		}
		else if (*client->read.tail == 3)
			client->stop = true;
	}
}

void		write_client(t_client *client)
{
	int	ret;
	int	bytes_write;

	if (client->write.head < client->write.tail)
	{
		bytes_write = client->write.tail - client->write.head;
		ret = write(STDOUT_FILENO, client->write.head, bytes_write);
	}
	else
	{
		bytes_write = client->write.tail - client->write.end;
		ret = write(STDOUT_FILENO, client->write.head, bytes_write);
	}
	if (ret == -1)
		return (cl_error("write_client: write", client));
	client->write.head += ret;
	client->write.len += ret;
	if (client->write.head >= client->write.end)
		client->write.head = client->write.start;
}

void		read_server(t_client *client)
{
	int	ret;

	ret = recv(client->sock, client->write.tail, client->write.len, 0);
	if (ret == -1)
		return (cl_error("read_server: recv", client));
	if (ret == 0)
	{
		ft_putendl("read_server: Connection closed by foreign host");
		close(client->sock);
		client->sock = -1;
		return ;
	}
	write(STDOUT_FILENO, client->write.tail, ret);
}

void	write_server(t_client *client)
{
	int	ret;
	int	bytes_send;

	if (client->read.head < client->read.tail)
	{
		bytes_send = client->read.tail - client->read.head;
		ret = send(client->sock, client->read.head, bytes_send, 0);
		client->read.head = client->read.tail;
	}
	else
	{
		bytes_send = client->read.end - client->read.head;
		ret = send(client->sock, client->read.head, bytes_send, 0);
		client->read.head = client->read.start;
	}
	if (ret < 0)
		return (cl_error("write_server: send", client));
	if (ret == 0)
		ft_putendl("write_server: server disconnected");
	else if (ret != bytes_send)
		cl_error("write_server: sent less bytes than expected", client);
	client->read.len += bytes_send;
}
