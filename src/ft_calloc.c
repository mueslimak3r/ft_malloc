#include "ft_malloc_deps.h"

void	*ft_calloc(size_t count, size_t size)
{
	char	*new;
	int		new_size;
	int		i;

	ft_putstr("CALLOC\n");
	if (!size || !count)
		return (NULL);
	new_size = size * count;
	new = ft_malloc(new_size);
	if (!new)
		return (NULL);
	i = 0;
	while (i < new_size)
	{
		new[i] = 0;
		i++;
	}
	return (new);
}

void	*calloc(size_t count, size_t size)
{
	return (ft_calloc(count, size));
}
