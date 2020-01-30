#include "ft_malloc.h"

static t_malloc_data    g_data = {NULL, NULL};

void		show_alloc_mem()
{
	t_header	*current = g_data.allocp;
	while (current)
	{
		printf("%p : %lu bytes\n", current, current->size * META_SIZE + META_SIZE);
		current = current->next;
	}
}

void		show_free_mem()
{
	t_header	*current = g_data.freep;
	while (current)
	{
		printf("%p : %lu bytes\n", current, current->size * META_SIZE + META_SIZE);
		current = current->next;
	}
}

void		fragment_block(t_header *current, t_header **last, size_t size)
{
	if (current->size > size + 1)
	{
		t_header *tmp_next = current->next;
		t_header *new = current + size + 1;
		new->next = tmp_next;
		if (tmp_next)
			tmp_next->prev = new;
		new->prev = last ? *last : NULL;
		new->size = current->size - size - 1;
		new->flags ^= 0x1;
		current->size = size;
		if (last && *last && *last != current)
			(*last)->next = new;
		else
			g_data.freep = new;
	}
	else if (last && *last && *last != current)
		(*last)->next = current->next;
	else
		g_data.freep = current->next;
}

t_header    *find_free_block(t_header **last, size_t size)
{
	t_header *current = g_data.freep;
	while (current && !(current->size >= size / META_SIZE))
	{
		*last = current;
		current = current->next;
	}
	if (current)
	{
		printf("found block\n");
		fragment_block(current, last, size / META_SIZE);
	}
	return current;
}

void		ft_free(void *ptr)
{
	if (!ptr)
		return ;
	t_header    *block_ptr = (t_header*)ptr - 1;
	block_ptr->flags ^= 0x1;
	if (block_ptr->prev)
		block_ptr->prev->next = block_ptr->next;
	if (block_ptr->next)
		block_ptr->next->prev = block_ptr->prev;
	if (g_data.freep)
		g_data.freep->prev = block_ptr;
	if (g_data.allocp == block_ptr)
		g_data.allocp = block_ptr->next;
	block_ptr->next = g_data.freep;
	block_ptr->prev = NULL;
	g_data.freep = block_ptr;
}

t_header    *request_space(t_header **last, size_t size)
{
	size_t		to_alloc = (size_t)getpagesize();
	t_header	*block = mmap(NULL, to_alloc, (PROT_READ | PROT_WRITE), (MAP_PRIVATE | MAP_ANONYMOUS), -1, 0);
	if ((void*)block == MAP_FAILED)
		return NULL;
	printf("reserved %zu bytes at %p\n", to_alloc, block);
	block->size = (to_alloc / META_SIZE) - 1;
	block->next = NULL;
	block->prev = NULL;
	block->flags = 0;
	fragment_block(block, last, size / META_SIZE);
	return (block);
}

void        *ft_malloc(size_t size)
{
	t_header *block;
	t_header *last;

	if (size <= 0)
		return NULL;
	printf("input size: %zu\n", size);
	size = META_SIZE * (size / META_SIZE) + (size % META_SIZE ? META_SIZE : 0);
	printf("meta size: %lu, new size: %zu\n", META_SIZE, size);
	if (!g_data.freep)
	{
		printf("getting first block\n");
		block = request_space(NULL, size);
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
	{
		block->flags &= 0x1;
		block->next = g_data.allocp;
		block->prev = NULL;
		g_data.allocp = block;
	}
	return(block + 1);
}
