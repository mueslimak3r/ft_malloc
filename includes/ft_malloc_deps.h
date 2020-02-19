/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc_deps.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 02:48:03 by calamber          #+#    #+#             */
/*   Updated: 2020/02/19 01:38:07 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MALLOC_DEPS_H
# define FT_MALLOC_DEPS_H

# include <sys/mman.h>
# include <unistd.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdlib.h>
# include <stdarg.h>
# include <errno.h>
# include <stdio.h>
# include <string.h>
# include <pthread.h>

# define FT_PROT (PROT_READ | PROT_WRITE)
# define FT_MAP (MAP_ANONYMOUS | MAP_PRIVATE)

/*
** zones
*/

# define MIN_ALLOC 100
# define TINY	96
# define SMALL	2016
# define DEBUG	0

/*
** block flags bitfield values
*/

# define TINY_FLAG 		2u
# define SMALL_FLAG		4u
# define LARGE_FLAG		8u
# define IS_ALLOCD_FLAG	1u

typedef struct s_header			t_header;
typedef struct s_malloc_data	t_malloc_data;
typedef struct s_malloc_stats	t_malloc_stats;

struct			s_header
{
	unsigned long	size;
	unsigned long	flags;
	t_header		*next;
	t_header		*prev;
};

struct			s_malloc_stats
{
	unsigned long	bytes_mapped;
	unsigned long	bytes_unmapped;
};

struct			s_malloc_data
{
	t_header		*tiny;
	t_header		*small;
	t_header		*large;
	unsigned long	tiny_amt;
	unsigned long	small_amt;
	unsigned int	page_size;
	unsigned int	meta_size;
	t_malloc_stats	debug_stats;
};

extern t_malloc_data	g_data;
extern pthread_mutex_t	g_mutex;
//static bool				g_initialized;

int				malloc_check_init(void);
void			*ft_malloc(size_t size);
void			ft_free(void *ptr);
void			*ft_realloc(void *ptr, size_t size);
void			*ft_calloc(size_t count, size_t size);
void			ft_malloc_init(void);
void			ft_putnbr_u_base_fd(unsigned long long nb, int base, int fd);
t_header		*request_space(size_t size, size_t units,
					unsigned long flags, unsigned long *amt);
int				malloc_check_if_valid(t_header *block_ptr);
int				ft_printf_fd(int fd, char *fmt, ...);
void			ft_putstr_fd(char const *s, int fd);
#endif
