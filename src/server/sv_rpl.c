/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_rpl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/01 21:38:52 by gbourgeo          #+#    #+#             */
/*   Updated: 2017/04/11 08:36:01 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sv_main.h"

void			rpl_cmode(t_grp *g, char *limit)
{
	sv_cl_write(":", g->to);
	sv_cl_write((g->on->cmode & CHFL_ANON && g->to->socket.fd != g->from->socket.fd) ?
				"anonymous" : g->from->inf->nick, g->to);
	sv_cl_write("!~", g->to);
	sv_cl_write((g->on->cmode & CHFL_ANON && g->to->socket.fd != g->from->socket.fd) ?
				"anonymous" : g->from->inf->username, g->to);
	sv_cl_write("@", g->to);
	if (*g->from->socket.host)
		sv_cl_write((g->on->cmode & CHFL_ANON && g->to->socket.fd != g->from->socket.fd) ?
					"anonymous" : g->from->socket.host, g->to);
	else
		sv_cl_write((g->on->cmode & CHFL_ANON && g->to->socket.fd != g->from->socket.fd) ?
					"anonymous" : g->from->socket.addr, g->to);
	sv_cl_write(" MODE ", g->to);
	sv_cl_write(g->on->name, g->to);
	sv_cl_write((g->c) ? " +" : " -", g->to);
	sv_cl_write(g->mdr, g->to);
	if ((*g->ptr == 'l' || *g->ptr == 'k') && g->c)
	{
		sv_cl_write(" ", g->to);
		sv_cl_write((*g->ptr == 'l') ? limit : g->on->key, g->to);
	}
	sv_cl_write(END_CHECK, g->to);
	if (limit)
		free(limit);
}

void			rpl_umode(t_grp *g, t_chan *c, t_client *to, t_client *cl)
{
	sv_cl_write(":", to);
	sv_cl_write((c->cmode & CHFL_ANON) ? "anonymous" : cl->inf->nick, to);
	sv_cl_write("!~", to);
	sv_cl_write((c->cmode & CHFL_ANON) ? "anonymous" : cl->inf->username, to);
	sv_cl_write("@", to);
	if (*cl->socket.host)
		sv_cl_write((c->cmode & CHFL_ANON) ? "anonymous" : cl->socket.host, to);
	else
		sv_cl_write((c->cmode & CHFL_ANON) ? "anonymous" : cl->socket.addr, to);
	sv_cl_write(" MODE ", to);
	sv_cl_write(c->name, to);
	sv_cl_write((g->c) ? " :+" : " :-", to);
	sv_cl_write(g->ptr, to);
	sv_cl_write(" ", to);
	sv_cl_write(g->to->inf->nick, to);
	sv_cl_write(END_CHECK, to);
}
