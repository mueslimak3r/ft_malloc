#include "unit_tests.h"

int	unit_test1(void)
{
	printf("starting\n");
	if (malloc(0))
	{
		;
	}

	printf("\nalloc'd memory:\n");
	show_alloc_mem();

	char *ref = "hello world";
	int len = ft_strlen(ref);
	char *s = malloc(len + 1);
	if (!s)
	{
		printf("error\n");
		return (0);
	}
	int i;
	for (i = 0; i < len; i++)
	{
		s[i] = ref[i];
	}
	s[i] = '\0';
	printf("|%s| i: %d\n", s, i);
	printf("\nalloc'd memory:\n");
	show_alloc_mem();

	printf("\n");
	char *s2 = malloc(len + 1);
	if (!s2)
	{
		printf("error\n");
		return (0);
	}
	for (i = 0; i < len; i++)
	{
		s2[i] = ft_isalpha(ref[i]) ? ref[i] - 32 : ref[i];
	}
	s2[i] = '\0';
	printf("|%s| i: %d\n", s2, i);
	printf("\nalloc'd memory:\n");
	show_alloc_mem();

	printf("\nfreeing s2...\n");
	free(s2);
	printf("\nalloc'd memory:\n");
	show_alloc_mem();

	printf("\nfreeing s1...\n");
	free(s);
	printf("\nalloc'd memory:\n");
	show_alloc_mem();

	char *large_mem = malloc(8192);
	if (!large_mem)
	{
		;
	}
	char *large_two = malloc(8192);
	if (!large_two)
	{
		;
	}
	printf("\nalloc'd memory:\n");
	show_alloc_mem();

	free(large_mem);
	printf("\nalloc'd memory:\n");
	show_alloc_mem();
	count_blocks();
    return (0);
}
