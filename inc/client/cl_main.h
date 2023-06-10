/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_main.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/12 18:46:47 by gbourgeo          #+#    #+#             */
/*   Updated: 2023/06/10 15:10:13 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CL_MAIN_H
# define CL_MAIN_H

# include <stdbool.h>
# include <ncurses.h>
# include "err_list.h"
# include "common.h"

# define CONNECT	{ "CONNECT", cl_connect }
# define HELP		{ "HELP", cl_help }
# define NICK		{ "NICK", cl_nick }
# define PASS		{ "PASS", cl_pass }
# define QUIT		{ "QUIT", cl_quit }
# define USER		{ "USER", cl_user }
# define END		{ NULL, cl_nosuchcommand }

typedef enum	cl_log_e
{
	CL_LOG_DEBUG = 0,
	CL_LOG_INFO = 1,
	CL_LOG_WARNING = 2,
	CL_LOG_ERROR = 3,
	CL_LOG_FATAL = 4,
}				cl_log_t;

# define cl_log(log_level, error_str, client) cl_log_real(log_level, __FILE__, __LINE__, error_str, client)

/*
** Ncurses related enum and structure
*/
enum
{
	CLIENT_TITLE_COLOR = 8,
	CL_PURPLE,
	CL_WHITE,
	CL_YELLOW,
	CL_RED,
	CL_LIGHT_BLUE,
	CL_WEIRD,
};

typedef struct		s_windows
{
	WINDOW			*main;		/**< @brief Main ncurse window */
	WINDOW			*chatbox;	/**< @brief Output box */
	WINDOW			*chatwin;	/**< @brief Output window */
	WINDOW			*textbox;	/**< @brief Input text box */
	WINDOW			*textwin;	/**< @brief Input text window */
	char			*ptr;		/**< @brief Position Pointer of the input ringbuffer */
} t_windows;
typedef struct		s_client
{
	// struct termios	tattr;				/**< @brief Terminal attributes */
	t_windows		windows;
	bool			stop;				/**< @brief Client loop breaker */
	int				sock;				/**< @brief Server socket */
	char			**user;				/**< @brief USER command save */
	char			*pass;				/**< @brief PASS command save */
	char			nick[NICK_LEN + 1];	/**< @brief NICK command save */
	t_buf			read;				/**< @brief Input ringbuffer. Read from client, write to server */
	t_buf			write;				/**< @brief Output ringbuffer. Read from server, write to client */
}					t_client;

typedef struct		s_cmd
{
	char			*name;
	void			(*fct)(char **, t_client *);
}					t_cmd;

int					create_ncurses_chat(t_client *client);
int					create_ncurses_text(t_client *client);
int					cl_default(int ret, t_client *client);
int					cl_ctrl_c(__attribute__((unused)) int ret, t_client *client);
int					cl_ctrl_d(__attribute__((unused)) int ret, t_client *client);
int					cl_lf(__attribute__((unused)) int ret, t_client *client);
int					cl_backspace(__attribute__((unused)) int ret, t_client *client);
int					cl_key_dc(__attribute__((unused)) int ret, t_client *client);
int					cl_key_left(__attribute__((unused)) int ret, t_client *client);
int					cl_key_right(__attribute__((unused)) int ret, t_client *client);

const char			*cl_geterror(errnum_list_e errnum, t_client *client, ...);
void				cl_log_real(cl_log_t log_level, const char *file, const int line,
								const char *err, t_client *client);
int					cl_getaddrinfo(char **argv, t_client *client);
void				cl_loop(t_client *client);
int					read_client_input(t_client *client);
void				read_from_server(t_client *client);
void				write_to_client(t_client *client);
void				write_to_server(t_client *client);

int					cl_get(t_client *cl);
int					cl_put(t_client *cl);
void				cl_send(int fd, char *cmd, char *param, char **next);

void				cl_connect(char **cmds, t_client *cl);
void				cl_help(char **cmds, t_client *cl);
void				cl_nick(char **cmds, t_client *e);
void				cl_nosuchcommand(char **cmds, t_client *e);
void				cl_pass(char **cmds, t_client *cl);
void				cl_quit(char **cmds, t_client *cl);
void				cl_user(char **cmds, t_client *cl);

#endif
