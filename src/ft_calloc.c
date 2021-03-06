/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 13:54:11 by calamber          #+#    #+#             */
/*   Updated: 2020/02/19 05:20:27 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc_deps.h"

static void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char *dst;

	dst = b;
	while (len)
	{
		*(dst) = (unsigned char)c;
		dst++;
		len--;
	}
	return (b);
}

void		*ft_calloc(size_t count, size_t size)
{
	char	*new;
	int		new_size;

	new_size = size * count;
	new = ft_malloc(new_size);
	if (!new)
		return (NULL);
	ft_memset(new, 0, new_size);
	return (new);
}

void		*calloc(size_t count, size_t size)
{
	return (ft_calloc(count, size));
}
