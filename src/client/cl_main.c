/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/12 18:37:59 by gbourgeo          #+#    #+#             */
/*   Updated: 2023/01/03 21:18:16 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termios.h>
#include <signal.h>
#include <unistd.h>
#include "ft_dprintf.h"
#include "cl_main.h"

t_client	g_client;

static void		cl_init(void)
{
	struct termios	tattr;

	ft_memset(&g_client, 0, sizeof(g_client));
	g_client.stop = false;
	ft_init_ringbuf(&g_client.read, g_client.rd, BUFF);
	ft_init_ringbuf(&g_client.write, g_client.wr, BUFF);
	if (!isatty(STDIN_FILENO))
	{
		ft_dprintf(STDERR_FILENO, "** \e[31mERROR\e[0m :Not in a terminal\n");
		exit(EXIT_FAILURE);
	}
	tcgetattr(STDIN_FILENO, &g_client.tattr);
	tcgetattr(STDIN_FILENO, &tattr);
	tattr.c_cflag &= ~(ICANON | ECHO);
	tattr.c_cc[VMIN] = 1;
	tattr.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &tattr);
}

int				main(int ac, char **av)
{
	char		*port;

	(void)ac;
	port = DEF_PORT;
	ft_putendl("*****************************");
	ft_putendl("* Welcome to GBO-IRC client *");
	ft_putendl("*****************************");
	// signal(SIGINT, SIG_IGN);
	cl_init();
	if (!av[1])
		ft_putendl("\e[34mUsage: ./client [host_name[:port]] [port]\e[0m");
	else if (av[2])
		port = av[2];
	else if ((port = ft_strrchr(av[1], ':')) != NULL)
		*port++ = 0;
	if (av[1])
		cl_getaddrinfo(av[1], port, &g_client);
	cl_loop(&g_client);
	cl_quit(NULL, &g_client);
	ft_putendl("Client quit.");
	return (0);
}
