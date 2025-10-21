#include "base.h"
#include "pool.h"
#include <stdlib.h>

bool pool_init(Pool *pool, size_t chunk_size, size_t chunks_per_block) {
    if (!pool || chunk_size == 0 || chunks_per_block == 0) {
        return false;
    }

    if (chunk_size < sizeof(Chunk)) {
        chunk_size = sizeof(Chunk);
    }

    pool->free_list = nullptr;
    pool->chunk_size = chunk_size;
    pool->chunks_per_block = chunks_per_block;

    return true;
}

void *pool_alloc(Pool *pool) {
    if (!pool || pool->chunk_size == 0) {
        return nullptr;
    }

    if (pool->free_list == nullptr) {
        Chunk *block = block_alloc(pool);
        if (block == nullptr) {
            return nullptr;
        }
        pool->free_list = block;
    }

    Chunk *chunk = pool->free_list;
    pool->free_list = chunk->next;

    return chunk;
}

Chunk *block_alloc(Pool *pool) {
    size_t block_size = pool->chunk_size * pool->chunks_per_block;
    Chunk *block = (Chunk *)malloc(block_size);
    if (block == nullptr) {
        return nullptr;
    }

    Chunk *chunk = block;
    for (size_t i = 0; i < pool->chunks_per_block - 1; ++i) {
        Chunk *next_chunk = (Chunk *)((char *)chunk + pool->chunk_size);
        chunk->next = next_chunk;
        chunk = next_chunk;
    }
    chunk->next = nullptr;

    return block;
}

void pool_free(Pool *pool, void *ptr) {
    if (!pool || !ptr) {
        return;
    }

    Chunk *chunk = (Chunk *)ptr;
    chunk->next = pool->free_list;
    pool->free_list = chunk;
}

void pool_destroy(Pool *pool) {
    if (!pool)
        return;

    pool->free_list = nullptr;
    pool->chunk_size = 0;
    pool->chunks_per_block = 0;
}
