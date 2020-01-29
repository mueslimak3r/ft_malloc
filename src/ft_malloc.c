#include "ft_malloc.h"

int test(void)
{
    return (0);
}
t_header    *find_free_block(t_header **last, size_t size)
{
    t_header *current = g_data.freep;
    while (current && !(current->flags & 0x1 && current->size >= size))
    {
        *last = current;
        current = current->next;
    }
  return current;
}

void    ft_free(void *ptr)
{
    t_header *header = (t_header*)ptr;

}

t_header    *request_space(t_header *last, size_t size)
{
    t_header *block;
    block = sbrk(0);
    void *request = sbrk(size + META_SIZE);
    if (request == (void*) -1)
    {
        return NULL;
    }
    if (last)
    {
        last->next = block;
    }
    block->size = size;
    block->next = NULL;
    block->flags ^= 0x1;
    return block;
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
            block->flags &= 0x1;
    }
    return(block + 1);
}