/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cntdigit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 22:58:13 by alkozma           #+#    #+#             */
/*   Updated: 2019/11/21 22:59:38 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_cntdigit(long n, int base)
{
	int	counter;

	counter = 0;
	if (n < 0)
	{
		if (base == 10)
			counter += 1;
		n *= -1;
	}
	if (n == 0)
		counter++;
	while (n > 0)
	{
		n /= 10;
		counter++;
	}
	return (counter);
}
