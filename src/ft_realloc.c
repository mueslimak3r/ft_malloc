/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 01:08:58 by calamber          #+#    #+#             */
/*   Updated: 2020/02/15 14:39:19 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc_deps.h"

void		*ft_memcpy(void *dst, const void *src, size_t n)
{
	char		*d1;
	const char	*s1;

	d1 = dst;
	s1 = src;
	while (n--)
		*d1++ = *s1++;
	return (dst);
}

void		*ft_realloc(void *ptr, size_t size)
{
	t_header	*block_ptr;
	void		*new_ptr;

	ft_printf_fd(1, "REALLOC\n");
	if (size == 0)
		return (NULL);
	if (!ptr || !g_data)
		return (ft_malloc(size));
	block_ptr = (t_header*)ptr - 1;
	if ((void*)block_ptr >= g_data->heap_start && (void*)block_ptr < g_data->heap_end)
	{
		if (size <= block_ptr->size * g_data->meta_size)
			return (ptr);
		new_ptr = ft_malloc(size);
		if (!new_ptr)
			return (ptr);
		ft_memcpy(new_ptr, ptr, block_ptr->size * g_data->meta_size);
		ft_free(ptr);
		return (new_ptr);
	}
	return (ft_malloc(size));
}

void		*realloc(void *ptr, size_t size)
{
	return (ft_realloc(ptr, size));
}
