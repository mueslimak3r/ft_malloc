#include "ft_malloc_deps.h"

void		show_alloc_mem()
{
	if (!g_initialized)
		ft_malloc_init();
	t_header	*current = g_data.tiny;
	printf("ALLOCD:\nSMALL:\n");
	while (current)
	{
		if (current->flags & 0x1)
		{
			unsigned int type = current->flags >> 1;
			printf("%p : %lu bytes, type: %u\n", current, current->size * g_data.meta_size + g_data.meta_size, type);
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
			printf("%p : %lu bytes, type: %u\n", current, current->size * g_data.meta_size + g_data.meta_size, type);
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
			printf("%p : %lu bytes, type: %u\n", current, current->size * g_data.meta_size + g_data.meta_size, type);
		}
		current = current->next;
	}
}

void		show_free_mem()
{
	if (!g_initialized)
		ft_malloc_init();
	t_header	*current = g_data.tiny;
	printf("FREE:\nTINY:\n");
	while (current)
	{
		if (!(current->flags & 0x1))
		{
			unsigned int type = current->flags >> 1;
			printf("%p : %lu bytes, type: %u\n", current, current->size * g_data.meta_size + g_data.meta_size, type);
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
			printf("%p : %lu bytes, type: %u\n", current, current->size * g_data.meta_size + g_data.meta_size, type);
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
			printf("%p : %lu bytes, type: %u\n", current, current->size * g_data.meta_size + g_data.meta_size, type);
		}
		current = current->next;
	}
}
