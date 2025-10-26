#ifndef POOL_H
#define POOL_H

#include <stddef.h>
#include <stdbool.h>

// Chunks that points to free blocks
typedef struct Chunk Chunk;
struct Chunk {
    Chunk *next; // free blocks
};

// Core pool struct
typedef struct Pool {
    void *memory; // track actual memory
    Chunk *free_chunk; // first free chunk
    size_t block_size;
    size_t num_blocks;
} Pool;

// Inititialize pool with fixed size
Pool *pool_init(size_t block_size, size_t num_blocks);
// Allocates a block inside pool
void *pool_alloc(Pool *pool);
// Free block
void pool_free(Pool *pool, void *ptr);
// Free entire pool
void pool_destroy(Pool *pool);

// USAGE:
/*
#include "mem/pool.c"
#include <stdio.h>

int main(void) {
    // create pool for 3 integers
    Pool *pool = pool_init(sizeof(int), 3);

    // allocate some integers
    int *a = (int *)pool_alloc(pool);
    int *b = (int *)pool_alloc(pool);

    // use them
    *a = 10;
    *b = 20;
    printf("a = %d, b = %d\n", *a, *b);

    // free one
    pool_free(pool, a);

    // reuse freed slot
    int *c = (int *)pool_alloc(pool);
    *c = 30;
    printf("c = %d\n", *c);

    // cleanup
    pool_free(pool, b);
    pool_free(pool, c);
    pool_destroy(pool);

    return 0;
}
*/

#endif
