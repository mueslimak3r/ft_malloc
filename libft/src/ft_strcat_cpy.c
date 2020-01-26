/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat_cpy.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 22:55:59 by alkozma           #+#    #+#             */
/*   Updated: 2019/11/21 22:56:54 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_strcat_cpy(char *dst, char const *s1, char const *s2, int size)
{
	int		i;
	int		j;

	if (!s1 && !s2)
		return ;
	i = 0;
	j = 0;
	if (dst == NULL)
		return ;
	while (s1 && s1[i] != '\0' && i < size)
	{
		dst[i] = s1[i];
		i++;
	}
	while (s2 && s2[j] != '\0' && i + j < size)
	{
		dst[i + j] = s2[j];
		j++;
	}
	dst[i + j] = '\0';
}
