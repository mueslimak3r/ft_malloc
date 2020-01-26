/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/04 00:19:01 by calamber          #+#    #+#             */
/*   Updated: 2018/05/04 00:20:20 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strsub(char const *s, unsigned int start, size_t len)
{
	char*str;

	if (!s)
		return (NULL);
	if (!(str = ft_memalloc(sizeof(char) * (len + 1))))
		return (0);
	while (start--)
	{
		s++;
		if (!(*s))
			return (0);
	}
	ft_strncpy(str, s, len);
	return (str);
}
