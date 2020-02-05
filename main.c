#include "ft_malloc.h"
#include "libft.h"
#include <stdio.h>

int main(void)
{
	malloc(0);

	printf("\nalloc'd memory:\n");
	show_alloc_mem();
	printf("\nfree memory:\n");
	show_free_mem();

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
	printf("\nfree memory:\n");
	show_free_mem();

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
	printf("\nfree memory:\n");
	show_free_mem();


	printf("\nfreeing s2...\n");
	free(s2);
	printf("\nalloc'd memory:\n");
	show_alloc_mem();
	printf("\nfree memory:\n");
	show_free_mem();

	printf("\nfreeing s1...\n");
	free(s);
	printf("\nalloc'd memory:\n");
	show_alloc_mem();
	printf("\nfree memory:\n");
	show_free_mem();

	char *large_mem = malloc(8192);
	int nb;
	for (nb = 0; nb < 8191; nb++)
	{
		large_mem[nb] = 'A';//((int)'A' + i + 1) % 26;
	}
	large_mem[nb] = '\0';
	printf("%s\n", large_mem);
	char *large_two = malloc(8192);
	for (nb = 0; nb < 8191; nb++)
	{
		large_two[nb] = 'A';//((int)'A' + i + 1) % 26;
	}
	large_two[nb] = '\0';
	printf("%s\n", large_two);
	printf("\nalloc'd memory:\n");
	show_alloc_mem();
	printf("\nfree memory:\n");
	show_free_mem();
	free(large_mem);
	printf("\nalloc'd memory:\n");
	show_alloc_mem();
	printf("\nfree memory:\n");
	show_free_mem();
    return (0);
}
