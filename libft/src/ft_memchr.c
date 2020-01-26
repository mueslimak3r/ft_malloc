/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/28 22:35:04 by calamber          #+#    #+#             */
/*   Updated: 2018/04/28 22:36:54 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*s1;

	s1 = (const unsigned char *)s;
	while (n-- > 0)
	{
		if (*s1 == (unsigned char)c)
			return ((void *)s1);
		s1++;
	}
	return (NULL);
}
