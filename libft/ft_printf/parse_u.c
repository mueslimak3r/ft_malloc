/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_u.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 15:45:37 by calamber          #+#    #+#             */
/*   Updated: 2019/10/25 07:00:27 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

size_t			usenbr(t_pflags *f, t_buffer *buffer, char t, int base)
{
	size_t		ret;
	char		*str;
	int			size;

	ret = 0;
	str = ft_uitoa_base(f->inbuf->u, base);
	size = (int)ft_strlen(str);
	size = (f->max_size <= 0 && f->inbuf->u == 0) ? 0 : size;
	f->pound = (f->pound && (((t == 'x' || t == 'X') && f->inbuf->u != 0) ||
				(t == 'o' || t == 'O'))) ? f->pound : false;
	size += (t == 'p' || ((t == 'x' || t == 'X') && f->pound)) ? 2 : 0;
	size += ((t == 'o' || t == 'O') && f->pound) ? 1 : 0;
	ret += justify_d(buffer, f, size, 0);
	str = (ft_isupper(t)) ? ft_strcase(str, 'a') : str;
	ret += ((t == 'o' || t == 'O') && f->pound) ?
		ft_savechar(buffer, '0', 1) : 0;
	ret += ((f->max_size == 0 || (f->l_size && f->max_size == -1))
			&& f->inbuf->u == 0) ? 0 : ft_savestr(buffer, str, -1);
	ret += justify_d(buffer, f, size, 1);
	free(str);
	return (ret);
}

size_t			parse_u(char type, t_buffer *buffer, t_pflags *flags)
{
	size_t		ret;

	ret = 0;
	if (type == 'o')
		ret = usenbr(flags, buffer, type, 8);
	else if (type == 'u')
		ret = usenbr(flags, buffer, type, 10);
	else if (type == 'x' || type == 'X' || type == 'p')
		ret = usenbr(flags, buffer, type, 16);
	return (ret);
}

size_t			route_u(va_list l, char t, t_buffer *b, t_pflags *f)
{
	t_inbuf		buf;

	if (t == 'p')
		buf.u = va_arg(l, unsigned long long);
	else if (f->shrt == true || f->chr == true)
		buf.u = (unsigned long long)va_arg(l, int);
	else if (f->longint == true)
		buf.u = (unsigned long long)va_arg(l, unsigned long);
	else if (f->llong == true)
		buf.u = va_arg(l, unsigned long long);
	else if (f->j == true)
		buf.u = (unsigned long long)va_arg(l, uintmax_t);
	else if (f->z == true)
		buf.u = (unsigned long long)va_arg(l, size_t);
	else
		buf.u = (unsigned long long)va_arg(l, unsigned int);
	f->inbuf = &buf;
	f->t = t;
	return (parse_u(t, b, f));
}
