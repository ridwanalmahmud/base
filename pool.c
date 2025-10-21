#include "base.h"
#include "pool.h"
#include <stdlib.h>

bool pool_init(Pool *pool, size_t chunk_size, size_t chunks_per_block) {
    if (!pool || chunk_size == 0 || chunks_per_block == 0) {
        log_err("Invalid pool initialization parameters");
        return false;
    }

    if (chunk_size < sizeof(Chunk)) {
        log_info(
            "Chunk size increased from %zu to %zu", chunk_size, sizeof(Chunk));
        chunk_size = sizeof(Chunk);
    }

    pool->free_list = nullptr;
    pool->chunk_size = chunk_size;
    pool->chunks_per_block = chunks_per_block;

    log_info("Pool initialized: chunk_size=%zu, chunks_per_block=%zu",
             chunk_size,
             chunks_per_block);
    return true;
}

void *pool_alloc(Pool *pool) {
    if (!pool || pool->chunk_size == 0) {
        log_err("Invalid pool or uninitialized pool");
        return nullptr;
    }

    if (pool->free_list == nullptr) {
        log_info("Free list empty, allocating new block");
        Chunk *block = block_alloc(pool);
        if (block == nullptr) {
            log_err("Failed to allocate new block");
            return nullptr;
        }
        pool->free_list = block;
    }

    Chunk *chunk = pool->free_list;
    pool->free_list = chunk->next;

    log_info("Allocated chunk at %p", (void *)chunk);
    return chunk;
}

Chunk *block_alloc(Pool *pool) {
    size_t block_size = pool->chunk_size * pool->chunks_per_block;
    log_info("Allocating block of %zu bytes (%zu chunks)",
             block_size,
             pool->chunks_per_block);

    Chunk *block = (Chunk *)malloc(block_size);
    if (block == nullptr) {
        log_err("malloc failed for block of %zu bytes", block_size);
        return nullptr;
    }

    Chunk *chunk = block;
    for (size_t i = 0; i < pool->chunks_per_block - 1; ++i) {
        Chunk *next_chunk = (Chunk *)((char *)chunk + pool->chunk_size);
        chunk->next = next_chunk;
        chunk = next_chunk;
    }
    chunk->next = nullptr;

    log_info("Block allocated successfully at %p", (void *)block);
    return block;
}

void pool_free(Pool *pool, void *ptr) {
    if (!pool || !ptr) {
        log_err("Invalid pool or null pointer in pool_free");
        return;
    }

    Chunk *chunk = (Chunk *)ptr;
    chunk->next = pool->free_list;
    pool->free_list = chunk;

    log_info("Freed chunk at %p back to pool", ptr);
}

void pool_destroy(Pool *pool) {
    if (!pool) {
        log_err("Attempted to destroy null pool");
        return;
    }

    log_info("Destroying pool (chunk_size=%zu, chunks_per_block=%zu)",
             pool->chunk_size,
             pool->chunks_per_block);

    pool->free_list = nullptr;
    pool->chunk_size = 0;
    pool->chunks_per_block = 0;
}
