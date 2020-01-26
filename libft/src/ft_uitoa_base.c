/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_uitoa_base.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/17 11:45:09 by calamber          #+#    #+#             */
/*   Updated: 2019/10/25 07:01:46 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int						u_numsize(unsigned long long nbr, int base)
{
	int					size;

	size = 0;
	if (!nbr)
		return (1);
	while (nbr >= (unsigned long long)base)
	{
		nbr /= base;
		size++;
	}
	return (size + 1);
}

char					*ft_uitoa_base(unsigned long long nbr, int base)
{
	char				*res;
	int					index;
	char				*hex;
	int					size;

	hex = "0123456789abcdef";
	size = u_numsize(nbr, base);
	res = (char*)ft_memalloc(sizeof(char) * (size + 1));
	index = size - 1;
	while (nbr >= (unsigned long long)base)
	{
		res[index] = hex[nbr % base];
		nbr /= base;
		index--;
	}
	res[index] = hex[nbr % base];
	return (res);
}
