#include "ft_malloc_deps.h"

void		show_alloc_mem()
{
	t_header	*current = g_data.tiny;
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
	t_header	*current = g_data.tiny;
	printf("TINY:\n");
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
