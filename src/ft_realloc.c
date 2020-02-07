#include "ft_malloc_deps.h"

void		*ft_realloc(void *ptr, size_t size)
{
	if (!ptr || size == 0)
		return (NULL);
	if (!g_initialized)
		ft_malloc_init();
	t_header    *block_ptr = (t_header*)ptr - 1;
	if (size <= block_ptr->size * g_data.meta_size)
		return (ptr);
	void	*new_ptr = ft_malloc(size);
	if (!new_ptr)
		return (NULL);
	ft_memcpy(new_ptr, ptr, block_ptr->size * g_data.meta_size);
	ft_free(ptr);
	return (new_ptr);
}

void		*realloc(void *ptr, size_t size)
{
	return (ft_realloc(ptr, size));
}
