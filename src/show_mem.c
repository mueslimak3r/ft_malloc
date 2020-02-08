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

void		get_blk_data(t_header *cur, char *name)
{
	int			freed;
	int			allocd;

	freed = 0;
	allocd = 0;
	while (cur)
	{
		if (cur->flags & 0x1)
			allocd++;
		else
			freed++;
		cur = cur->next;
	}
	printf("%s data total: %d freed: %d allocd: %d\n", name, allocd + freed, freed, allocd);
}

void		count_blocks()
{
	printf("\e[1;32m");
	get_blk_data(g_data.tiny, "tiny");
	get_blk_data(g_data.small, "small");
	get_blk_data(g_data.large, "large");
	printf("\e[0m");
}

void		print_mem(char *name, t_header *cur, int freed)
{
	printf("\n%s\n", name);
	while (cur)
	{
		if ((freed && !(cur->flags & 0x1)) || (!freed && cur->flags & 0x1))
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
