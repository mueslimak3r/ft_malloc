/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readstdin_line.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/26 02:41:12 by calamber          #+#    #+#             */
/*   Updated: 2019/03/26 02:42:14 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_readstdin_line(void)
{
	char	buf[BUFF_SIZE + 1];
	char	*tmp;
	char	*s;
	int		ret;

	s = 0;
	while ((ret = read(0, buf, BUFF_SIZE)) > 0)
	{
		buf[ret] = '\0';
		tmp = s ? ft_strjoin(s, buf) : ft_strdup(buf);
		free(s);
		s = tmp;
		if (ft_strchr(s, '\n'))
		{
			tmp = ft_strndup(s, (ft_strchr(s, '\n') - s));
			free(s);
			return (tmp);
		}
	}
	return (0);
}
