#ifndef FT_MALLOC_H
#define FT_MALLOC_H

#include <stdio.h>
#include <unistd.h>

void		free(void *ptr);
void		*malloc(size_t size);
void		*realloc(void *ptr, size_t size);


void 		show_alloc_mem();
void 		show_free_mem();
void		count_blocks();

#endif
