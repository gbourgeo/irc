/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_read_write.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/06/01 22:53:32 by gbourgeo          #+#    #+#             */
/*   Updated: 2023/06/13 13:23:59 by gbourgeo         ###   ########.fr       */
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
		{ KEY_LEFT, cl_key_left }, { KEY_RIGHT, cl_key_right },
		{ 546, cl_ctrl_key_left }, { 561, cl_ctrl_key_right },
		{ KEY_END, cl_key_end }, { KEY_HOME, cl_key_home },
		// { KEY_UP, cl_key_up }, { KEY_DOWN, cl_key_down },
	};
	unsigned long	i;
	int				ret;

	ret = wgetch(client->windows.textwin);
	if (ret == ERR)
		cl_log(CL_LOG_ERROR, client, "read");
	else if (ret == 0)
		cl_log(CL_LOG_FATAL, client, "client quit");
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
	int		ret;
	int		send_b;

	send_b = (client->read.head >= client->read.tail) ?
		client->read.end - client->read.head :
		client->read.tail - client->read.head;
	ret = send(client->sock, client->read.head, send_b, MSG_DONTWAIT | MSG_NOSIGNAL);
	if (ret < 0 && errno != EAGAIN && errno != EWOULDBLOCK)
	{
		cl_log(CL_LOG_ERROR, client, "send()");
		close(client->sock);
		client->sock = -1;
	}
	else if (ret == 0)
	{
		cl_log(CL_LOG_INFO, client, "Connection closed by foreign host");
		close(client->sock);
		client->sock = -1;
	}
	else if (ret != send_b)
		cl_log(CL_LOG_WARNING, client, "sent less bytes than expected");
	else
		client->sendtoserv = false;
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

	if (client->write.len >= BUFF)
		return ;
	read_b = (client->write.tail >= client->write.head) ?
		client->write.end - client->write.tail :
		client->write.head - client->write.tail;
	ret = recv(client->sock, client->write.tail, read_b, MSG_DONTWAIT | MSG_NOSIGNAL);
	if (ret < 0 && errno != EAGAIN && errno != EWOULDBLOCK)
	{
		cl_log(CL_LOG_ERROR, client, "recv()");
		close(client->sock);
		client->sock = -1;
	}
	else if (ret == 0)
	{
		cl_log(CL_LOG_INFO, client, "Connection closed by foreign host");
		close(client->sock);
		client->sock = -1;
	}
	else
		ft_move_tail(ret, &client->write);
}

/**
 * @brief Fonction d'écriture sur la sortie standard des données
 * reçues du serveur.
 *
 * @param client Structure principale du client
 */
void		write_to_client(t_client *client)
{
	char	*ptr;
	int		write_b;

	write_b = (client->write.tail <= client->write.head) ?
		client->write.end - client->write.head :
		client->write.tail - client->write.head;
	ptr = client->write.head;
	do
	{
		if (*ptr == '\r')
			*ptr = ' ';
		if (++ptr >= client->write.end)
			ptr = client->write.start;
	} while (ptr != client->write.tail);
	wprintw(client->windows.chatwin, "%.*s", write_b, client->write.head);
	wrefresh(client->windows.chatwin);
	wrefresh(client->windows.textwin); /* Réinitialise le curseur dans la zone d'input */
	ft_move_head(write_b, &client->write);
}
