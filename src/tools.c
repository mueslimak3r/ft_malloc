/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 13:55:48 by calamber          #+#    #+#             */
/*   Updated: 2020/02/15 13:55:49 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc_deps.h"

void				ft_putstr_fd(char const *s, int fd)
{
	size_t	i;

	i = 0;
	if (!s)
		return ;
	while (s[i])
		i++;
	write(fd, s, i);
}

static int			find_in_bucket(t_header *block_ptr, t_header *bucket)
{
	while (bucket)
	{
		if (bucket == block_ptr)
			return (1);
		bucket = bucket->next;
	}
	return (0);
}

int					malloc_check_if_valid(t_header *block_ptr)
{
	int	ret;

	ret = 0;
	if (!malloc_check_init())
		return (0);
	if ((find_in_bucket(block_ptr, g_data.tiny)) ||
	(find_in_bucket(block_ptr, g_data.small)) ||
	(find_in_bucket(block_ptr, g_data.large)))
		ret = 1;
	return (ret);
}

void				*malloc(size_t size)
{
	return (ft_malloc(size));
}
