/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 03:05:57 by alkozma           #+#    #+#             */
/*   Updated: 2019/04/27 07:14:44 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_realloc(void *thing, size_t orig_size, size_t size)
{
	void	*ret;

	if (!(ret = (void*)ft_memalloc(size)))
		return (NULL);
	if (!thing)
		return (ret);
	ft_memcpy(ret, thing, orig_size);
	free(thing);
	thing = NULL;
	return (ret);
}
