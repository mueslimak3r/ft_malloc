/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unit_test3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 04:25:01 by calamber          #+#    #+#             */
/*   Updated: 2020/02/10 04:25:03 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "unit_tests.h"

int	unit_test3(void)
{
	printf("starting\n");
	if (malloc(0))
	{
		;
	}
	count_blocks();
	void *tiny;
	tiny = malloc(TINY / 2);
	printf("____________________________________________________________\n");
	printf("mallocd %d bytes from TINY zone at %p\n", TINY / 2, tiny);
	show_alloc_mem();
	count_blocks();
	tiny = realloc(tiny, TINY);
	printf("reallocd1: %p\n", tiny);
	show_alloc_mem();
	count_blocks();
	printf("____________________________________________________________\n");

	tiny = realloc(tiny, SMALL);
	printf("\n____________________________________________________________\n");
	printf("reallocd2: %p\n", tiny);
	show_alloc_mem();
	count_blocks();
	printf("____________________________________________________________\n");
	return (0);
}
