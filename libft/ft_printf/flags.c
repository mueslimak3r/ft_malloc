/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 16:01:47 by calamber          #+#    #+#             */
/*   Updated: 2018/11/30 16:04:39 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void			initpflags(t_pflags *flags)
{
	flags->chr = false;
	flags->shrt = false;
	flags->longint = false;
	flags->llong = false;
	flags->largel = false;
	flags->j = false;
	flags->z = false;
	flags->pound = 0;
	flags->zero = false;
	flags->plus = 0;
	flags->space = 0;
	flags->minus = false;
	flags->min_len = -1;
	flags->l_size = 0;
	flags->max_size = -1;
	flags->inbuf = NULL;
	flags->isneg = 0;
	flags->t = 0;
}

size_t			check_size(int index, const char *format, t_pflags *flags)
{
	flags->l_size = 1;
	flags->max_size = 0;
	while ((format[index] >= '0') && (format[index] <= '9'))
	{
		flags->max_size *= 10;
		flags->max_size += (format[index] - '0');
		index++;
	}
	return (index);
}

size_t			check_plusminus(size_t i, const char *f, t_pflags *fl)
{
	if (f[i] == '-')
		fl->minus = true;
	else if (f[i] == '.')
		i = check_size(++i, f, fl);
	else if (f[i] == '0' && !(ft_isdigit(f[i - 1])))
		fl->zero = true;
	if ((f[i] >= '0') && (f[i] <= '9'))
	{
		while ((f[i] >= '0') && (f[i] <= '9'))
		{
			fl->min_len = (fl->min_len > -1) ? fl->min_len : 0;
			fl->min_len *= 10;
			fl->min_len += (f[i] - '0');
			i++;
		}
		i--;
	}
	if (f[i] == '.')
		i = check_size(++i, f, fl);
	return (i);
}

size_t			checkpflags(const char *format, size_t index, t_pflags *flags)
{
	initpflags(flags);
	while (format[index] && (isflag(format[index]) == 0))
	{
		if (format[index] == '#')
			flags->pound = 2;
		else if (format[index] == ' ')
			flags->space = 1;
		else if (format[index] == '+')
			flags->plus = 1;
		else if (format[index] == 'L')
			flags->largel = true;
		else if (format[index] == 'j')
			flags->j = true;
		else if (format[index] == 'z')
			flags->z = true;
		else if ((format[index] == '-') ||
				format[index] == '.' || ft_isdigit(format[index]))
			index = check_plusminus(index, format, flags);
		else if (format[index] == 'l' && format[index + 1] != 'l')
			flags->longint = true;
		if (!(isflag(format[index])))
			index++;
	}
	return (index);
}
