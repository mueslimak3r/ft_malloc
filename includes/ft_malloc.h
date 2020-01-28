#ifndef FT_MALLOC_H
#define FT_MALLOC_H

#include "libft.h"

// mmap(2) munmap(2) getpagesize(3) getrlimit(2) libpthread
/*
typedef struct s_header t_header;

struct s_header
{
    t_header        *next;
    unsigned long   size;
    unsigned long   flags;
};

void free(void *ptr);
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
void show_alloc_mem();
*/

int test(void);

#endif