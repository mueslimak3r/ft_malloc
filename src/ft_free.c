#include "ft_malloc_deps.h"

int			find_allocd_block(t_header *page_start, t_header **last)
{
	t_header	*tmp;
	int			ret;

	ret = 0;
	*last = NULL;
	tmp = page_start;
	while (tmp && (unsigned long)tmp < (unsigned long)page_start + g_data.page_size)
	{
		if (tmp->flags & 0x1)
			ret = 1;
		*last = tmp;
		tmp = tmp->next;
	}
	return (ret);
}

int			check_unmap(t_header *page_start, unsigned long flags)
{
	t_header	*last;
	unsigned long	allocs_per_page;

	allocs_per_page = g_data.page_size / ((flags & 0x2 ? TINY : SMALL) + g_data.meta_size);
	if (!page_start || (flags & 0x2 && g_data.tiny_amt - allocs_per_page <
	MIN_ALLOC) || (flags & 0x4 && g_data.small_amt - allocs_per_page < MIN_ALLOC)
	|| (find_allocd_block(page_start, &last)) || !(flags & 0x2 || flags & 0x4))
		return (0);
	if (flags & 0x2)
		g_data.tiny_amt -= allocs_per_page;
	else if (flags & 0x4)
		g_data.small_amt -= allocs_per_page;
	if (page_start == g_data.tiny)
		g_data.tiny = last && (unsigned long)last->next >= (unsigned long)page_start + g_data.page_size ? last->next : NULL;
	if (page_start == g_data.small)
		g_data.small = last && (unsigned long)last->next >= (unsigned long)page_start + g_data.page_size ? last->next : NULL;
	if (page_start->prev)
		page_start->prev->next = last->next;
	if (last && last->next)
		last->next->prev = page_start->prev;
	munmap(page_start, g_data.page_size);
	return (1);
}

void		ft_free(void *ptr)
{
	t_header		*block_ptr;
	unsigned long	offset;

	if (ptr == NULL || !g_initialized)
		return ;
	pthread_mutex_lock(&g_mutex_lock);
	block_ptr = ((t_header*)ptr) - 1;
	if (!block_ptr)
		return ;
	//ft_putstr_fd("FREEING: ", 0);
	//ft_putnbr_u_base_fd((unsigned long)ptr, 16, 0);
	//ft_putstr_fd("\n", 0);
	if (block_ptr->flags & 0x8)
	{
		if (block_ptr->prev)
			block_ptr->prev->next = block_ptr->next;
		if (block_ptr->next)
			block_ptr->next->prev = block_ptr->prev;
		if (g_data.large == block_ptr)
			g_data.large = g_data.large->next;
		munmap(block_ptr, block_ptr->size * g_data.meta_size + g_data.meta_size);
	}
	else if ((unsigned long)block_ptr >= g_data.page_size)
	{
		offset = (unsigned long)block_ptr % g_data.page_size;
		if (offset)
			offset /= g_data.meta_size;
		//ft_putstr_fd("CHECKING UNMAP: ", 0);
		//ft_putnbr_u_base_fd((unsigned long)(block_ptr - offset), 16, 0);
		//ft_putstr_fd("\n", 0);
		if (!(check_unmap(block_ptr - offset, block_ptr->flags)))
			block_ptr->flags ^= 0x1;
	}
	else
		block_ptr->flags ^= 0x1;
	pthread_mutex_unlock(&g_mutex_lock);
}

void		free(void *ptr)
{
	ft_free(ptr);
}
