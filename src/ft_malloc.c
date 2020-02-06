#include "ft_malloc_deps.h"

void		init_blocks(t_header *block, size_t block_size, size_t block_amt, unsigned long flags)
{
	t_header		*last;
	unsigned long	size_in_blocks;

	last = NULL;
	if (!block)
		return ;
	size_in_blocks = block_size / g_data.meta_size;
	if (DEBUG)
		fprintf(g_data.debug_out, "size in blks: %lu blk size %zu meta size: %zu\n", size_in_blocks, block_size, g_data.meta_size);
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
	size_t		to_alloc = size > g_data.page_size ? (g_data.page_size * ((size / g_data.page_size) + 1)) : g_data.page_size;
	size_t		units_allocd;

	fprintf(g_data.debug_out, "to alloc %zu\n", to_alloc);
	t_header	*block = mmap(NULL, to_alloc, (PROT_READ | PROT_WRITE), (MAP_PRIVATE | MAP_ANONYMOUS), -1, 0);
	if ((void*)block == MAP_FAILED)
		return NULL;
	block->next = NULL;
	block->prev = NULL;
	block->size = (to_alloc / g_data.meta_size) - 1;
	block->flags = flags;
	if (!(flags & 0x8))
	{
		units_allocd = (block->size + 1) / ((units / g_data.meta_size) + 1);
		*amt = units_allocd;
		init_blocks(block, units, units_allocd, flags);
	}
	if (DEBUG)
		fprintf(g_data.debug_out, "reserved %zu bytes at %p\n", to_alloc, block);
	return (block);
}

void		ft_malloc_init(void)
{
	if (DEBUG)
		g_data.debug_out = fopen("/tmp/ft_malloc_debug", "wab+");
	else
		g_data.debug_out = stdout;
	g_data.meta_size = sizeof(t_header);
	g_data.page_size = (size_t)getpagesize();
	g_data.tiny = request_space(MIN_ALLOC * (TINY + g_data.meta_size), TINY, 0x2, &g_data.tiny_amt);
	if (!g_data.tiny)
		return ;
	g_data.small = request_space(MIN_ALLOC * (SMALL + g_data.meta_size), SMALL, 0x4, &g_data.small_amt);
	if (!g_data.small)
		return ;
	g_initialized = true;
}

t_header    *find_free_block(t_header **last, size_t size)
{
	t_header *current = last ? *last : NULL;
	while (current && !(!(current->flags & 0x1) && current->size >= size / g_data.meta_size))
	{
		*last = current;
		current = current->next;
	}
	if (current)
		fprintf(g_data.debug_out, "found block\n");
	return current;
}

void        *malloc(size_t size)
{
	t_header		*block;
	t_header		*last;
	size_t			block_size;
	unsigned int	flags;

	if (!g_initialized)
		ft_malloc_init();
	if (size == 0)
		return NULL;
	block_size = 0;
	if (DEBUG)
		fprintf(g_data.debug_out, "input size: %zu\n", size);
	size = g_data.meta_size * (size / g_data.meta_size) + (size % g_data.meta_size ? g_data.meta_size : 0);
	if (DEBUG)
		fprintf(g_data.debug_out, "meta size: %lu, new size: %zu\n", g_data.meta_size, size);
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
		if (flags == 0x8)
			block = request_space(size + g_data.meta_size, 1, 0x8, NULL);
		else
			block = request_space(MIN_ALLOC * (block_size + g_data.meta_size), block_size, flags, flags == 0x2 ? &g_data.tiny_amt : &g_data.small_amt);
		if (!block)
			return (NULL);
		block->prev = NULL;
		if (last)
		{
			block->prev = last;
			last->next = block;
		}
		else if (flags == 0x8)
			g_data.large = block;
	}
	if (block)
		block->flags |= 0x1;
	return(block + 1);
}
