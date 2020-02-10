/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 02:48:37 by calamber          #+#    #+#             */
/*   Updated: 2020/02/10 02:48:38 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc_deps.h"

int			find_allocd_block(t_header *page_start, t_header **last)
{
	t_header	*tmp;
	t_header	*end;
	int			ret;

	ret = 0;
	*last = NULL;
	tmp = page_start;
	end = page_start + (g_data.page_size / g_data.meta_size);
	while (tmp && tmp >= page_start && tmp < end)
	{
		if (tmp->flags & IS_ALLOCD_FLAG)
			ret = 1;
		*last = tmp;
		tmp = tmp->next;
	}
	return (ret);
}

t_header	*get_page_start(t_header *block_ptr)
{
	unsigned long offset;

	if (block_ptr && (unsigned long)block_ptr >= g_data.page_size)
	{
		offset = (unsigned long)block_ptr % g_data.page_size;
		if (offset)
			offset /= g_data.meta_size;
		return (block_ptr - offset);
	}
	return (NULL);
}

int			check_unmap(t_header *page_start, unsigned long flags)
{
	t_header		*last;
	t_header		*next;
	t_header		*prev;
	unsigned long	allocs_per_page;

	allocs_per_page = g_data.page_size /
		((flags & TINY_FLAG ? TINY : SMALL) + g_data.meta_size);
	if (!page_start || !(flags & (TINY_FLAG | SMALL_FLAG)))
		return (0);
	if ((flags & TINY_FLAG && g_data.tiny_amt - allocs_per_page < MIN_ALLOC) ||
	(flags & SMALL_FLAG && g_data.small_amt - allocs_per_page < MIN_ALLOC) ||
		(find_allocd_block(page_start, &last)))
		return (0);
	next = last && last->next && (last->next < page_start || last->next >=
		g_data.page_size / g_data.meta_size + page_start) ? last->next : NULL;
	prev = page_start->prev;
	(flags & TINY_FLAG) ? (g_data.tiny_amt -= allocs_per_page) : 0;
	(flags & SMALL_FLAG) ? (g_data.small_amt -= allocs_per_page) : 0;
	(prev) ? (prev->next = next) : 0;
	(next) ? (next->prev = prev) : 0;
	(page_start == g_data.tiny) ? (g_data.tiny = next) : 0;
	(page_start == g_data.small) ? (g_data.small = next) : 0;
	g_data.debug_stats.bytes_unmapped += g_data.page_size;
	munmap(page_start, g_data.page_size);
	return (1);
}

void		ft_free(void *ptr)
{
	t_header		*block_ptr;

	if (ptr == NULL || !g_initialized)
		return ;
	block_ptr = ((t_header*)ptr) - 1;
	if (!block_ptr)
		return ;
	block_ptr->flags ^= 0x1;
	if (block_ptr->flags & 0x8)
	{
		if (block_ptr->prev)
			block_ptr->prev->next = block_ptr->next;
		if (block_ptr->next)
			block_ptr->next->prev = block_ptr->prev;
		if (g_data.large == block_ptr)
			g_data.large = g_data.large->next;
		g_data.debug_stats.bytes_unmapped += block_ptr->size *
						g_data.meta_size + g_data.meta_size;
		munmap(block_ptr, block_ptr->size *
						g_data.meta_size + g_data.meta_size);
	}
	else
		check_unmap(get_page_start(block_ptr), block_ptr->flags);
}

void		free(void *ptr)
{
	ft_free(ptr);
}
