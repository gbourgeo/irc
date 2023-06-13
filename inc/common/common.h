/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/14 11:25:20 by gbourgeo          #+#    #+#             */
/*   Updated: 2023/06/13 12:28:33 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_H
# define COMMON_H

# include <sys/types.h>
# include "libft.h"

/*
** The size of this server / client Ringbuffer on read and write.
*/
# define BUFF		256

/*
** The maximum lenght a nickname can be.
*/
# define NICK_LEN	9

/*
** ADDR_LEN		The length we can store a client address.
**				Be aware the server handle ipv6.
*/
# define ADDR_LEN	INET6_ADDRSTRLEN

/*
** Default port number
*/
# define DEF_PORT	"6667"

# define END_CHECK "\r\n"
# define END_CHECK_LEN 2

# define ISSPECIAL(c) (((c) >= '[' && (c) <= '`') || ((c) >= '{' && (c) <= '}'))

/*
** Structure s_buf implemented for client write and server read / write
** circular buffer.
*/

typedef struct	s_buf
{
	char	buffer[BUFF];
	char	*start;
	char	*end;
	char	*head;
	char	*tail;
	int		len;
}				t_buf;

void		ft_free(char ***cmds);
char		*ft_strtoupper(char *str);
char		*ft_strtolower(char *str);
int			ft_strisalnum(char *str);
int			sv_strcmp(const char *s1, const char *s2);
int			sv_strncmp(const char *s1, const char *s2, size_t n);
char		**sv_strsplit(char *str, char c);
void		ft_init_ringbuf(t_buf *buffer);
void		ft_move_head(ssize_t len, t_buf *buf);
void		ft_move_tail(ssize_t len, t_buf *buf);
void		ft_move_tail_forward(ssize_t len, t_buf *buf, char **ptr);
void		ft_move_tail_backward(ssize_t len, t_buf *buf, char **ptr);

#endif
