/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_wchar.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 16:07:12 by calamber          #+#    #+#             */
/*   Updated: 2018/11/30 16:07:42 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	ft_three_bits(char *tab, int *wc)
{
	tab[0] = (*wc >> 12) + 0xE0;
	tab[1] = ((*wc >> 6) & 0x3F) + 0x80;
	tab[2] = (*wc & 0x3F) + 0x80;
}

static void	ft_four_bits(char *tab, int *wc)
{
	tab[0] = (*wc >> 18) + 0xF0;
	tab[1] = ((*wc >> 12) & 0x3F) + 0x80;
	tab[2] = ((*wc >> 6) & 0x3F) + 0x80;
	tab[3] = (*wc & 0x3F) + 0x80;
}

size_t		ft_sizewchar(wchar_t wc)
{
	size_t len;

	len = 0;
	if ((int)wc >= 0 && (int)wc <= 0x7F)
		len = 1;
	else if ((int)wc <= 0x7FF)
		len = 2;
	else if ((int)wc <= 0xFFFF)
		len = 3;
	else if ((int)wc <= 0x10FFFF)
		len = 4;
	return (len);
}

size_t		get_wchar(t_buffer *buffer, wchar_t wc)
{
	char	tab[5];
	int		nb;

	nb = (int)wc;
	ft_bzero(tab, 5);
	if ((int)wc < 0 || (((int)wc > 0xD7FF && (int)wc < 0xE000))
	|| ((int)wc > 0x10FFFF))
		return (-1);
	else if ((int)wc == 0)
		tab[0] = (int)wc + 1;
	else if ((int)wc <= 0x7F)
		tab[0] = (int)wc;
	else if ((int)wc <= 0x7FF)
	{
		tab[0] = ((int)wc >> 6) + 0xC0;
		tab[1] = ((int)wc & 0x3F) + 0x80;
	}
	else if ((int)wc <= 0xFFFF)
		ft_three_bits(tab, &nb);
	else if ((int)wc <= 0x10FFFF)
		ft_four_bits(tab, &nb);
	ft_savestr(buffer, tab, -1);
	return (ft_strlen(tab));
}
