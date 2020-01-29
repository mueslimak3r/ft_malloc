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
	printf("|%s|\n", s);
	printf("alloc'd memory:\n");
	show_alloc_mem();
	printf("free memory:\n");
	show_free_mem();
    return (0);
}
