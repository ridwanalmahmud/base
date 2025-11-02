#include "mem.h"
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
