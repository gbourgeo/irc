/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/17 01:24:24 by gbourgeo          #+#    #+#             */
/*   Updated: 2023/01/01 02:26:13 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_H
# define COMMANDS_H

/**
 * @brief Structure to handle the server commands
 * 
 */
typedef struct			s_command
{
	char				*name;		/** @brief Name of the command */
	void				(*fct)();	/** @brief Commands' function to be called */
	int					privilege;	/** @brief Privilege needed to access the command*/
}						t_command;

#endif
