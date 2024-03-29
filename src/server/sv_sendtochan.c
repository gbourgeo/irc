/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_sendtochan.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/17 19:48:00 by gbourgeo          #+#    #+#             */
/*   Updated: 2017/03/14 18:23:42 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sv_main.h"
#include <sys/socket.h>

void			sv_sendto_chan_new(t_client *cl)
{
	(void)cl;
}

void			sv_sendto_chan_msg(char *msg, t_client *cl)
{
	(void)msg;
	(void)cl;
}

void			sv_sendto_chan(t_chan *chan, t_client *cl, t_server *e)
{
	(void)chan;
	(void)cl;
	(void)e;
}
