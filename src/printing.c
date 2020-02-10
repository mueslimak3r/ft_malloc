/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 01:09:05 by calamber          #+#    #+#             */
/*   Updated: 2020/02/07 01:09:06 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc_deps.h"

void	print_nb(unsigned long nb, int byte_count, int base, int fd)
{
	static char	*hex_table = "01234567890ABCDEF";
	char		printed_chars[byte_count + 1];
	int			pos;

	pos = byte_count - 1;
	printed_chars[byte_count] = '\0';
	if (base == 16)
	{
		printed_chars[0] = '0';
		printed_chars[1] = 'x';
	}
	if (nb == 0)
		printed_chars[pos] = '0';
	else
	{
		while (nb > 0)
		{
			printed_chars[pos] = (base == 16) ?
				(hex_table[(long)(nb % base)]) : ((long)(nb % base) + '0');
			nb /= base;
			pos--;
		}
	}
	ft_putstr_fd(printed_chars, fd);
}

int		count_bytes(unsigned long nb, int base)
{
	int	ret;

	ret = nb == 0 ? 1 : 0;
	while (nb > 0)
	{
		nb /= base;
		ret++;
	}
	return (ret);
}

void	ft_putnbr_u_base_fd(unsigned long nb, int base, int fd)
{
	int byte_count;

	byte_count = count_bytes(nb, base);
	print_nb(nb, byte_count + (base == 16 ? 2 : 0), base, fd);
}

int		handle_single(int fd, char **fmt, va_list *vargs)
{
	if (!*fmt)
		return (0);
	if (**fmt == '%' && *(*fmt + 1) == 's')
		ft_putstr_fd(va_arg(*vargs, char*), fd);
	else if (**fmt == '%' && *(*fmt + 1) == 'd')
		ft_putnbr_u_base_fd(va_arg(*vargs, int), 10, fd);
	else if (**fmt == '%' && *(*fmt + 1) && *(*fmt + 1) == 'p')
		ft_putnbr_u_base_fd(va_arg(*vargs, long), 16, fd);
	else
		return (0);
	*fmt += 2;
	return (1);
}

int		ft_printf_fd(int fd, char *fmt, ...)
{
	va_list	vargs;

	va_start(vargs, (char*)fmt);
	while (*fmt)
	{
		if (handle_single(fd, &fmt, &vargs))
			continue ;
		else if (*fmt == '%' && *(fmt + 1) && *(fmt + 2) &&
			ft_strncmp(fmt + 1, "lu", 2) == 0)
		{
			ft_putnbr_u_base_fd(va_arg(vargs, long), 10, fd);
			fmt += 3;
		}
		else
		{
			ft_putchar_fd(*fmt, fd);
			fmt += 1;
		}
	}
	return (1);
}
