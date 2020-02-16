/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 13:54:11 by calamber          #+#    #+#             */
/*   Updated: 2020/02/15 13:54:13 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc_deps.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char *dst;

	dst = b;
	while (len--)
		*(dst++) = (unsigned char)c;
	return (b);
}

void	*ft_calloc(size_t count, size_t size)
{
	char	*new;
	int		new_size;

	new_size = size * count;
	new = ft_malloc(new_size);
	if (!new)
		return (NULL);
	ft_memset(new, 0, new_size);
	ft_printf_fd(1, "CALLOC END\n");
	return (new);
}

void	*calloc(size_t count, size_t size)
{
	ft_printf_fd(1, "CALLOC\n");
	return (ft_calloc(count, size));
}
