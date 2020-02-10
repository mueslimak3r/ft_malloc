/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 03:46:17 by calamber          #+#    #+#             */
/*   Updated: 2020/02/10 03:46:18 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc_deps.h"

void		init_blocks(t_header *block, size_t block_size,
						size_t block_amt, unsigned long flags)
{
	t_header		*last;
	unsigned long	size_in_blocks;

	last = NULL;
	if (!block)
		return ;
	size_in_blocks = block_size / g_data.meta_size;
	while (block_amt > 0)
	{
		block->next = NULL;
		block->prev = last;
		if (last)
			last->next = block;
		block->flags = flags;
		block->size = size_in_blocks;
		last = block;
		block += size_in_blocks + 1;
		block_amt--;
	}
}

t_header	*request_space(size_t size, size_t units,
					unsigned long flags, unsigned long *amt)
{
	size_t		to_alloc;
	size_t		units_allocd;
	t_header	*block;

	to_alloc = size > g_data.page_size ? (g_data.page_size *
			((size / g_data.page_size) + 1)) : g_data.page_size;
	block = mmap(NULL, to_alloc, (PROT_READ | PROT_WRITE),
							(MAP_PRIVATE | MAP_ANONYMOUS), -1, 0);
	if ((void*)block == MAP_FAILED)
		return (NULL);
	g_data.debug_stats.bytes_mapped += to_alloc;
	block->next = NULL;
	block->prev = NULL;
	block->size = (to_alloc / g_data.meta_size) - 1;
	block->flags = flags;
	if (!(flags & LARGE_FLAG))
	{
		units_allocd = (block->size + 1) / ((units / g_data.meta_size) + 1);
		if (amt)
			*amt += units_allocd;
		init_blocks(block, units, units_allocd, flags);
	}
	return (block);
}

void		ft_malloc_init(void)
{
	g_data.meta_size = sizeof(t_header);
	g_data.page_size = (size_t)getpagesize();
	g_data.debug_stats = (t_malloc_stats){ 0, 0 };
	g_data.large = NULL;
	g_data.tiny = request_space((TINY + g_data.meta_size) *
				MIN_ALLOC, TINY, TINY_FLAG, &g_data.tiny_amt);
	if (!g_data.tiny)
		return ;
	g_data.small = request_space((SMALL + g_data.meta_size) *
				MIN_ALLOC, SMALL, SMALL_FLAG, &g_data.small_amt);
	if (!g_data.small)
	{
		g_data.debug_stats.bytes_unmapped +=
					g_data.tiny_amt * (TINY + g_data.meta_size);
		munmap(g_data.tiny, g_data.tiny_amt * (TINY + g_data.meta_size));
		exit(1);
	}
	g_initialized = true;
}
