/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_main.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/05/12 14:49:14 by gbourgeo          #+#    #+#             */
/*   Updated: 2023/05/29 15:42:46 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SV_MAIN_H
# define SV_MAIN_H

# include <netinet/in.h>
# include <netdb.h>
# include <stdarg.h>
# include <errno.h>
# include <string.h>
# include "common.h"
# include "help.h"
# include "err_list.h"
# include "flags.h"
# include "conf.h"

/*
** Negative value of any of this defines will lead to unexpected behaviors !!!
*/

# define SERVER_SID_LEN 128
# define UID_LEN 9

/*
** MAX_CLIENT			Set the number of clients this program will handle. It can't
**						be over the system limit. (see RLIMIT_NOFILE)
*/
# define MAX_CLIENT 42

/*
** MAX_CLIENT_BY_IP 	Let you choose how many times the same IP can connect.
*/
# define MAX_CLIENT_BY_IP 3

/*
** USERNAME_LEN			Maximum length a client username.
*/
# define USERNAME_LEN 9

/*
** SERV_NAME_LEN		The maximum length a servers' name can be. Over this value,
**						a server name will be truncated.
*/
# define SERVER_NAME_LEN	128

/*
** SERVER_LOCATION_LEN	The maximum server location length.
*/
# define SERVER_LOCATION_LEN	128

/*
** CHAN_LIMIT			The maximum number of channels a client can join.
*/
# define CHAN_LIMIT	120

/*
** CHANNAME_LEN		The maximum length a channels' name can be. Over this value,
**					a channel name will be truncated.
*/
# define CHANNAME_LEN 50

/*
** CHANKEY_LEN		THe maximum length a channel key can be.
*/
# define CHANKEY_LEN 23

/*
** TOPIC_LEN		The maximum length of a topic. Over this value, the
**					topic will be truncated.
*/
# define TOPIC_LEN	80

/*
** Some knowledge:
** - Channels with '&' as prefix are local to the server where they are created.
** - Channels with '+' as prefix do not support channel modes.  This means
**   that all the modes are unset, with the exception of the 't' channel
**   flag which is set.
** - A user who creates a channel with the character '!' as prefix is
**   identified as the "channel creator".
** - Two groups of channels: standard channels which prefix is either '&', '#'
**   or '+', and "safe channels" which prefix is '!'.
*/
# define ISCHAN(c) (c == '#' || c == '+' || c == '&' || c == '!')

/**
 * @brief Enumeration of all log levels
 */
typedef enum
{
	LOG_LEVEL_DEBUG = 0,
	LOG_LEVEL_INFO,
	LOG_LEVEL_WARNING,
	LOG_LEVEL_ERROR,
	LOG_LEVEL_FATAL,
}	e_log_level;

/**
 * @brief Enumeration of all log types
 */
typedef enum
{
	LOG_TYPE_SYSTEM = 0,
	LOG_TYPE_CONF,
	LOG_TYPE_SERVER,
	LOG_TYPE_CLIENT,
}	e_log_type;

/**
 * @brief Enumeration of file descriptors types
 */
enum
{
	FD_FREE,
	FD_SERVER,
	FD_CLIENT
};

/*
** I set a void * in the struct s_listin, why?
** Because in s_fd, that void * is a t_chan *
** and in t_chan, that void * is a t_client *
*/

typedef struct			s_listing
{
	struct s_listing	*prev;
	struct s_listing	*next;
	void				*is;
	int					mode;
}						t_listing;

typedef struct			s_file
{
	char				registered;
	char				username[USERNAME_LEN + 1];
	char				*pass;
	int					umode;
	char				nick[NICK_LEN + 1];
	char				**realname;
	time_t				must_change_nick;
	struct s_file		*next;
}						t_file;

typedef struct			s_socket_info
{
	int					fd;						/* File descriptor to read/send data */
	struct sockaddr		csin;					/* Internet address */
	char				addr[ADDR_LEN];		/* Internet address in human readable format */
	char				host[NI_MAXHOST];	/* Host name */
	char				port[NI_MAXSERV];	/* Port of connection */
}						t_socket;

typedef struct			s_client_info
{
	struct s_client_info	*prev;
	struct s_client_info	*next;
	t_socket			socket;					/* Client socket information */
	char				uid[10];				/* Unique identifier */
	t_file				*inf;					/* Client log'in informations */
	short				type;
	char				*away;
	t_listing			*chans;					/* List of channels client is connected to */
	int					chansnb;				/* Number of channels client is connected to */
	void				(*fct_read)();			/* Function to read client command */
	void				(*fct_write)();			/* Function to send client response */
	t_buf				rd;						/* Ringbuffer for reading */
	t_buf				wr;						/* Ringbuffer for writing */
	char				*queue;
	int					leaved;					/* Says if the client has leaved and need to be deleted from the server */
	char				*reason;				/* The reason he left the server */
}						t_client;

typedef struct			s_chan
{
	struct s_chan		*prev;
	struct s_chan		*next;
	char				name[CHANNAME_LEN + 1];	/* Name of the channel */
	char				topic[TOPIC_LEN + 1];	/* Topic of the channel */
	int					cmode;					/* Channels' mode */
	int					nbusers;				/* Number of visible user */
	int					invisibl;				/* Number of invisible user */
	int					limit;					/* Limit of users */
	char				key[CHANKEY_LEN];		/* Key to join the channel */
	t_listing			*users;					/* List of channels' users */
}						t_chan;

/**
 * Server main structure
 */
typedef struct			s_server
{
	e_log_level			verbose;	/* Server verbosity level */
	t_conf				conf;	/* Server configuration informations */
	char				name[SERVER_NAME_LEN + 1];	/* Server name */
	char				addr[ADDR_LEN];	/* Server address */
	char				location[SERVER_LOCATION_LEN + 1];	/* Server location */
	char				*port;	/* Server port */
	char				sid[SERVER_SID_LEN + 1];	/* Server ID */
	t_socket			v4;	/* Server ip v4 informations */
	t_socket			v6;	/* Server ip v6 informations */
	char				*creation;	/* Server creation date */
	char				running;	/* Server running state */
	t_file				*users;	/* Server users chained list */
	size_t				members;	/* Total number of members connected */
	t_client			*clients;	/* Server clients chained list */
	t_chan				*chans;	/* Server channels chained list */
	fd_set				fd_read;
	fd_set				fd_write;
	char				*ptr;
}						t_server;

typedef struct			s_grp
{
	t_listing			*list;
	char				*ptr;
	char				mdr[2];
	char				c;
	t_client			*from;
	t_chan				*on;
	t_client			*to;
}						t_grp;

int						sv_parse_conf_file(t_server *server);
int						is_chan_member(t_chan *ch, t_client *cl);
int						is_modo(t_chan *chan, t_client *cl);
t_chan					*find_chan(char *name, t_chan *chans);
void					rpl_away(t_client *to, t_client *cl, t_server *server);
void					rpl_cmode(t_grp *grp, char *limit);
void					rpl_umode(t_grp *g, t_chan *c, t_client *to, t_client *cl);
void					rpl_motd(t_client *cl, t_server *server);
void					send_joinmsg_toothers(t_chan *chan, t_client *cl);
int						sv_accept_v4(t_server *server);
int						sv_accept_v6(t_server *server);
t_listing				*sv_add_chantouser(t_chan *chan, t_client *cl, t_server *server);
t_listing				*sv_add_usertochan(t_client *cl, t_chan *chan, t_server *server);
t_user					*sv_allowed(t_socket *inf, t_user *ptr);
void					sv_away(char **cmds, t_server *server, t_client *cl);
void					sv_channel_mode(char **cmds, t_chan *ch, t_client *cl, t_server *server);
void					sv_chan_user_mode(t_grp *grp, char ***cmd, t_server *server);
void					sv_check_clients(t_server *server);
int						sv_check_name_valid(char *name);
void					sv_cl_read(t_server *server, t_client *cl);
void					sv_cl_send(t_client *cl);
void					sv_cl_write(char *str, t_client *cl);
t_client				*sv_clear_client(t_client *cl, t_server *server);
void					sv_connect(char **cmds, t_server *server, t_client *cl);
int						sv_connect_client(t_client *cl, t_server *server);
void					sv_err(errnum_list_e errnum, t_client *client, ...);
void					sv_error(e_log_level level, char *str, t_server *server);
int						sv_log(e_log_level level, e_log_type type, const char *msg, ...);
void					sv_close_server(t_server *server);

void					sv_find_userinchan(char **cmd, t_chan *chan, t_client *cl);
void					sv_get_cl_password(t_client *cl, t_server *server);
int						sv_globcmp(const char *s1, const char *s2);
void					sv_help(char **cmds, t_server *server, t_client *cl);
int						sv_init_server(t_socket *v4, t_socket *v6, char *addr, char *port);
void					sv_join(char **cmds, t_server *server, t_client *cl);
void					sv_join_chan(char *name, char ***c, t_client *cl, t_server *server);
void					sv_leave(char **cmds, t_server *server, t_client *cl);
void					sv_list(char **cmds, t_server *server, t_client *cl);
int						sv_loop(t_server *server);
void					sv_mode(char **cmds, t_server *server, t_client *cl);
void					sv_msg(char **cmds, t_server *server, t_client *cl);
void					sv_msg_chan(char *chan_name, char **cmds, t_client *cl, t_server *server);
int						sv_new_client(t_socket *socket, t_client **clients);
void					sv_nick(char **cmds, t_server *server, t_client *cl);
void					sv_nick_change(t_client *cl, t_server *server);
void					sv_notice(char *str, t_client *cl, t_server *server);
void					sv_oper(char **cmds, t_server *server, t_client *cl);
void					sv_pass(char **cmds, t_server *server, t_client *cl);
void					sv_quit(char **cmds, t_server *server, t_client *cl);
void					sv_signals(void);
void					sv_sendto_chan(t_chan *chan, t_client *cl, t_server *server);
void					sv_sendto_chan_msg(char *msg, t_client *cl);
void					sv_sendto_chan_new(t_client *cl);
char					**sv_split(t_buf *buf);
int						sv_record_type_index(char c);
int						sv_tabcmp(char **t1, char **t2);
void					sv_topic(char **cmds, t_server *server, t_client *cl);
void					sv_user(char **cmds, t_server *server, t_client *cl);
void					sv_user_mode(char **cmds, t_client *cl);
void					sv_welcome(t_server *server, t_client *cl);
void					sv_who(char **cmds, t_server *server, t_client *cl);
void					sv_who_chan(char **cmds, t_client *cl, t_server *server);
void					sv_who_info(t_client *us, t_client *cl, t_server *server);

#endif
