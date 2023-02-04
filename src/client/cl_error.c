/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/16 05:12:01 by gbourgeo          #+#    #+#             */
/*   Updated: 2023/01/01 23:56:22 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <unistd.h>
#include <string.h>
#include "ft_dprintf.h"
#include "cl_main.h"

void		cl_error(const char *err, t_client *client)
{
	client->stop = 1;
	ft_dprintf(STDERR_FILENO, "\e[31m**ERROR**\e[0m :%s", err);
	if (errno != 0)
		ft_dprintf(STDERR_FILENO, " :%s\n", strerror(errno));
	ft_dprintf(STDERR_FILENO, "\n");
	errno = 0;
}
