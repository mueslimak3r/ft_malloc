/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/30 19:38:09 by calamber          #+#    #+#             */
/*   Updated: 2019/12/13 19:52:21 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static t_gnl_file	*new_gnl_file(int fd)
{
	t_gnl_file	*file;

	file = (t_gnl_file *)ft_memalloc(sizeof(t_gnl_file));
	if (!file)
		return (0);
	file->fd = fd;
	file->buf = 0;
	return (file);
}

static t_list		*get_fd(t_list *head, int fd)
{
	t_gnl_file	*file;

	if (!head)
	{
		file = new_gnl_file(fd);
		head = ft_lstnew(file, sizeof(t_gnl_file));
		if (head && head->content)
			((t_gnl_file *)head->content)->buf = ft_strnew(BUFF_SIZE);
		ft_memdel((void **)&file);
	}
	while (head && head->content)
	{
		if (((t_gnl_file *)head->content)->fd == fd)
			return (head);
		if (!head->next)
		{
			file = new_gnl_file(fd);
			head->next = ft_lstnew(file, sizeof(t_gnl_file));
			if (head->next && head->next->content)
				((t_gnl_file *)head->next->content)->buf = ft_strnew(BUFF_SIZE);
			ft_memdel((void **)&file);
		}
		head = head->next;
	}
	return (0);
}

static char			*stresize(char **buf, int start, size_t size)
{
	char	*new;
	int		baselen;

	baselen = ft_strlen(*buf + start);
	new = ft_strnew(baselen + size);
	if (!new)
		return (0);
	new = ft_strncpy(new, *buf + start, baselen);
	ft_strdel(buf);
	return (new);
}

static int			process_line(t_gnl_file *file, int ret, char **line)
{
	char	*newline_cur;
	int		new_strlen;

	newline_cur = ft_strchr(file->buf, '\n');
	new_strlen = newline_cur - file->buf;
	if (!newline_cur && !ret)
	{
		*line = stresize(&file->buf, 0, 0);
		if (!*line)
			return (-1);
		if (!**line)
			return (0);
		return (1);
	}
	else
		*line = ft_strnew(new_strlen);
	if (!*line)
		return (-1);
	*line = ft_strncpy(*line, file->buf, new_strlen);
	file->buf = stresize(&file->buf, new_strlen + 1, 0);
	if (!*line || !file->buf)
		return (-1);
	return (1);
}

int					get_next_line(const int fd, char **line)
{
	static t_list	*head;
	t_gnl_file		*file;
	int				ret;

	if (read(fd, 0, 0) == -1 || !line)
		return (-1);
	if (!head && !(head = get_fd(0, fd)))
		return (-1);
	if (!(file = get_fd(head, fd)->content))
		return (-1);
	if (!file->buf)
		return (0);
	ret = 1;
	while (!ft_strchr(file->buf, '\n') && ret > 0)
	{
		file->buf = stresize(&file->buf, 0, BUFF_SIZE);
		if (!file->buf)
			return (-1);
		ret = read(fd, (file->buf + ft_strlen(file->buf)), BUFF_SIZE);
		if (ret < 0)
			return (-1);
	}
	return (process_line(file, ret, line));
}
