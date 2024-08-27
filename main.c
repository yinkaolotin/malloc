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
t_block find_blk(t_block *last, size_t s)
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
t_block extend_heap(t_block *last, size_t s)
{
        // empty heap
        t_block new;

        new = sbrk(0); /* just to make ensure prev breakpoint(start) is pointed to */
        if (sbrk(BLOCK_SIZE + s) == (void*)-1)
        {
                return NULL;
        }

        new->size = s;
        new->free = 0;

        if (last != NULL)
        {
                last->next = new;
        }
        return new;
}


/*
 * splblk- splits a free block if wide enough to hold new allocation plus a new block(atleast BLOCKSIZE+4).
 * @b: free block to be split
 * @s: size of the new allocation
*/
void split_blk(t_block *b, size_t s)
{
        t_block free;

        size_t old_b_size = b->size;
        // the new free block is moved further after
        free = b->data + s;
        free->size = old_b_size - s - BLOCK_SIZE;
        free->next = b->next;

        b->size = s;
        b->next = free;
        b->free = 1;
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;


        return 0;
}

