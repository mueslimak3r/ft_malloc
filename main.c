#include "ft_malloc.h"
#include <stdio.h>

int main(void)
{
	char *ref = "hello world";
	int len = ft_strlen(ref);
	char *s = ft_malloc(len + 1);
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
	printf("\nfree memory:\n");
	show_free_mem();

	printf("\n");
	char *s2 = ft_malloc(len + 1);
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
	printf("\nfree memory:\n");
	show_free_mem();
    return (0);
}
