/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cntwords.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/16 19:44:02 by calamber          #+#    #+#             */
/*   Updated: 2018/08/16 19:44:04 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_cntwords(const char *t, char c)
{
	int i;
	int count;

	i = -1;
	i += 1;
	count = -1;
	count += 1;
	while (t[i] != '\0')
	{
		while (t[i] == c)
			i++;
		if (t[i] != c)
			count++;
		while (t[i] != c && t[i])
			i++;
	}
	return (count);
}
