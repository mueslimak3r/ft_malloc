#include "ft_malloc.h"

static t_malloc_data    g_data = { NULL, NULL, NULL, 0, 0, 0 };
static bool				g_initialized = false;

void		show_alloc_mem()
{
	t_header	*current = g_data.tiny;
	printf("SMALL:\n");
	while (current)
	{
		if (current->flags & 0x1)
		{
			unsigned int type = current->flags >> 1;
			printf("%p : %lu bytes, type: %u\n", current, current->size * META_SIZE + META_SIZE, type);
		}
		current = current->next;
	}
	current = g_data.small;
	printf("SMALL:\n");
	while (current)
	{
		if (current->flags & 0x1)
		{
			unsigned int type = current->flags >> 1;
			printf("%p : %lu bytes, type: %u\n", current, current->size * META_SIZE + META_SIZE, type);
		}
		current = current->next;
	}
	current = g_data.large;
	printf("LARGE:\n");
	while (current)
	{
		if (current->flags & 0x1)
		{
			unsigned int type = current->flags >> 1;
			printf("%p : %lu bytes, type: %u\n", current, current->size * META_SIZE + META_SIZE, type);
		}
		current = current->next;
	}
}

void		show_free_mem()
{
	t_header	*current = g_data.tiny;
	printf("TINY:\n");
	while (current)
	{
		if (!(current->flags & 0x1))
		{
			unsigned int type = current->flags >> 1;
			printf("%p : %lu bytes, type: %u\n", current, current->size * META_SIZE + META_SIZE, type);
		}
		current = current->next;
	}
	current = g_data.small;
	printf("SMALL:\n");
	while (current)
	{
		if (!(current->flags & 0x1))
		{
			unsigned int type = current->flags >> 1;
			printf("%p : %lu bytes, type: %u\n", current, current->size * META_SIZE + META_SIZE, type);
		}
		current = current->next;
	}
	current = g_data.large;
	printf("LARGE:\n");
	while (current)
	{
		if (!(current->flags & 0x1))
		{
			unsigned int type = current->flags >> 1;
			printf("%p : %lu bytes, type: %u\n", current, current->size * META_SIZE + META_SIZE, type);
		}
		current = current->next;
	}
}

t_header    *request_space(size_t size)
{
	size_t		to_alloc = size > g_data.page_size ? (g_data.page_size * ((size / g_data.page_size) + 1)) : g_data.page_size;
	printf("to alloc %zu\n", to_alloc);
	t_header	*block = mmap(NULL, to_alloc, (PROT_READ | PROT_WRITE), (MAP_PRIVATE | MAP_ANONYMOUS), -1, 0);
	if ((void*)block == MAP_FAILED)
		return NULL;
	block->next = NULL;
	block->prev = NULL;
	block->size = to_alloc / META_SIZE - 1;
	block->flags = 0x8;
	printf("reserved %zu bytes at %p\n", to_alloc, block);
	return (block);
}

void		init_blocks(t_header *block, size_t block_size, size_t block_amt, unsigned long flags)
{
	t_header		*last;

	last = NULL;
	if (!block)
		return ;
	while (block_amt > 0)
	{
		block->next = NULL;
		block->prev = last;
		if (last)
			last->next = block;
		block->flags = flags;
		block->size = block_size / META_SIZE;
		last = block;
		block += (block_size / META_SIZE);
		block_amt--;
	}
}

void		ft_malloc_init(void)
{
	g_data.page_size = (size_t)getpagesize();
	g_data.tiny = request_space(MIN_ALLOC * (TINY + META_SIZE));
	if (!g_data.tiny)
		return ;
	init_blocks(g_data.tiny, TINY, MIN_ALLOC, 0x2);
	g_data.tiny_amt = MIN_ALLOC;
	g_data.small = request_space(MIN_ALLOC * (SMALL + META_SIZE));
	if (!g_data.small)
		return ;
	init_blocks(g_data.small, SMALL, MIN_ALLOC, 0x4);
	g_data.tiny_amt = MIN_ALLOC;
	g_initialized = true;
}

t_header    *find_free_block(t_header **last, size_t size)
{
	t_header *current = last ? *last : NULL;
	while (current && !(!(current->flags & 0x1) && current->size >= size / META_SIZE))
	{
		*last = current;
		current = current->next;
	}
	if (current)
		printf("found block\n");
	return current;
}

void		unmap_mem(t_header *page_start, size_t amt)
{
	if (!page_start)
		return ;
	if (page_start->flags & 0x2)
		g_data.tiny_amt -= amt / (TINY + META_SIZE);
	else if (page_start->flags & 0x4)
		g_data.small_amt -= amt / (SMALL + META_SIZE);
	munmap(page_start, amt);
}

void		check_unmap(t_header *page_start)
{
	t_header	*tmp = page_start;
	t_header	*last = NULL;
	if (page_start->flags & 0x8)
	{
		if (page_start->prev)
			page_start->prev->next = page_start->next;
		if (page_start->next)
			page_start->next->prev = page_start->prev;
		if (g_data.large == page_start)
			g_data.large = g_data.large->next;
		unmap_mem(page_start, page_start->size * META_SIZE + META_SIZE);
		return ;
	}
	if (page_start->flags & 0x2 && g_data.tiny_amt - (g_data.page_size / (TINY + META_SIZE)) < MIN_ALLOC)
		return ;
	if (page_start->flags & 0x4 && g_data.small_amt - (g_data.page_size / (SMALL + META_SIZE)) < MIN_ALLOC)
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
	if (page_start && page_start == g_data.tiny)
		g_data.tiny = last ? last->next : NULL;
	else if (page_start && page_start == g_data.small)
		g_data.small = last ? last->next : NULL;
	unmap_mem(page_start, g_data.page_size);
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

void        *ft_malloc(size_t size)
{
	t_header		*block;
	t_header		*last;
	size_t			block_size;
	unsigned int	flags;

	if (size == 0)
		return NULL;
	block_size = 0;
	printf("input size: %zu\n", size);
	size = META_SIZE * (size / META_SIZE) + (size % META_SIZE ? META_SIZE : 0);
	printf("meta size: %lu, new size: %zu\n", META_SIZE, size);
	if (!g_initialized)
		ft_malloc_init();
	flags = 0;
	last = NULL;
	if (size <= TINY)
	{
		flags = 0x2;
		block_size = TINY;
		last = g_data.tiny;
	}
	else if (size > TINY && size <= SMALL)
	{
		flags = 0x4;
		block_size = SMALL;
		last = g_data.small;
	}
	else
	{
		flags = 0x8;
		last = g_data.large;
		block_size = size;
	}
	block = find_free_block(&last, size);
	if (!block)
	{
		block = request_space(flags == 0x8 ? size + META_SIZE : MIN_ALLOC * (block_size + META_SIZE));
		if (!block)
			return (NULL);
		if (!(flags & 0x8))
			init_blocks(block, block_size, MIN_ALLOC, flags);
		if (flags != 0x8 && block_size == TINY)
			g_data.tiny_amt += MIN_ALLOC;
		else if (flags != 0x8 && block_size == SMALL)
			g_data.small_amt += MIN_ALLOC;
		block->prev = NULL;
		if (last)
		{
			block->prev = last;
			last->next = block;
		}
		else if (flags & 0x8)
			g_data.large = block;
	}
	if (block)
		block->flags |= 0x1;
	return(block + 1);
}
