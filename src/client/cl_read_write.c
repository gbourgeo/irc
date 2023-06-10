/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_read_write.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/06/01 22:53:32 by gbourgeo          #+#    #+#             */
/*   Updated: 2023/06/10 15:14:51 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include "cl_main.h"

/**
 * @brief Fonction de lecture de l'entrée standart du client.
 * 
 * @param client Structure principale du client
 */
int			read_client_input(t_client *client)
{
	static struct {
		int	value;
		int	(*hdlr)(int, t_client *);
	} s_character[] = {
		{ 3, cl_ctrl_c }, { 4, cl_ctrl_d }, { 10, cl_lf },
		{ KEY_BACKSPACE, cl_backspace }, { KEY_DC, cl_key_dc },
		// { KEY_UP, cl_key_up }, { KEY_DOWN, cl_key_down },
		{ KEY_LEFT, cl_key_left }, { KEY_RIGHT, cl_key_right },
	};
	unsigned long	i;
	int				ret;

	ret = wgetch(client->windows.textwin);
	if (ret == ERR)
		cl_log(CL_LOG_ERROR, "read", client);
	else if (ret == 0)
		cl_log(CL_LOG_FATAL, "client quit", client);
	else
	{
		i = 0;
		while (i < sizeof(s_character) / sizeof(s_character[0]))
		{
			if (ret == s_character[i].value)
				return (s_character[i].hdlr(ret, client));
			i++;
		}
		return (cl_default(ret, client));
	}
	return (ret);
}

/**
 * @brief Fonction d'écriture sur la socket du serveur.
 * 
 * Principe du buffer tournant : (H=head, T=tail)
 * 		| | | |H| | | | |T| | | |
 * 		       ---------> bytes_send
 *
 * 		| | | |T| | | | |H| | | |
 * 		                 ------> bytes_send (first)
 * 		| | | |T| | | | |H| | | |
 * 		-------> bytes_send (second)
 * 
 * @param client Structure principale du client
 */
void	write_to_server(t_client *client)
{
	int	ret;
	int	send_b;

	if (client->read.head < client->read.tail)
		send_b = client->read.tail - client->read.head;
	else
		send_b = client->read.end - client->read.head;
	if (send_b == 0)
		return ;
	ret = send(client->sock, client->read.head, send_b, MSG_DONTWAIT | MSG_NOSIGNAL);
	if (ret < 0 && errno != EAGAIN && errno != EWOULDBLOCK)
	{
		cl_log(CL_LOG_ERROR, "send", client);
		close(client->sock);
		client->sock = -1;
	}
	if (ret == 0)
	{
		cl_log(CL_LOG_INFO, "Connection closed by foreign host", client);
		close(client->sock);
		client->sock = -1;
	}
	else if (ret != send_b)
		cl_log(CL_LOG_WARNING, "sent less bytes than expected", client);
	ft_move_head(ret, &client->read);
}

/**
 * @brief Fonction de lecture sur la socket du serveur.
 * 
 * @param client Structure principale du client
 */
void		read_from_server(t_client *client)
{
	ssize_t	ret;
	int		read_b;

	read_b = client->write.end - client->write.tail;
	if (read_b == 0 || client->write.len >= BUFF)
		return ;
	ret = recv(client->sock, client->write.tail, read_b, MSG_DONTWAIT | MSG_NOSIGNAL);
	if (ret < 0 && errno != EAGAIN && errno != EWOULDBLOCK)
	{
		cl_log(CL_LOG_ERROR, "recv", client);
		close(client->sock);
		client->sock = -1;
	}
	else if (ret == 0)
	{
		cl_log(CL_LOG_INFO, "Connection closed by foreign host", client);
		close(client->sock);
		client->sock = -1;
	}
	ft_move_tail(ret, &client->write);
}

/**
 * @brief Fonction d'écriture sur la sortie standart les données
 * reçues du serveur.
 *
 * @param client Structure principale du client
 */
void		write_to_client(t_client *client)
{
	ssize_t	ret;
	int		write_b;

	if (client->write.len == 0)
		return ;
	if (client->write.tail <= client->write.head)
		write_b = client->write.end - client->write.head;
	else
		write_b = client->write.tail - client->write.head;
	ret = wprintw(client->windows.chatwin, "%.*s", write_b, client->write.head);
	if (ret == ERR)
		cl_log(CL_LOG_ERROR, "write", client);
	ft_move_head(ret, &client->write);
}
