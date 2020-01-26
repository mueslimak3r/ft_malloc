/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_chars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 16:24:26 by calamber          #+#    #+#             */
/*   Updated: 2019/01/13 23:29:41 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

size_t		justify_chars(t_buffer *b, t_pflags *f, int size)
{
	size_t	ret;

	ret = 0;
	if (f->min_len > size)
		ret += ft_savechar(b, ' ', (f->min_len - size));
	return (ret);
}

size_t		parse_chars(char *d, char t, t_buffer *b, t_pflags *f)
{
	size_t	ret;
	int		size;

	ret = 0;
	if (t == 's')
	{
		size = (f->max_size > -1 && f->max_size < (int)ft_strlen(d))
			? f->max_size : (int)ft_strlen(d);
	}
	else
		size = 1;
	ret += !(f->minus) ? (justify_chars(b, f, size)) : 0;
	ret += (t == 's') ? ft_savestr(b, d, size) :
		(ft_savechar(b, *d, 1));
	ret += (f->minus) ? (justify_chars(b, f, size)) : 0;
	return (ret);
}

size_t		parse_wchar(va_list l, t_buffer *b, t_pflags *f, char t)
{
	int		size;
	wchar_t	*array;
	wchar_t	single;
	int		i;

	i = 0;
	size = 0;
	if (t == 'c' && f->longint == true)
	{
		single = (wchar_t)va_arg(l, wint_t);
		get_wchar(b, single);
		size = ft_sizewchar(single);
	}
	else
	{
		array = va_arg(l, wchar_t*);
		while (array[i])
		{
			get_wchar(b, array[i]);
			size += ft_sizewchar(array[i++]);
		}
	}
	return (size);
}

size_t		route_chars(va_list l, char t, t_buffer *b, t_pflags *f)
{
	char	*str;
	char	c;

	if (f->longint || t == 'S' || t == 'C')
		return (parse_wchar(l, b, f, t));
	else if (t == 's')
	{
		str = va_arg(l, char*);
		return (parse_chars(str, t, b, f));
	}
	else if (t == 'c')
	{
		c = (char)va_arg(l, int);
		return (parse_chars(&c, t, b, f));
	}
	return (0);
}
