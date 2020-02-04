#include "ft_malloc_deps.h"

void		check_unmap(t_header *page_start)
{
	t_header	*tmp = page_start;
	t_header	*last = NULL;
	unsigned long	allocs_per_page;

	if (!page_start)
		return ;
	if (page_start->flags & 0x8)
	{
		if (page_start->prev)
			page_start->prev->next = page_start->next;
		if (page_start->next)
			page_start->next->prev = page_start->prev;
		if (g_data.large == page_start)
			g_data.large = g_data.large->next;
		munmap(page_start, page_start->size * g_data.meta_size + g_data.meta_size);
		return ;
	}
	else if (page_start->flags & 0x2)
		allocs_per_page = g_data.page_size / (TINY + g_data.meta_size);
	else if (page_start->flags & 0x4)
		allocs_per_page = g_data.page_size / (SMALL + g_data.meta_size);
	else
		return ;

	if (page_start->flags & 0x2 && g_data.tiny_amt - allocs_per_page < MIN_ALLOC)
		return ;
	else if (page_start->flags & 0x4 && g_data.small_amt - allocs_per_page < MIN_ALLOC)
		return ;


	while (tmp && (uintptr_t)tmp < (uintptr_t)page_start + g_data.page_size)
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
	munmap(page_start, g_data.page_size);
}

void		ft_free(void *ptr)
{
	if (!ptr)
		return ;
	if (!g_initialized)
	{
		ft_malloc_init();
		return ;
	}
	t_header    *block_ptr = (t_header*)ptr - 1;
	block_ptr->flags ^= 0x1;
	check_unmap((t_header*)((uintptr_t)block_ptr - ((uintptr_t)block_ptr % g_data.page_size)));
}
