/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/07 05:20:47 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/11/27 16:27:35 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONF_H
# define CONF_H

# include <netdb.h>
# include "common.h"

# define CONF_FILE "irc.conf"

/**
 * @brief Enumeration of M lines
 *
 */
enum
{
	M_LINE_MODE = 0,
	M_LINE_SERVER_NAME,
	M_LINE_SERVER_IP,
	M_LINE_SERVER_LOCATION,
	M_LINE_SERVER_PORT,
	M_LINE_SERVER_ID,
	M_LINE_MIN_ARGS,
};

/**
 * @brief Enumeration of I lines
 *
 */
enum
{
	I_LINE_MODE = 0,
	I_LINE_HOST_ADDR,
	I_LINE_PASSWORD,
	I_LINE_HOST_NAME,
	I_LINE_PORT,
	I_LINE_CLASS,
	I_LINE_FLAGS,
	I_LINE_MIN_ARGS,
};

/**
 * @brief Enumeration of O lines
 *
 */
enum
{
	O_LINE_MODE = 0,
	O_LINE_HOST_NAME,
	O_LINE_PASSWORD,
	O_LINE_NICKNAME,
	O_LINE_PORT,
	O_LINE_CLASS,
	O_LINE_MIN_ARGS,
};

typedef struct	s_conf_line
{
	char		mode;
	int			(*handler)(char **, void *);
	int			min_args;
}				t_conf_line;

/**
 * @brief Structure holding informations about a user
 * defined in the irc configuration file.
 */
typedef struct		s_user
{
	char			mode;					/* User Privileges */
	char			hostaddr[ADDR_LEN];		/* User Host name */
	char			*passwd;				/* User Password */
	char			hostname[NI_MAXHOST];	/* User Real Name */
	char			nick[NICK_LEN];			/* User Nickname */
	char			port[6];				/* User */
	int				class;					/* User */
	struct s_user	*next;
}					t_user;

typedef struct		s_conf
{
	const char		*conf_file;		/* Configuration file path */
	t_user			*allowed_user;	/* List of allowed users.    In conf file, starts by I: and have an empty password field */
	t_user			*pass_user;		/* List of restricted users. In conf file, starts by I: and have an non-empty password field */
	t_user			*opers;			/* List of operator users.   In conf file, starts by O: */
}					t_conf;

#endif
