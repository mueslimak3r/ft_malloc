/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/18 15:59:34 by calamber          #+#    #+#             */
/*   Updated: 2018/05/03 23:46:05 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(char *src)
{
	char	*p;
	int		i;

	i = 0;
	while (src[i] != '\0')
		i++;
	if (!(p = (char *)ft_memalloc(sizeof(char) * (i + 1))))
		return (0);
	i = -1;
	while (src[++i])
		p[i] = src[i];
	return (p);
}
