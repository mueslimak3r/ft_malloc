/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/01 17:40:23 by calamber          #+#    #+#             */
/*   Updated: 2019/07/01 18:41:27 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_radix(char c, unsigned int base)
{
	if (base <= 10)
		return (c >= '0' && c <= '0' + (char)base);
	return ((c >= '0' && c <= '9')
		|| (c >= 'A' && c <= ('A' + (char)base - 10)));
}

int			ft_atoi_base(char *str, const int base)
{
	int		value;
	int		sign;
	char	x;

	value = 0;
	sign = 1;
	if (!str)
		return (0);
	if (base <= 1 || base > 36)
		return (0);
	while (str && *str && ft_isspace(*str))
		str++;
	while (*str == '-' || *str == '+')
		if (*(str++) == '-')
			sign = -1;
	while ((x = ft_toupper(*(str++))) && is_radix(x, base))
		if (x - 'A' >= 0)
			value = value * base + x - 'A' + 10;
		else
			value = value * base + x - '0';
	return (value * sign);
}
