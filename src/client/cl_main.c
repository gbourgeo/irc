/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/12 18:37:59 by gbourgeo          #+#    #+#             */
/*   Updated: 2023/06/13 10:48:37 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termios.h>
#include <signal.h>
#include <unistd.h>
#include "cl_main.h"

t_client	g_client;

static void		cl_init_pairs(void)
{
	init_pair(CLIENT_TITLE_COLOR, COLOR_YELLOW, -1);
	init_pair(CL_PURPLE, COLOR_MAGENTA, -1);
	init_pair(CL_WHITE, COLOR_WHITE, -1);
	init_pair(CL_YELLOW, COLOR_YELLOW, -1);
	init_pair(CL_RED, COLOR_RED, -1);
	init_pair(CL_LIGHT_BLUE, COLOR_CYAN, -1);
	init_pair(CL_WEIRD, COLOR_WHITE, COLOR_RED);
}

static int		cl_init(void)
{
	g_client.stop = false;
	ft_init_ringbuf(&g_client.read);
	ft_init_ringbuf(&g_client.write);
	g_client.sendtoserv = false;
	g_client.windows.ptr = g_client.read.tail;
	if ((g_client.windows.main = initscr()) == NULL)
		return (1);
	raw();
	noecho();
	start_color();
	use_default_colors();
	clear();
	cl_init_pairs();
	return (create_ncurses_chat(&g_client) || create_ncurses_text(&g_client));
}

static void		cl_end(void)
{
	if (g_client.windows.chatbox)
		delwin(g_client.windows.chatbox);
	if (g_client.windows.chatwin)
		delwin(g_client.windows.chatwin);
	if (g_client.windows.textbox)
		delwin(g_client.windows.textbox);
	if (g_client.windows.textwin)
		delwin(g_client.windows.textwin);
	if (g_client.windows.main)
		endwin();
	if (g_client.sock > 0)
		close(g_client.sock);
	ft_strdel(&g_client.pass);
	ft_free(&g_client.user);
}

/**
 * Usage: ./client [host_name[:port]] [port]
 */
int				main(__attribute__((unused)) int ac, char **av)
{
	ft_memset(&g_client, 0, sizeof(g_client));
	if (cl_init() == 0)
	{
		if (av[1])
			cl_getaddrinfo(av, &g_client);
		cl_loop(&g_client);
	}
	else
		ft_putendl_fd("\e[31mError initialising client\e[0m", STDERR_FILENO);
	cl_end();
	return (0);
}
