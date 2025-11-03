#ifndef MEM_H
#define MEM_H

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Chunks that points to free blocks
typedef struct Chunk Chunk;
struct Chunk {
    Chunk *next; // free blocks
};

typedef struct Pool {
    void *memory; // track actual memory
    Chunk *free_chunk; // first free chunk
    size_t block_size;
    size_t num_blocks;
} Pool;

Pool *pool_init(size_t block_size, size_t num_blocks);
void *pool_alloc(Pool *pool);
void pool_free(Pool *pool, void *ptr);
void pool_destroy(Pool *pool);

#ifdef __cplusplus
}
#endif

#endif
