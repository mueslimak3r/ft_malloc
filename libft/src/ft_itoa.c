/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/04 00:24:45 by calamber          #+#    #+#             */
/*   Updated: 2019/09/21 10:32:21 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_itoa(long long nbr)
{
	int			length;
	long long	sign;
	char		*str;

	sign = nbr;
	length = 1;
	while (sign /= 10)
		length++;
	sign = nbr < 0 ? 1 : 0;
	length = (nbr < 0) ? (length + 1) : (length);
	str = ft_strnew(length);
	if (!str)
		return (NULL);
	if (sign)
		str[0] = '-';
	nbr = (nbr < 0) ? (nbr * -1) : nbr;
	while (--length >= sign)
	{
		str[length] = (nbr >= 10) ? (nbr % 10) + 48 : nbr + 48;
		nbr /= 10;
	}
	return (str);
}
