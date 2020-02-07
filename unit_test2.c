#include "ft_malloc.h"
#include "ft_malloc_deps.h"
#include "libft.h"
#include <stdio.h>

int main(void)
{
	printf("starting\n");
	if (malloc(0))
	{
		;
	}
	count_blocks();
	printf("\nalloc'd memory:\n");
	show_alloc_mem();
	printf("\nfree memory:\n");
	show_free_mem();
	printf("\n\nbefore\n");
	void *tiny_arr[112];
	for (int i = 0; i < 112; i++)
		tiny_arr[i] = malloc(TINY);

	void *small_arr[102];
	for (int i = 0; i < 102; i++)
		small_arr[i] = malloc(SMALL);
	printf("\n\nafter\n");
	printf("\nalloc'd memory:\n");
	show_alloc_mem();
	printf("\nfree memory:\n");
	show_free_mem();
	count_blocks();
	void	*big_one = malloc(4096 * 2);
	if (big_one)
	{
		;
	}
	printf("\nbig alloc\nalloc'd memory:\n");
	show_alloc_mem();
	count_blocks();
	free(big_one);
	printf("freed big one\n");
	count_blocks();
	void *new_tiny_arr[112];
	for (int i = 0; i < 112; i++)
		new_tiny_arr[i] = malloc(TINY);
	printf("\n\nafter\n");
	printf("\nalloc'd memory:\n");
	show_alloc_mem();
	printf("\nfree memory:\n");
	show_free_mem();
	count_blocks();
	for (int i = 0; i < 112; i++)
		free(new_tiny_arr[i]);
	/*
	printf("\n\nafter free big\n");
	printf("\nalloc'd memory:\n");
	show_alloc_mem();
	printf("\nfree memory:\n");
	show_free_mem();
	*/
	printf("\n");
	count_blocks();
	return (0);
}
