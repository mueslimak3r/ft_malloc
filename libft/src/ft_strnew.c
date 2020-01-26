/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/03 23:51:29 by calamber          #+#    #+#             */
/*   Updated: 2018/05/03 23:53:33 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnew(size_t size)
{
	char *str;

	str = ft_memalloc(size + 1);
	if (str == NULL)
		return (0);
	else
	{
		ft_bzero(str, size + 1);
		return (str);
	}
}
