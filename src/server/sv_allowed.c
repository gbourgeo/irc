/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sv_allowed.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/11 08:11:58 by gbourgeo          #+#    #+#             */
/*   Updated: 2022/10/31 21:04:53 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sv_main.h"

/**
 * @brief This function compare a socket informations to an allowed users list.
 * It first checks the port, given or not,
 * then compares the hostname. If it doesn't match, it compares the hostaddr.
 * 
 * @param socket Client socket information
 * @param user List of allowed users
 * @return t_user* Pointer to an allowed user
 */
t_user			*sv_allowed(t_socket *socket, t_user *user)
{
	while (user)
	{
		if (!*user->port
		|| ft_atoi(user->port) == 0
		|| ft_strcmp(user->port, socket->port) == 0)
		{
			if (ft_strchr(user->hostname, '*'))
			{
				if (!sv_globcmp(user->hostname, socket->host))
					break ;
			}
			else if (!ft_strcmp(user->hostname, socket->host))
				break ;
			if (ft_strchr(user->hostaddr, '*'))
			{
				if (!sv_globcmp(user->hostaddr, socket->addr))
					break ;
			}
			else if (!ft_strcmp(user->hostaddr, socket->addr))
				break ;
		}
		user = user->next;
	}
	return (user);
}
