/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 01:13:01 by calamber          #+#    #+#             */
/*   Updated: 2020/02/07 01:13:04 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc_deps.h"

void		init_blocks(t_header *block, size_t block_size, size_t block_amt, unsigned long flags)
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

t_header    *request_space(size_t size, size_t units, unsigned long flags, unsigned long *amt)
{
	size_t		to_alloc;
	size_t		units_allocd;
	t_header	*block;

	to_alloc = size > g_data.page_size ? (g_data.page_size * ((size / g_data.page_size) + 1)) : g_data.page_size;
	block = mmap(NULL, to_alloc, (PROT_READ | PROT_WRITE), (MAP_PRIVATE | MAP_ANONYMOUS), -1, 0);
	if ((void*)block == MAP_FAILED)
		return (NULL);
	block->next = NULL;
	block->prev = NULL;
	block->size = (to_alloc / g_data.meta_size) - 1;
	block->flags = flags;
	if (!(flags & 0x8))
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
	pthread_mutex_init(&g_mutex_lock, NULL);
	pthread_mutex_lock(&g_mutex_lock);
	g_data.meta_size = sizeof(t_header);
	g_data.page_size = (size_t)getpagesize();
	g_data.tiny = request_space(MIN_ALLOC * (TINY + g_data.meta_size), TINY, 0x2, &g_data.tiny_amt);
	if (!g_data.tiny)
	{
		pthread_mutex_unlock(&g_mutex_lock);
		return ;
	}
	g_data.small = request_space(MIN_ALLOC * (SMALL + g_data.meta_size), SMALL, 0x4, &g_data.small_amt);
	if (!g_data.small)
	{
		pthread_mutex_unlock(&g_mutex_lock);
		return ;
	}
	g_data.large = NULL;
	g_initialized = true;
	pthread_mutex_unlock(&g_mutex_lock);
}

t_header    *find_free_block(t_header **last, size_t size)
{
	t_header *current;

	current = last ? *last : NULL;
	while (current && !(!(current->flags & 0x1) && current->size >= size / g_data.meta_size))
	{
		*last = current;
		current = current->next;
	}
	return (current);
}

void		get_type(size_t *flags, size_t *block_size, t_header **last, size_t size)
{
	if (size <= TINY)
	{
		*flags = 0x2;
		*block_size = TINY;
		*last = g_data.tiny;
	}
	else if (size > TINY && size <= SMALL)
	{
		*flags = 0x4;
		*block_size = SMALL;
		*last = g_data.small;
	}
	else
	{
		*flags = 0x8;
		*last = g_data.large;
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
	else if (flags == 0x8)
		g_data.large = new;
}

void        *ft_malloc(size_t size)
{
	t_header		*block;
	t_header		*last;
	size_t			block_size;
	size_t			flags;

	if (!g_initialized)
		ft_malloc_init();
	//ft_putstr_fd("MALLOC CALLED\n", 1);
	if (size == 0)
		return NULL;
	pthread_mutex_lock(&g_mutex_lock);
	size = g_data.meta_size * (size / g_data.meta_size) + (size % g_data.meta_size ? g_data.meta_size : 0);
	get_type(&flags, &block_size, &last, size);
	block = find_free_block(&last, size);
	if (!block)
	{
		if (flags == 0x8)
			block = request_space(size + g_data.meta_size, 1, 0x8, NULL);
		else
			block = request_space(MIN_ALLOC * (block_size + g_data.meta_size),
			block_size, flags, (flags == 0x2 ? &g_data.tiny_amt : &g_data.small_amt));
		if (!block)
		{
			pthread_mutex_unlock(&g_mutex_lock);
			return (NULL);
		}
		join_new_block(block, last, flags);
	}
	if (block)
		block->flags |= 0x1;
	pthread_mutex_unlock(&g_mutex_lock);
	return(block + 1);
}

void        *malloc(size_t size)
{
	return (ft_malloc(size));
}
