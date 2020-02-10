/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 02:47:55 by calamber          #+#    #+#             */
/*   Updated: 2020/02/10 02:47:56 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MALLOC_H
# define FT_MALLOC_H

# include <stdio.h>
# include <unistd.h>

void		free(void *ptr);
void		*malloc(size_t size);
void		*realloc(void *ptr, size_t size);

void		show_alloc_mem();
void		show_free_mem();
void		count_blocks();

#endif
