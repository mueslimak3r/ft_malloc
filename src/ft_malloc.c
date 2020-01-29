#include "ft_malloc.h"

int test(void)
{
    return (0);
}

void    ft_free(void *ptr);
{
    ;
}

t_header    *find_free_block(t_header **last, size_t size)
{
    
}

void        *ft_malloc(size_t size)
{
    t_header *block;
    t_header *last;

    if (size <= 0)
        return NULL;
    if (!g_data.freep)
    {
        block = request_space(NULL, size);
        if (!block)
            return NULL;
        g_data.freep = block;
    }
    else
    {
        last = g_data.freep;
        block = find_free_block(&last, size);
        if (!block)
        {
            block = request_space(last, size);
            if (!block)
                return NULL;
        }
        else
            block->flags = 1;
    }
    return(block + 1);
}