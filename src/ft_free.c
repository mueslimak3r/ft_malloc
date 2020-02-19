/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 02:48:37 by calamber          #+#    #+#             */
/*   Updated: 2020/02/19 05:43:09 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc_deps.h"
#include "ft_malloc.h"

static int			find_allocd_block(t_header *page_start, t_header **last)
{
	t_header	*tmp;
	t_header	*end;
	int			ret;

	ret = 0;
	*last = NULL;
	if (!page_start)
		return (0);
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

static t_header		*get_page_start(t_header *block_ptr)
{
	unsigned long offset;
	/*
	unsigned long offset;

	if (block_ptr && (uintptr_t)block_ptr >= g_data.page_size)
	{
		offset = (uintptr_t)block_ptr % g_data.page_size;
		//if (offset)
		//	offset /= g_data.meta_size;
		return ((t_header*)((uintptr_t)block_ptr - offset));
	}
	return (NULL);
	*/
	if (block_ptr)
	{
		offset = (uintptr_t)block_ptr % g_data.page_size;
		return ((t_header*)((uintptr_t)block_ptr - offset));
	}
	return (NULL);
}

static int			check_unmap(t_header *page_start, unsigned long flags)
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
	next = last && last->next ? last->next : NULL;//(last->next < page_start || last->next >=
		//g_data.page_size / g_data.meta_size + page_start) ? last->next : NULL;
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

static int			find_in_bucket(t_header *block_ptr, t_header *bucket)
{
	while (bucket)
	{
		if (bucket == block_ptr)
			return (1);
		bucket = bucket->next;
	}
	return (0);
}

int					malloc_check_if_valid(t_header *block_ptr)
{
	int	ret;

	ret = 0;
	if (!malloc_check_init())
		return (0);
	pthread_mutex_lock(&g_mutex);
	if ((find_in_bucket(block_ptr, g_data.tiny)) ||
	(find_in_bucket(block_ptr, g_data.small)) ||
	(find_in_bucket(block_ptr, g_data.large)))
		ret = 1;
	pthread_mutex_unlock(&g_mutex);
	return (ret);
}

void				ft_free(void *ptr)
{
	t_header		*block_ptr;

	//ft_printf_fd(1, "free\n");
	if (ptr == NULL || !malloc_check_init())
	{
		//if (ptr != NULL)
		//	ft_printf_fd(1, "not init\n", ptr, g_data);
		return ;
	}
	block_ptr = ((t_header*)ptr) - 1;
	if (!block_ptr || !(malloc_check_if_valid(block_ptr)))
	{
		//ft_printf_fd(1, "err bad block\n");
		return ;
	}
	pthread_mutex_lock(&g_mutex);
	block_ptr->flags ^= 0x1;
	if (block_ptr->flags & 0x8)
	{
		if (block_ptr->prev)
			block_ptr->prev->next = block_ptr->next;
		if (block_ptr->next)
			block_ptr->next->prev = block_ptr->prev;
		if (g_data.large == block_ptr)
			g_data.large = g_data.large->next;
		g_data.debug_stats.bytes_unmapped += (block_ptr->size *
						g_data.meta_size) + g_data.meta_size;
		munmap(block_ptr, (block_ptr->size * g_data.meta_size) + g_data.meta_size);
	}
	else
		check_unmap(get_page_start(block_ptr), block_ptr->flags);
	pthread_mutex_unlock(&g_mutex);
}

void				free(void *ptr)
{
	ft_free(ptr);
	//ft_printf_fd(1, "finished free\n");
}
