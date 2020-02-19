/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 03:46:17 by calamber          #+#    #+#             */
/*   Updated: 2020/02/19 02:23:36 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc_deps.h"

t_malloc_data	g_data = { NULL, NULL, NULL, 0, 0, 0, 0, (t_malloc_stats){ 0, 0 } };
pthread_mutex_t	g_mutex = PTHREAD_MUTEX_INITIALIZER;

int			malloc_check_init(void)
{
	static int	init;
	int			curr;

	pthread_mutex_lock(&g_mutex);
	curr = init;
	init++;
	pthread_mutex_unlock(&g_mutex);
	return (curr);
}

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
	block = mmap(NULL, to_alloc, FT_PROT,
							FT_MAP, -1, 0);
	if ((void*)block == MAP_FAILED)
	{
		int errnum;

		errnum = errno;
		ft_printf_fd(1, "error: %s\n", strerror( errnum ));
		return (NULL);
	}
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
	pthread_mutex_lock(&g_mutex);
	ft_printf_fd(1, "INIT %lu %lu %u g_data %p\n", (unsigned long)getpid(), (unsigned long)getppid(), pthread_self(), &g_data);
	g_data.meta_size = sizeof(t_header);
	g_data.page_size = (size_t)getpagesize();
	g_data.debug_stats = (t_malloc_stats){ 0, 0 };
	g_data.large = NULL;
	g_data.tiny = request_space((TINY + g_data.meta_size) *
				MIN_ALLOC, TINY, TINY_FLAG, &g_data.tiny_amt);
	if (!g_data.tiny)
	{
		ft_printf_fd(1, "BIG ERROR\n");
		pthread_mutex_unlock(&g_mutex);
		return ;
	}
	g_data.small = request_space((SMALL + g_data.meta_size) *
				MIN_ALLOC, SMALL, SMALL_FLAG, &g_data.small_amt);
	if (!g_data.small)
	{
		munmap(g_data.tiny, g_data.tiny_amt * (TINY + g_data.meta_size));
		ft_printf_fd(1, "BIG ERROR\n");
	}
	//ft_printf_fd(1, "init g_data: %p tiny %p small %p large %p\n", g_data, g_data.tiny, g_data.small, g_data.large);
	//ft_printf_fd(1, "tiny sz %lu small sz %lu\n", g_data.tiny_amt, g_data.small_amt);
	ft_printf_fd(1, "INIT  g_data %p\n", &g_data);
	pthread_mutex_unlock(&g_mutex);
}
