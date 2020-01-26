/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcase.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/04 22:44:43 by calamber          #+#    #+#             */
/*   Updated: 2018/12/04 22:45:19 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strcase(char *str, char format)
{
	char	*ret;

	if (!str)
		return (NULL);
	ret = str;
	while (*str)
	{
		if (format >= 'a' && format <= 'z')
			*str = ft_toupper(*str);
		else
			*str = ft_tolower(*str);
		str++;
	}
	return (ret);
}
