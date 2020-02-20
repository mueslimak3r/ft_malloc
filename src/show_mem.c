/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_mem.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 01:09:13 by calamber          #+#    #+#             */
/*   Updated: 2020/02/19 05:43:25 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc_deps.h"

static unsigned long	get_blk_data(t_header *cur, char *name)
{
	unsigned long			freed;
	unsigned long			allocd;

	freed = 0;
	allocd = 0;
	while (cur)
	{
		if (cur->flags & IS_ALLOCD_FLAG)
			allocd += (cur->flags & LARGE_FLAG ? cur->size *
					g_data.meta_size + g_data.meta_size : 1);
		else
			freed += (cur->flags & LARGE_FLAG ? cur->size *
					g_data.meta_size + g_data.meta_size : 1);
		cur = cur->next;
	}
	ft_printf_fd(1, "%s data total: %lu freed: %lu allocd: %lu\n",
							name, allocd + freed, freed, allocd);
	return (allocd + freed);
}

void					malloc_count_blocks(void)
{
	unsigned long tiny_size;
	unsigned long small_size;
	unsigned long large_size;
	unsigned long expected_used;
	unsigned long actual_used;

	ft_printf_fd(1, "\e[1;32mp %u\nh %u\n", g_data.page_size, g_data.meta_size);
	tiny_size = get_blk_data(g_data.tiny, "tiny") * (TINY + g_data.meta_size);
	small_size = get_blk_data(g_data.small, "small") *
							(SMALL + g_data.meta_size);
	large_size = get_blk_data(g_data.large, "large");
	ft_printf_fd(1, "total mapped bytes: %lu\n",
						g_data.debug_stats.bytes_mapped);
	ft_printf_fd(1, "tiny amt: ** logged %lu just found %lu **\n",
			g_data.tiny_amt * (TINY + g_data.meta_size), tiny_size);
	ft_printf_fd(1, "small amt: ** logged %lu just found %lu **\n",
			g_data.small_amt * (SMALL + g_data.meta_size), small_size);
	expected_used = (g_data.tiny_amt * (TINY + g_data.meta_size)) +
			(g_data.small_amt * (SMALL + g_data.meta_size)) + large_size;
	actual_used = tiny_size + small_size + large_size;
	ft_printf_fd(1, "expected bytes in use: %lu\nactual bytes in use: %lu\n",
												expected_used, actual_used);
	ft_printf_fd(1, "total unmapped bytes: %lu\nlost bytes: %lu\n\e[0m",
	g_data.debug_stats.bytes_unmapped, g_data.debug_stats.bytes_mapped -
						g_data.debug_stats.bytes_unmapped - actual_used);
}

static void				print_mem(char *name, t_header *cur, int freed)
{
	unsigned int	type;

	ft_printf_fd(1, "\n%s\n", name);
	while (cur)
	{
		if ((freed && !(cur->flags & IS_ALLOCD_FLAG)) ||
				(!freed && cur->flags & IS_ALLOCD_FLAG))
		{
			type = cur->flags >> 1;
			ft_printf_fd(1, "addr: %p | bytes: %lu | type: %u | allocd: %lu|\n",
				cur, cur->size * g_data.meta_size + g_data.meta_size,
											type, cur->flags & 0x1);
		}
		cur = cur->next;
	}
}

void					show_alloc_mem(void)
{
	if (!malloc_check_init())
		return ;
	ft_printf_fd(1, "\e[1;35m*****************\nALLOCD MEMORY:");
	print_mem("TINY:", g_data.tiny, 0);
	print_mem("SMALL:", g_data.small, 0);
	print_mem("LARGE:", g_data.large, 0);
	ft_printf_fd(1, "*****************\e[0m\n");
}

void					show_free_mem(void)
{
	if (!malloc_check_init())
		return ;
	ft_printf_fd(1, "\e[1;34m*****************\nFREE MEMORY:");
	print_mem("TINY:", g_data.tiny, 1);
	print_mem("SMALL:", g_data.small, 1);
	print_mem("LARGE:", g_data.large, 1);
	ft_printf_fd(1, "*****************\e[0m\n");
}
