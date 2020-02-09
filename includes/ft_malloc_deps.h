#ifndef FT_MALLOC_DEPS_H
#define FT_MALLOC_DEPS_H


#include <sys/mman.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h> // remove later
#include "libft.h"
// mmap(2) munmap(2) getpagesize(3) getrlimit(2) libpthread

/*
** zones
*/

#define MIN_ALLOC 100
#define TINY 224
#define SMALL 2016
#define DEBUG 0

/*
** block flags bitfield values
*/

#define TINY_FLAG 		2u
#define SMALL_FLAG		4u
#define LARGE_FLAG		8u
#define IS_ALLOCD_FLAG	1u

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
	FILE			*debug_out;
	unsigned long	tiny_amt;
	unsigned long	small_amt;
	size_t			page_size;
	size_t			meta_size;
};

t_malloc_data			g_data;
bool					g_initialized;

void        *ft_malloc(size_t size);
void		ft_free(void *ptr);
void		*ft_realloc(void *ptr, size_t size);
void		ft_malloc_init(void);
void		ft_putnbr_u_base_fd(unsigned long nb, int base, int fd);

#endif
