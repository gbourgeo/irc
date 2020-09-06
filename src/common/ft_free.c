/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/12 20:52:42 by gbourgeo          #+#    #+#             */
/*   Updated: 2016/06/14 07:47:47 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void			ft_free(char ***cmds)
{
	int			i;

	i = 0;
	if (!cmds || !*cmds)
		return ;
	while ((*cmds)[i] != NULL)
		free((*cmds)[i++]);
	free(*cmds);
}
