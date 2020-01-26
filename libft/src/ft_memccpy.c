/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/28 22:04:58 by calamber          #+#    #+#             */
/*   Updated: 2018/04/28 22:13:24 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	size_t p;

	p = 0;
	while (p < n)
	{
		((char*)dst)[p] = ((char*)src)[p];
		if (((char*)dst)[p] == (char)c)
		{
			return (&((char*)dst)[p + 1]);
		}
		p++;
	}
	return (0);
}
