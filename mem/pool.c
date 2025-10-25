#include "../base.h"
#include "pool.h"
#include <stdlib.h>

Pool *pool_init(size_t block_size, size_t num_blocks) {
    if (block_size == 0 || num_blocks == 0) {
        log_err("Invalid pool initialization parameters");
    }

    // allocate core pool
    Pool *pool = malloc(sizeof(Pool));
    if (!pool) {
        log_err("Pool allocation failed");
        return nullptr;
    }

    // allocate blocks of memory
    size_t actual_block_size =
        (block_size < sizeof(Chunk)) ? sizeof(Chunk) : block_size;
    pool->memory = calloc(num_blocks, actual_block_size);
    if (!pool->memory) {
        log_err("Chunk allocation failed");
        free(pool);
        return nullptr;
    }

    pool->block_size = actual_block_size;
    pool->num_blocks = num_blocks;
    pool->free_chunk = nullptr;

    // build free list (LIFO)
    char *mem = (char *)pool->memory; // (char *) for pointer arithmetic
    for (size_t i = 0; i < num_blocks; i++) {
        Chunk *chunk = (Chunk *)(mem + (i * actual_block_size));
        chunk->next = pool->free_chunk;
        pool->free_chunk = chunk;
    }

    return pool;
}

void *pool_alloc(Pool *pool) {
    if (!pool || !pool->free_chunk || pool->block_size == 0) {
        log_err("Invalid pool or uninitialized pool");
        return nullptr;
    }

    // remove first chunk from free list
    Chunk *chunk = pool->free_chunk;
    pool->free_chunk = chunk->next;

    return chunk;
}

void pool_free(Pool *pool, void *ptr) {
    if (!pool || !ptr) {
        log_err("Invalid pool or null pointer in pool_free");
        return;
    }

    // validate pointer is within pool memory
    char *start = (char *)pool->memory;
    char *end = start + (pool->block_size * pool->num_blocks);
    char *block_ptr = (char *)ptr;

    if (block_ptr < start || block_ptr >= end) {
        log_err("Pointer not from this pool");
        return;
    }

    // verify alignment
    if ((block_ptr - start) % pool->block_size != 0) {
        log_err("Misaligned pointer");
        return;
    }

    // add block back to free list
    Chunk *chunk = (Chunk *)ptr;
    chunk->next = pool->free_chunk;
    pool->free_chunk = chunk;
}

void pool_destroy(Pool *pool) {
    if (!pool) {
        log_err("Attempted to destroy null pool");
        return;
    }

    free(pool->memory);
    free(pool);
}
