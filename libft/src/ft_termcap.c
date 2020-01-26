/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_termcap.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 22:57:16 by alkozma           #+#    #+#             */
/*   Updated: 2019/11/21 22:57:47 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		ft_termcap(char *cmd, int fd)
{
	char	*temp;
	char	buf[30];

	temp = buf;
	if (!cmd || fd < 1)
		return ;
	ft_putstr_fd(tgetstr(cmd, &temp), fd);
}
