#include "ft_malloc_deps.h"

void		check_unmap(t_header *page_start, unsigned long flags)
{
	t_header	*tmp = page_start;
	t_header	*last = NULL;
	unsigned long	allocs_per_page;

	if (!page_start)
		return ;
	fprintf(g_data.debug_out, "PAGE START %p | flags %lu\n", page_start, page_start->flags);
	if (flags & 0x8)
	{
		if (DEBUG)
			fprintf(g_data.debug_out, "FREEING LARGE %p | size %lu\n", page_start, page_start->size);
		if (page_start->prev)
			page_start->prev->next = page_start->next;
		if (page_start->next)
			page_start->next->prev = page_start->prev;
		if (g_data.large == page_start)
			g_data.large = g_data.large->next;
		if (DEBUG)
			fprintf(g_data.debug_out, "UNMAPPING: %p size: %lu\n", page_start, page_start->size * g_data.meta_size + g_data.meta_size);
		munmap(page_start, page_start->size * g_data.meta_size + g_data.meta_size);
		return ;
	}

	if (page_start->flags & 0x2)
		allocs_per_page = g_data.page_size / (TINY + g_data.meta_size);
	else if (page_start->flags & 0x4)
		allocs_per_page = g_data.page_size / (SMALL + g_data.meta_size);
	else
		return ;

	if (page_start->flags & 0x2 && g_data.tiny_amt - allocs_per_page < MIN_ALLOC)
		return ;
	else if (page_start->flags & 0x4 && g_data.small_amt - allocs_per_page < MIN_ALLOC)
		return ;

	while (tmp && (unsigned long)tmp < (unsigned long)page_start + g_data.page_size)
	{
		if (tmp->flags & 0x1)
			return ;
		last = tmp;
		tmp = tmp->next;
	}
	if (page_start->prev)
		page_start->prev->next = last->next;
	if (last && last->next)
		last->next->prev = page_start->prev;
	if (page_start == g_data.tiny)
	{
		g_data.tiny_amt -= allocs_per_page;
		g_data.tiny = last ? last->next : NULL;
	}
	else if (page_start == g_data.small)
	{
		g_data.small_amt -= allocs_per_page;
		g_data.small = last ? last->next : NULL;
	}
	if (DEBUG)
		fprintf(g_data.debug_out, "UNMAPPING: %p size: %lu\n", page_start, g_data.page_size);
	munmap(page_start, g_data.page_size);
}

void		free(void *ptr)
{
	if (!ptr)
		return ;
	if (!g_initialized)
	{
		ft_malloc_init();
		return ;
	}
	t_header    *block_ptr = (t_header*)ptr - 1;
	if (DEBUG)
	{
		fprintf(g_data.debug_out, "|freeing: %p| from input ptr %p| flags: %lu | prev %p | next %p |\n", block_ptr, ptr, block_ptr->flags, block_ptr->prev, block_ptr->next);
		if (block_ptr->flags & 8)
		{
			t_header *current = g_data.large;
			printf("LARGE ALLOCS:\n");
			while (current)
			{
				if (current->flags & 0x1)
				{
					unsigned int type = current->flags >> 1;
					fprintf(g_data.debug_out, "%p : %lu bytes, type: %u\n", current, current->size * g_data.meta_size + g_data.meta_size, type);
				}
				current = current->next;
			}
		}
	}
	block_ptr->flags ^= 0x1;

	if (block_ptr->flags & 0x8)
		check_unmap(block_ptr, 0x8);
	else
	{
		unsigned long offset = (unsigned long)block_ptr % g_data.page_size;
		if (offset)
			offset /= g_data.meta_size;
		check_unmap(block_ptr - offset, block_ptr->flags);
	}

	fprintf(g_data.debug_out, "FINISHED FREEING %p\n", block_ptr);
}
