/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 01:13:01 by calamber          #+#    #+#             */
/*   Updated: 2020/02/15 14:39:42 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc_deps.h"

t_header	*find_free_block(t_header **last, size_t size)
{
	t_header *current;

	if (!*last)
		return (NULL);
	current = *last;
	while (current && !(!(current->flags & IS_ALLOCD_FLAG)
				&& current->size * g_data->meta_size >= size))
	{
		*last = current;
		current = current->next;
	}
	return (current);
}

void		get_type(size_t *flags, size_t *block_size,
							t_header **last, size_t size)
{
	if (size <= TINY)
	{
		*flags = TINY_FLAG;
		*block_size = TINY;
		*last = g_data->tiny;
	}
	else if (size > TINY && size <= SMALL)
	{
		*flags = SMALL_FLAG;
		*block_size = SMALL;
		*last = g_data->small;
	}
	else
	{
		*flags = LARGE_FLAG;
		*last = g_data->large;
		*block_size = size;
	}
}

void		join_new_block(t_header *new, t_header *last, size_t flags)
{
	new->prev = NULL;
	if (last)
	{
		new->prev = last;
		last->next = new;
	}
	else if (flags == TINY_FLAG)
		g_data->tiny = new;
	else if (flags == SMALL_FLAG)
		g_data->small = new;
	else if (flags == LARGE_FLAG)
		g_data->large = new;
}

void		*ft_malloc(size_t size)
{
	t_header		*block;
	t_header		*last;
	size_t			block_size;
	size_t			flags;
	static int		init;

	//ft_printf_fd(1, "MALLOC\n");
	if (!init++)
		ft_malloc_init();
	if (size == 0)
		return (NULL);
	pthread_mutex_lock(&g_mutex);
	size += size % g_data->meta_size;
	get_type(&flags, &block_size, &last, size);
	block = find_free_block(&last, size);
	if (!block)
	{
		if (flags == LARGE_FLAG)
			block = request_space(size + g_data->meta_size, 1, flags, NULL);
		else
			block = request_space((block_size + g_data->meta_size) * MIN_ALLOC,
block_size, flags, (flags == TINY_FLAG ? &g_data->tiny_amt : &g_data->small_amt));
		if (!block)
		{
			pthread_mutex_unlock(&g_mutex);
			return (NULL);
		}
		join_new_block(block, last, flags);
	}
	(block) ? (block->flags |= IS_ALLOCD_FLAG) : 0;
	//ft_printf_fd(1, "MALLOC END\n");
	pthread_mutex_unlock(&g_mutex);
	return (block + 1);
}

void		*malloc(size_t size)
{
	//ft_printf_fd(1, "MALLOC\n");
	return (ft_malloc(size));
}
