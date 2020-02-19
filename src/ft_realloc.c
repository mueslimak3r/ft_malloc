/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 01:08:58 by calamber          #+#    #+#             */
/*   Updated: 2020/02/19 01:48:11 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc_deps.h"

static void		*ft_memcpy(void *dst, const void *src, size_t n)
{
	char		*d1;
	const char	*s1;

	d1 = dst;
	s1 = src;
	while (n--)
		*d1++ = *s1++;
	return (dst);
}

void			*ft_realloc(void *ptr, size_t size)
{
	t_header	*block_ptr;
	void		*new_ptr;

	if (size == 0)
		return (NULL);
	if (!ptr)
		return (ft_malloc(size));
	if (!malloc_check_init() || !malloc_check_if_valid((t_header*)ptr - 1))
		return (NULL);
	pthread_mutex_lock(&g_mutex);
	block_ptr = (t_header*)ptr - 1;
	if (size <= block_ptr->size * g_data.meta_size)
	{
		pthread_mutex_unlock(&g_mutex);
		return (ptr);
	}
	else if (block_ptr->size * g_data.meta_size < size)
	{
		pthread_mutex_unlock(&g_mutex);
		new_ptr = ft_malloc(size);
		if (!new_ptr)
			return (ptr);
		pthread_mutex_lock(&g_mutex);
		ft_memcpy(new_ptr, ptr, block_ptr->size * g_data.meta_size);
		pthread_mutex_unlock(&g_mutex);
		ft_free(ptr);
		return (new_ptr);
	}
	return (NULL);
}

void			*realloc(void *ptr, size_t size)
{
	//ft_printf_fd(1, "REALLOC\n");
	return (ft_realloc(ptr, size));
}
