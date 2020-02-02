#include "ft_malloc.h"

static t_malloc_data    g_data = { NULL, NULL, NULL };
static bool				g_initialized = false;

void		show_alloc_mem()
{
	t_header	*current = g_data.tiny;
	while (current)
	{
		if (current->flags & 0x1)
			printf("%p : %lu bytes\n", current, current->size * META_SIZE + META_SIZE);
		current = current->next;
	}
	current = g_data.small;
	while (current)
	{
		if (current->flags & 0x1)
			printf("%p : %lu bytes\n", current, current->size * META_SIZE + META_SIZE);
		current = current->next;
	}
	current = g_data.large;
	while (current)
	{
		if (current->flags & 0x1)
			printf("%p : %lu bytes\n", current, current->size * META_SIZE + META_SIZE);
		current = current->next;
	}
}

void		show_free_mem()
{
	
	t_header	*current = g_data.tiny;
	while (current)
	{
		if (!(current->flags & 0x1))
			printf("%p : %lu bytes\n", current, current->size * META_SIZE + META_SIZE);
		current = current->next;
	}
	current = g_data.small;
	while (current)
	{
		if (!(current->flags & 0x1))
			printf("%p : %lu bytes\n", current, current->size * META_SIZE + META_SIZE);
		current = current->next;
	}
	current = g_data.large;
	while (current)
	{
		if (!(current->flags & 0x1))
			printf("%p : %lu bytes\n", current, current->size * META_SIZE + META_SIZE);
		current = current->next;
	}
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

void		ft_free(void *ptr)
{
	if (!ptr)
		return ;
	t_header    *block_ptr = (t_header*)ptr - 1;
	block_ptr->flags ^= 0x1;
}

t_header    *request_space(size_t size)
{
	size_t		page_size = (size_t)getpagesize();
	size_t		to_alloc = size > page_size ? (page_size * ((size / page_size) + 1)) : page_size;
	printf("to alloc %zu\n", to_alloc);
	t_header	*block = mmap(NULL, to_alloc, (PROT_READ | PROT_WRITE), (MAP_PRIVATE | MAP_ANONYMOUS), -1, 0);
	if ((void*)block == MAP_FAILED)
		return NULL;
	printf("reserved %zu bytes at %p\n", to_alloc, block);
	return (block);
}

void		init_blocks(t_header *block, size_t block_size, size_t block_amt)
{
	t_header *last;

	last = NULL;
	while (block_amt > 0)
	{
		block->next = NULL;
		block->prev = last;
		if (last)
			last->next = block;
		block->flags = 0;
		block->size = block_size / META_SIZE;
		last = block;
		block += (block_size / META_SIZE);
		block_amt--;
	}
}

void		ft_malloc_init(void)
{
	g_data.tiny = request_space(MIN_ALLOC * (TINY + META_SIZE));
	if (!g_data.tiny)
		return ;
	init_blocks(g_data.tiny, TINY, MIN_ALLOC);
	g_data.small = request_space(MIN_ALLOC * (SMALL + META_SIZE));
	if (!g_data.small)
		return ;
	init_blocks(g_data.small, SMALL, MIN_ALLOC);
	g_initialized = true;
}

void        *ft_malloc(size_t size)
{
	t_header	*block;
	t_header	*last;
	size_t		block_size;
	bool		alloc_single_zone = false;

	if (size == 0)
		return NULL;
	block_size = 0;
	printf("input size: %zu\n", size);
	size = META_SIZE * (size / META_SIZE) + (size % META_SIZE ? META_SIZE : 0);
	printf("meta size: %lu, new size: %zu\n", META_SIZE, size);
	if (!g_initialized)
		ft_malloc_init();
	last = NULL;
	if (size <= TINY)
	{
		block_size = TINY;
		last = g_data.tiny;
	}
	else if (size > TINY && size <= SMALL)
	{
		block_size = SMALL;
		last = g_data.small;
	}
	else
	{
		last = g_data.large;
		alloc_single_zone = true;
		block_size = size;
	}
	block = find_free_block(&last, size);
	if (!block)
	{
		block = request_space(alloc_single_zone ? size + META_SIZE : MIN_ALLOC * (block_size + META_SIZE));
		if (!block)
			return (NULL);
		init_blocks(block, block_size, alloc_single_zone ? 1 : MIN_ALLOC);
		block->prev = last;
		if (last)
			last->next = block;
		else
			last = block;
	}
	if (block)
		block->flags |= 0x1;
	return(block + 1);
}
