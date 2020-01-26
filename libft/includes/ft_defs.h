/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_defs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/04 22:53:01 by calamber          #+#    #+#             */
/*   Updated: 2019/12/19 15:40:33 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_DEFS_H
# define FT_DEFS_H

# include <unistd.h>
# include <stdlib.h>
# include <stdarg.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdint.h>
# include <wchar.h>
# include <locale.h>
# include "libft.h"

typedef struct				s_gnl_file
{
	int						fd;
	char					*buf;
}							t_gnl_file;

typedef struct				s_buffer
{
	char					*chr;
	int						buf_charsize;
	int						buf_usedchars;
}							t_buffer;

typedef struct				s_inbuf
{
	unsigned long long		u;
	long long int			s;
	long double				f;
}							t_inbuf;

typedef struct				s_pflags
{
	bool					chr;
	bool					shrt;
	bool					longint;
	bool					llong;
	bool					largel;
	bool					j;
	bool					z;
	int						pound;
	int						plus;
	int						space;
	bool					minus;
	bool					zero;
	int						l_size;
	int						max_size;
	int						min_len;
	int						isneg;
	char					t;
	t_inbuf					*inbuf;
}							t_pflags;

#endif
