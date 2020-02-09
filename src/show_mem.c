/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_mem.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 01:09:13 by calamber          #+#    #+#             */
/*   Updated: 2020/02/07 01:09:14 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc_deps.h"

unsigned long				get_blk_data(t_header *cur, char *name)
{
	unsigned long			freed;
	unsigned long			allocd;

	freed = 0;
	allocd = 0;
	while (cur)
	{
		if (cur->flags & IS_ALLOCD_FLAG)
			allocd += (cur->flags & LARGE_FLAG ? cur->size * g_data.meta_size + g_data.meta_size : 1);
		else
			freed += (cur->flags & LARGE_FLAG ? cur->size * g_data.meta_size + g_data.meta_size : 1);
		cur = cur->next;
	}
	printf("%s data total: %lu freed: %lu allocd: %lu\n", name, allocd + freed, freed, allocd);
	return (allocd + freed);
}

void		count_blocks()
{
	unsigned long tiny_size;
	unsigned long small_size;
	unsigned long large_size;

	printf("\e[1;32m");
	tiny_size = get_blk_data(g_data.tiny, "tiny") * (TINY + g_data.meta_size);
	small_size = get_blk_data(g_data.small, "small") * (SMALL + g_data.meta_size);
	large_size = get_blk_data(g_data.large, "large");

	printf("total mapped bytes: %lu\n", g_data.debug_stats.bytes_mapped);
	printf("tiny amt: ** logged %lu just found %lu **\nsmall amt: ** logged %lu just found %lu **\n",
		g_data.tiny_amt * (TINY + g_data.meta_size), tiny_size, g_data.small_amt * (SMALL + g_data.meta_size), small_size);

	unsigned long expected_used =
		(g_data.tiny_amt * (TINY + g_data.meta_size)) + (g_data.small_amt * (SMALL + g_data.meta_size)) + large_size;
	unsigned long actual_used = tiny_size + small_size + large_size;

	printf("expected bytes in use: %lu\n", expected_used);
	printf("actual bytes in use: %lu\n", actual_used);
	printf("total unmapped bytes: %lu\n", g_data.debug_stats.bytes_unmapped);

	printf("lost bytes: %lu\n", g_data.debug_stats.bytes_mapped - g_data.debug_stats.bytes_unmapped - actual_used);
	printf("\e[0m");
}

void		print_mem(char *name, t_header *cur, int freed)
{
	printf("\n%s\n", name);
	while (cur)
	{
		if ((freed && !(cur->flags & IS_ALLOCD_FLAG)) || (!freed && cur->flags & IS_ALLOCD_FLAG))
		{
			unsigned int type = cur->flags >> 1;
			printf("addr: %p | bytes: %lu | type: %u | allocd: %lu|\n", cur, cur->size * g_data.meta_size + g_data.meta_size, type, cur->flags & 0x1);
		}
		cur = cur->next;
	}
}

void		show_alloc_mem()
{
	if (!g_initialized)
		ft_malloc_init();
	printf("\e[1;35m*****************\nALLOCD MEMORY:");
	print_mem("TINY:", g_data.tiny, 0);
	print_mem("SMALL:", g_data.small, 0);
	print_mem("LARGE:", g_data.large, 0);
	printf("*****************\e[0m\n");
}

void		show_free_mem()
{
	if (!g_initialized)
		ft_malloc_init();
	printf("\e[1;34m*****************\nFREE MEMORY:");
	print_mem("TINY:", g_data.tiny, 1);
	print_mem("SMALL:", g_data.small, 1);
	print_mem("LARGE:", g_data.large, 1);
	printf("*****************\e[0m\n");
}
