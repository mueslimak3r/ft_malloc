#include "unit_tests.h"

int	unit_test3(void)
{
	printf("starting\n");
	if (malloc(0))
	{
		;
	}
	count_blocks();
	void *tiny;
	tiny = malloc(TINY / 2);
	printf("____________________________________________________________\n");
	printf("original: %p\n", tiny);
	show_alloc_mem();
	count_blocks();
	tiny = realloc(tiny, TINY);
	printf("reallocd1: %p\n", tiny);
	show_alloc_mem();
	count_blocks();
	printf("____________________________________________________________\n");

	tiny = realloc(tiny, SMALL);
	printf("____________________________________________________________\n");
	printf("reallocd2: %p\n", tiny);
	show_alloc_mem();
	count_blocks();
	printf("____________________________________________________________\n");
	return (0);
}
