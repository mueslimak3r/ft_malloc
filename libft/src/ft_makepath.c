/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_makepath.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/24 10:24:05 by calamber          #+#    #+#             */
/*   Updated: 2019/03/24 10:24:07 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char				*ft_makepath(char *s1, char *s2, char c)
{
	char			*str;
	char			*ret;

	if (!s1 || !s2)
		return (0);
	str = (char *)ft_memalloc(sizeof(char) *
		(ft_strlen(s1) + ft_strlen(s2) + 2));
	if (str == NULL)
		return (NULL);
	ret = str;
	while (*s1)
		*(str++) = *(s1++);
	if (*(s1 - 1) != c)
		*(str++) = c;
	while (*s2)
		*(str++) = *(s2++);
	return (ret);
}
