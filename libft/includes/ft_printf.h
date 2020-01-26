/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_printf.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/17 11:23:09 by calamber          #+#    #+#             */
/*   Updated: 2019/01/13 23:03:31 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "ft_defs.h"

int		ft_printf(const char *format, ...);
void	initbuffer(t_buffer *buffer);
void	initpflags(t_pflags *flags);
size_t	checkpflags(const char *format, size_t pos, t_pflags *flags);
size_t	pparse(va_list list, char type, t_buffer *buffer, t_pflags *flags);
size_t	route_chars(va_list l, char t, t_buffer *b, t_pflags *f);
size_t	usenbr(t_pflags *flags, t_buffer *buffer, char type, int base);
size_t	route_u(va_list l, char t, t_buffer *b, t_pflags *f);
size_t	route_d(va_list list, t_buffer *buffer, t_pflags *flags);
size_t	route_f(va_list list, t_buffer *buffer, t_pflags *flags);
int		ft_savechar(t_buffer *buffer, char s, int repeat);
int		ft_savestr(t_buffer *buffer, char *s, int size);
char	*uitoa_base(unsigned long long nbr, int base);
size_t	get_wchar(t_buffer *buffer, wchar_t wc);
size_t	ft_sizewchar(wchar_t wc);
size_t	justify_chars(t_buffer *b, t_pflags *f, int size);
size_t	justify_d(t_buffer *b, t_pflags *f, int s, int p);
int		ischar(char c);
int		isflag(char c);

#endif
