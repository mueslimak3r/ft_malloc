/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isnumber.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 22:59:45 by alkozma           #+#    #+#             */
/*   Updated: 2019/11/21 23:00:17 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isnumber(char *str, int len)
{
	int i;
	int ret;

	ret = 0;
	i = 0;
	if (!str)
		return (0);
	while ((str[i] == '+' || str[i] == '-') && i < len)
		i++;
	if (str[i] == 0)
		ret = 1;
	if (ft_atoi(str) != 0)
		ret = 1;
	while (str[i] && i < len)
	{
		if (!(ft_isdigit(str[i])))
			ret = 0;
		i++;
	}
	return (ret);
}
