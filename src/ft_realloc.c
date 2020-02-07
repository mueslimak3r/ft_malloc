/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 01:08:58 by calamber          #+#    #+#             */
/*   Updated: 2020/02/07 01:08:59 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc_deps.h"

void		*ft_realloc(void *ptr, size_t size)
{
	t_header	*block_ptr;
	void		*new_ptr;

	if (!ptr || size == 0 || !g_initialized)
		return (NULL);

	block_ptr = (t_header*)ptr - 1;
	if (size <= block_ptr->size * g_data.meta_size)
		return (ptr);
	new_ptr = ft_malloc(size);
	if (!new_ptr)
		return (NULL);
	ft_memcpy(new_ptr, ptr, block_ptr->size * g_data.meta_size);
	ft_free(ptr);
	return (new_ptr);
}

void		*realloc(void *ptr, size_t size)
{
	return (ft_realloc(ptr, size));
}
