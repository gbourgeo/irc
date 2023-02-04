/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err_list.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/17 01:15:21 by gbourgeo          #+#    #+#             */
/*   Updated: 2023/01/01 01:41:01 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERR_LIST_H
# define ERR_LIST_H

/*
** Errors list
*/
typedef enum errnum_list
{
	ERR_NOT_DEFINED = 0,
	ERR_NOSUCHNICK = 401,
	ERR_NOSUCHSERVER = 402,
	ERR_NOSUCHCHANNEL = 403,
	ERR_CANNOTSENDTOCHAN = 404,
	ERR_TOOMANYCHANNELS = 405,
	ERR_WASNOSUCHNICK = 406,
	ERR_TOOMANYTARGETS = 407,
	ERR_NOSUCHSERVICE = 408,
	ERR_NOORIGIN = 409,
	ERR_NORECIPIENT = 411,
	ERR_NOTEXTTOSEND = 412,
	ERR_NOTOPLEVEL = 413,
	ERR_WILDTOPLEVEL = 414,
	ERR_BADMASK = 415,
	ERR_UNKNOWNCOMMAND = 421,
	ERR_NOMOTD = 422,
	ERR_NOADMININFO = 423,
	ERR_FILEERROR = 424,
	ERR_CREATECHANNELFAIL = 426,
	ERR_NONICKNAMEGIVEN = 431,
	ERR_ERRONEUSNICKNAME = 432,
	ERR_NICKNAMEINUSE = 433,
	ERR_NICKCOLLISION = 436,
	ERR_UNAVAILABLERESOURCE = 437,
	ERR_USERNOTINCHANNEL = 441,
	ERR_NOTONCHANNEL = 442,
	ERR_USERONCHANNEL = 443,
	ERR_NOLOGIN = 444,
	ERR_SUMMONDISABLED = 445,
	ERR_USERDISABLED = 446,
	ERR_NOTREGISTERED = 451,
	ERR_NEEDMOREPARAMS = 461,
	ERR_ALREADYREGISTERED = 462,
	ERR_NOPERMFORHOST = 463,
	ERR_PASSWDMISMATCH = 464,
	ERR_YOUREBANNEDCREEP = 465,
	ERR_YOUWILLBEBANNED = 466,
	ERR_KEYSET = 467,
	ERR_CHANNELISFULL = 471,
	ERR_UNKNOWNMODE = 472,
	ERR_INVITEONLYCHAN = 473,
	ERR_BANNEDFROMCHAN = 474,
	ERR_BADCHANNELKEY = 475,
	ERR_BADCHANMASK = 476,
	ERR_NOCHANMODES = 477,
	ERR_BANLISTFULL = 478,
	ERR_ILLEGALNAME = 479,
	ERR_NOPRIVILEGES = 481,
	ERR_CHANOPRIVSNEEDED = 482,
	ERR_CANTKILLSERVER = 483,
	ERR_RESTRICTED = 484,
	ERR_UNIQOPPRIVSNEEDED = 485,
	ERR_NOOPERHOST = 491,
	ERR_UMODEUNKNOWNFLAG = 501,
	ERR_USERSDONTMATCH = 502,
	ERR_USERSMODE = 503,
}	errnum_list_e;

/*
** define for sv_err(..) function
*/
typedef struct	err_list_s
{
	errnum_list_e	errnum;
	char			*errstr;
}				err_list_t;

#endif
