/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_main.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/12 18:46:47 by gbourgeo          #+#    #+#             */
/*   Updated: 2023/03/12 15:36:21 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CL_MAIN_H
# define CL_MAIN_H

# include <stdbool.h>
# include <termios.h>
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

typedef struct		s_client
{
	struct termios	tattr;				/**< @brief Terminal attributes */
	bool			stop;				/**< @brief Client loop breaker */
	int				sock;				/**< @brief Server socket */
	char			**user;				/**< @brief USER command save */
	char			*pass;				/**< @brief PASS command save */
	char			nick[NICK_LEN + 1];	/**< @brief NICK command save */
	t_buf			read;				/**< @brief Input ringbuffer */
	char			rd[BUFF];			/**< @brief Input buffer */
	t_buf			write;				/**< @brief Output ringbuffer */
	char			wr[BUFF];			/**< @brief Output buffer */
}					t_client;

typedef struct		s_cmd
{
	char			*name;
	void			(*fct)(char **, t_client *);
}					t_cmd;

const char			*cl_geterror(errnum_list_e errnum, t_client *client, ...);
void				cl_log_real(cl_log_t log_level, const char *file, const int line,
								const char *err, t_client *cl);
int					cl_getaddrinfo(char *addr, char *port, t_client *cl);
void				cl_loop(t_client *client);
void				read_client(t_client *cl);
void				read_server(t_client *cl);
void				write_client(t_client *client);
void				write_server(t_client *client);

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
