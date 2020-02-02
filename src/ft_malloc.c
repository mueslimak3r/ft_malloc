#include "ft_malloc.h"

static t_malloc_data    g_data = { NULL, NULL, NULL, NULL };
static bool				init = false;

void		show_alloc_mem()
{
	t_header	*current = g_data.freep;
	while (current)
	{
		if (current->flags & 0x1)
			printf("%p : %lu bytes\n", current, current->size * META_SIZE + META_SIZE);
		current = current->next;
	}
}

void		show_free_mem()
{
	t_header	*current = g_data.freep;
	while (current)
	{
		if (!(current->flags & 0x1))
			printf("%p : %lu bytes\n", current, current->size * META_SIZE + META_SIZE);
		current = current->next;
	}
}

void		fragment_block(t_header *current, size_t size)
{
	if (current->size > size + 1)
	{
		t_header *new = current + size + 1;
		new->next = current->next;
		new->prev = current;
		new->size = current->size - size - 1;
		new->flags = 0;
		new->block_start = current->block_start;
		current->size = size;
		current->next = new;
	}
}

t_header    *find_free_block(t_header **last, size_t size)
{
	t_header *current = g_data.freep;
	while (current && !(!(current->flags & 0x1) && current->size >= size / META_SIZE))
	{
		*last = current;
		current = current->next;
	}
	if (current)
	{
		printf("found block\n");
		fragment_block(current, size / META_SIZE);
	}
	return current;
}

void		join_blocks(t_header *first, t_header *last)
{
	first->size += last->size + 1;
	first->next = last->next;
	if (last->next)
		last->next->prev = first;
}

void		ft_free(void *ptr)
{
	if (!ptr)
		return ;
	t_header    *block_ptr = (t_header*)ptr - 1;
	block_ptr->flags ^= 0x1;
	if (block_ptr->prev && !(block_ptr->prev->flags & 0x1) && block_ptr->prev->block_start == block_ptr->block_start)
		join_blocks(block_ptr->prev, block_ptr);
	if (block_ptr->next && !(block_ptr->next->flags & 0x1) && block_ptr->next->block_start == block_ptr->block_start)
		join_blocks(block_ptr, block_ptr->next);
}

t_header    *request_space(t_header **last, size_t size)
{
	size_t		page_size = (size_t)getpagesize();
	size_t		to_alloc = size > page_size ? page_size * ((size / page_size) + 1);
	printf("to alloc %zu\n", to_alloc);
	t_header	*block = mmap(NULL, to_alloc, (PROT_READ | PROT_WRITE), (MAP_PRIVATE | MAP_ANONYMOUS), -1, 0);
	if ((void*)block == MAP_FAILED)
		return NULL;
	printf("reserved %zu bytes at %p\n", to_alloc, block);
	block->size = (to_alloc / META_SIZE) - 1;
	block->next = NULL;
	block->prev = NULL;
	block->block_start = block;
	block->flags = 0;
	if (last && *last)
	{
		(*last)->next = block;
		block->prev = *last;
	}
	else
		*last = block;
	fragment_block(block, size / META_SIZE);
	return (block);
}

void        *ft_malloc(size_t size)
{
	t_header *block;
	t_header *last;

	if (size == 0)
		return NULL;
	printf("input size: %zu\n", size);
	size = META_SIZE * (size / META_SIZE) + (size % META_SIZE ? META_SIZE : 0);
	printf("meta size: %lu, new size: %zu\n", META_SIZE, size);
	if (!g_data.freep)
	{
		printf("getting first block\n");
		block = request_space(&g_data.freep, size);
		if (!block)
			return (NULL);
	}
	else
	{
		last = g_data.freep;
		block = find_free_block(&last, size);
		if (!block)
		{
			block = request_space(&last, size);
			if (!block)
				return (NULL);
		}
	}
	if (block)
		block->flags |= 0x1;
	return(block + 1);
}
