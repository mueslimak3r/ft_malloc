#ifndef FT_MALLOC_H
#define FT_MALLOC_H

#include "libft.h"
#include <sys/mman.h>
#include <unistd.h>
#include <stdbool.h>
// mmap(2) munmap(2) getpagesize(3) getrlimit(2) libpthread
#define MIN_ALLOC 100
#define TINY 224
#define SMALL 2016

typedef struct s_header         t_header;
typedef struct s_malloc_data    t_malloc_data;

struct s_header
{
    t_header        *next;
	t_header		*prev;
    unsigned long	size;
	unsigned long	flags;
};

struct s_malloc_data
{
	t_header		*tiny;
	t_header		*small;
	t_header		*large;
};

#define META_SIZE sizeof(t_header)

void ft_free(void *ptr);
void *ft_malloc(size_t size);

//void *ft_realloc(void *ptr, size_t size);

void show_alloc_mem();
void show_free_mem();

#endif
