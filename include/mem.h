#ifndef MEM_H
#define MEM_H

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

#endif
