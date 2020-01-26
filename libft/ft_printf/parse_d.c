/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_d.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 16:05:32 by calamber          #+#    #+#             */
/*   Updated: 2019/01/13 23:36:17 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

size_t			justify2(t_buffer *b, t_pflags *f, char t, int s)
{
	size_t		ret;

	ret = 0;
	if (f->space && !(f->min_len > s &&
		f->max_size < f->min_len) && (f->t == 'd'))
		ret += ft_savechar(b, ' ', 1);
	ret += (f->plus && t == 'd') ? ft_savechar(b, '+', 1) : 0;
	ret += (f->isneg && t == 'd') ? ft_savechar(b, '-', 1) : 0;
	if (t == 'p' || ((t == 'x' || t == 'X') && f->pound))
		ret += (ft_isupper(t)) ? ft_savestr(b, "0X", -1)
			: ft_savestr(b, "0x", -1);
	if ((f->zero == false) && f->max_size > s)
		ret += ft_savechar(b, '0', (f->max_size - s + f->isneg));
	else if (f->zero == true && (f->max_size > s
				|| (f->min_len > s && !f->minus)))
		ret += (f->max_size > s - f->plus) ? ft_savechar(b, '0',
				(f->max_size - s - f->plus)) :
					ft_savechar(b, '0', (f->min_len - s - f->plus));
	return (ret);
}

size_t			justify_d(t_buffer *b, t_pflags *f, int s, int p)
{
	size_t		ret;

	ret = 0;
	if (p == 0)
	{
		if (f->min_len > s && f->min_len > f->max_size &&
			f->max_size > s && f->minus == false)
			ret += ft_savechar(b, ' ', (f->min_len - f->max_size
				- f->plus - f->isneg));
		else if (f->min_len > s && f->minus == false
			&& (!((f->max_size > s) || (f->zero))))
			ret += ft_savechar(b, ' ', (f->min_len - s));
		ret += justify2(b, f, f->t, s);
	}
	else if (p == 1)
	{
		if (f->min_len > s && f->min_len > f->max_size
			&& f->max_size > 0 && f->minus == true)
			ret += ft_savechar(b, ' ', (f->min_len -
				f->max_size - f->plus - f->isneg));
		else if (f->min_len > s && (f->min_len >
			f->max_size) && f->minus == true)
			ret += ft_savechar(b, ' ', (f->min_len - s) - f->plus);
	}
	return (ret);
}

size_t			parse_d(t_buffer *b, t_pflags *f)
{
	int			size;
	char		*str;
	int			ret;

	ret = 0;
	str = ft_itoa(f->inbuf->s);
	f->space = (f->space && !f->plus && f->inbuf->s > 0) ? 1 : 0;
	size = ((f->l_size && f->max_size == 0) && f->inbuf->s == 0) ?
		0 : (int)ft_strlen(str);
	f->plus = (f->inbuf->s >= 0 && f->plus) ? 1 : 0;
	f->t = 'd';
	ret += justify_d(b, f, size, 0);
	ret += ((f->l_size && f->max_size == 0) && f->inbuf->s == 0) ?
		0 : ft_savestr(b, str + f->isneg, (int)ft_strlen(str + f->isneg));
	ret += justify_d(b, f, size, 1);
	f->inbuf->s = 0;
	free(str);
	return (ret);
}

size_t			route_d(va_list list, t_buffer *buffer, t_pflags *flags)
{
	t_inbuf		buf;

	if (flags->longint == true)
		buf.s = va_arg(list, long);
	else if (flags->llong == true)
		buf.s = va_arg(list, long long);
	else if (flags->j == true)
		buf.s = va_arg(list, intmax_t);
	else if (flags->z == true)
		buf.s = va_arg(list, ssize_t);
	else
		buf.s = va_arg(list, int);
	flags->isneg = (buf.s < 0) ? 1 : 0;
	flags->plus = (flags->isneg && flags->plus) ? 0 : flags->plus;
	flags->inbuf = &buf;
	return (parse_d(buffer, flags));
}
