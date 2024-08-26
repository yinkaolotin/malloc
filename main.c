#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define ALIGN4(x) (((((x)-1)>>2)<<2)+4)
#define BLOCK_SIZE 12

typedef struct s_block *t_block;
struct s_block {
        size_t      size;
        t_block     next;
        int         free;
        char        data[1]; /* mimic dynamic-sized */
};

/**
 * fblk - finds a free block that may be allocated
 * @last: last allocated block, kept to easily extend the heap if no suitable block is found
 * @s: desired size to be allocated
 *
 * Return: a suitable block(>= desired size). otherwise NULL (if heap is empty)
 */
t_block fblk(t_block *last, size_t s)
{
        t_block b;

        b = base;
        while(b != NULL && !(b->free && b->size >= s))
        {
                *last = b;
                b = b->next;
        }
        return b;
}

/**
 * extend_heap - extends the breakpoint of the heap
 * @last: last allocated block, to extend the heap
 * 
*/
t_block extend_heap(t_block *last)
{
        // empty heap
        t_block new;

        new = sbrk(0); /* just to make ensure prev breakpoint(start) is pointed to */
        if (sbrk(BLOCK_SIZE + s) == (void*)-1)
        {
                return NULL;
        }

        new->size = s;
        new->free = 1;
        return new;
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;


        return 0;
}

