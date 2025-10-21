#ifndef POOL_H
#define POOL_H

#include <stddef.h>
#include <stdbool.h>

typedef struct Chunk Chunk;
struct Chunk {
    Chunk *next;
};

typedef struct Pool {
    Chunk *free_list;
    size_t chunk_size;
    size_t chunks_per_block;
} Pool;

bool pool_init(Pool *pool, size_t chunk_size, size_t chunks_per_block);
void *pool_alloc(Pool *pool);
Chunk *block_alloc(Pool *pool);
void pool_free(Pool *pool, void *ptr);
void pool_destroy(Pool *pool);

#endif
