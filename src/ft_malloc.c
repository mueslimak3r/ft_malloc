/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 01:13:01 by calamber          #+#    #+#             */
/*   Updated: 2020/02/19 05:42:57 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc_deps.h"

static t_header	*first_fit(t_header **last, size_t size)
{
	t_header *current;

	if (!*last)
		return (NULL);
	current = *last;
	while (current && !(!(current->flags & IS_ALLOCD_FLAG)
				&& current->size * g_data.meta_size >= size))
	{
		*last = current;
		current = current->next;
	}
	return (current);
}

static t_header	*join_new_block(t_header *new, t_header *last, size_t flags)
{
	if (!new)
		return (NULL);
	new->prev = NULL;
	if (last)
	{
		new->prev = last;
		last->next = new;
	}
	else if (flags == TINY_FLAG)
		g_data.tiny = new;
	else if (flags == SMALL_FLAG)
		g_data.small = new;
	else if (flags == LARGE_FLAG)
		g_data.large = new;
	return (new);
}

static t_header	*get_more(size_t size, size_t block_size, size_t flags)
{
	t_header	*block;

	if (flags == LARGE_FLAG)
		block = request_space(size + g_data.meta_size, 1, flags, NULL);
	else
		block = request_space((block_size + g_data.meta_size) * MIN_ALLOC,
									block_size, flags, (flags == TINY_FLAG ?
									&g_data.tiny_amt : &g_data.small_amt));
	return (block);
}

static t_header	*find_block(size_t size)
{
	t_header		*block;
	t_header		*last;
	size_t			block_size;
	size_t			flags;

	if (size <= SMALL)
	{
		flags = (size <= TINY) ? TINY_FLAG : SMALL_FLAG;
		block_size = (size <= TINY) ? TINY : SMALL;
		last = (size <= TINY) ? g_data.tiny : g_data.small;
	}
	else
	{
		flags = LARGE_FLAG;
		last = g_data.large;
		block_size = size;
	}
	if (!(block = first_fit(&last, size)))
		return (join_new_block(get_more(size, block_size, flags), last, flags));
	return (block);
}

void			*ft_malloc(size_t size)
{
	t_header		*block;

	if (!malloc_check_init())
		ft_malloc_init();
	if (size == 0)
		return (NULL);
	size += (size % g_data.meta_size) ?
			g_data.meta_size - (size % g_data.meta_size) : 0;
	block = find_block(size);
	if (!block)
		return (NULL);
	block->flags |= IS_ALLOCD_FLAG;
	return (block + 1);
}
