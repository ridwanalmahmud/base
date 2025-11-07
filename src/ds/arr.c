#include "ds.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define ARRAY_INIT_CAPACITY 16
#define ARRAY_GROWTH_FACTOR 2

static int _arr_grow(Array *arr, size_t min_capacity) {
    size_t new_capacity = arr->capacity ? arr->capacity : ARRAY_INIT_CAPACITY;

    while (new_capacity < min_capacity) {
        if (new_capacity < 1024) {
            new_capacity *= 2;
        } else {
            new_capacity += new_capacity / 2;
        }

        if (new_capacity < arr->capacity) {
            return 0;
        }
    }

    void *new_data = realloc(arr->data, new_capacity * arr->elem_size);
    if (!new_data) {
        return 0;
    }

    arr->data = new_data;
    arr->capacity = new_capacity;
    return 1;
}

Array *arrNew(size_t elem_size, size_t initial_capacity) {
    if (elem_size == 0) {
        return NULL;
    }

    Array *arr = malloc(sizeof(Array));
    if (!arr) {
        return NULL;
    }

    arr->elem_size = elem_size;
    arr->len = 0;
    arr->capacity = initial_capacity > 0 ? initial_capacity : ARRAY_INIT_CAPACITY;
    arr->data = malloc(arr->capacity * elem_size);

    if (!arr->data) {
        free(arr);
        return NULL;
    }

    return arr;
}

Array *arrNewFromData(void *data, size_t elem_size, size_t count) {
    if (!data || elem_size == 0) {
        return NULL;
    }

    Array *arr = arrNew(elem_size, count);
    if (!arr) {
        return NULL;
    }

    memcpy(arr->data, data, count * elem_size);
    arr->len = count;

    return arr;
}

void arrFree(Array *arr) {
    if (!arr) {
        return;
    }

    free(arr->data);
    free(arr);
}

void *arrGet(const Array *arr, size_t index) {
    if (!arr || index >= arr->len) {
        return NULL;
    }
    return (char *)arr->data + (index * arr->elem_size);
}

void arrSet(Array *arr, size_t index, const void *value) {
    if (!arr || !value || index >= arr->len) {
        return;
    }
    memcpy((char *)arr->data + (index * arr->elem_size), value, arr->elem_size);
}

int arrPush(Array *arr, const void *value) {
    if (!arr || !value) {
        return 0;
    }

    if (arr->len >= arr->capacity) {
        if (!_arr_grow(arr, arr->len + 1)) {
            return 0;
        }
    }

    memcpy((char *)arr->data + (arr->len * arr->elem_size), value, arr->elem_size);
    arr->len++;
    return 1;
}

void arrPop(Array *arr, void *out_value) {
    if (!arr || arr->len == 0) {
        return;
    }

    if (out_value) {
        memcpy(out_value, (char *)arr->data + ((arr->len - 1) * arr->elem_size), arr->elem_size);
    }
    arr->len--;
}

void arrClear(Array *arr) {
    if (arr) {
        arr->len = 0;
    }
}

int arrReserve(Array *arr, size_t new_capacity) {
    if (!arr || new_capacity <= arr->capacity) {
        return 1;
    }

    return _arr_grow(arr, new_capacity);
}

int arrShrink(Array *arr) {
    if (!arr || arr->capacity == arr->len) {
        return 1;
    }

    if (arr->len == 0) {
        free(arr->data);
        arr->data = NULL;
        arr->capacity = 0;
        return 1;
    }

    void *new_data = realloc(arr->data, arr->len * arr->elem_size);
    if (!new_data) {
        return 0;
    }

    arr->data = new_data;
    arr->capacity = arr->len;
    return 1;
}

size_t arrLength(const Array *arr) {
    return arr ? arr->len : 0;
}

size_t arrCapacity(const Array *arr) {
    return arr ? arr->capacity : 0;
}

int arrInsert(Array *arr, size_t index, const void *value) {
    if (!arr || !value || index > arr->len) {
        return 0;
    }

    if (index == arr->len) {
        return arrPush(arr, value);
    }

    if (arr->len >= arr->capacity) {
        if (!_arr_grow(arr, arr->len + 1)) {
            return 0;
        }
    }

    // shift elements to make space
    char *base = (char *)arr->data;
    size_t move_size = (arr->len - index) * arr->elem_size;
    memmove(base + ((index + 1) * arr->elem_size), base + (index * arr->elem_size), move_size);

    // insert new element
    memcpy(base + (index * arr->elem_size), value, arr->elem_size);
    arr->len++;

    return 1;
}

void arrRemove(Array *arr, size_t index) {
    if (!arr || index >= arr->len) {
        return;
    }

    if (index == arr->len - 1) {
        arr->len--;
        return;
    }

    // shift elements to fill the gap
    char *base = (char *)arr->data;
    size_t move_size = (arr->len - index - 1) * arr->elem_size;
    memmove(base + (index * arr->elem_size), base + ((index + 1) * arr->elem_size), move_size);

    arr->len--;
}

void arrRemoveRange(Array *arr, size_t start, size_t end) {
    if (!arr || start >= end || end > arr->len) {
        return;
    }

    size_t remove_count = end - start;
    if (end < arr->len) {
        char *base = (char *)arr->data;
        size_t move_size = (arr->len - end) * arr->elem_size;
        memmove(base + (start * arr->elem_size), base + (end * arr->elem_size), move_size);
    }

    arr->len -= remove_count;
}

void *arrFind(const Array *arr, const void *value, int (*compare)(const void *, const void *)) {
    if (!arr || !value || !compare) {
        return NULL;
    }

    char *base = (char *)arr->data;
    for (size_t i = 0; i < arr->len; i++) {
        if (compare(base + (i * arr->elem_size), value) == 0) {
            return base + (i * arr->elem_size);
        }
    }

    return NULL;
}

int arrContains(const Array *arr, const void *value, int (*compare)(const void *, const void *)) {
    return arrFind(arr, value, compare) != NULL;
}

void arrSort(Array *arr, int (*compare)(const void *, const void *)) {
    if (!arr || !compare || arr->len < 2) {
        return;
    }

    qsort(arr->data, arr->len, arr->elem_size, compare);
}

Array *arrSlice(const Array *arr, size_t start, size_t end) {
    if (!arr || start > end || end > arr->len) {
        return NULL;
    }

    if (start == end) {
        return arrNew(arr->elem_size, 0);
    }

    size_t slice_len = end - start;
    Array *slice = arrNew(arr->elem_size, slice_len);
    if (!slice) {
        return NULL;
    }

    char *src_base = (char *)arr->data;
    memcpy(slice->data, src_base + (start * arr->elem_size), slice_len * arr->elem_size);
    slice->len = slice_len;

    return slice;
}

Array *arrDup(const Array *arr) {
    if (!arr) {
        return NULL;
    }

    return arrNewFromData(arr->data, arr->elem_size, arr->len);
}
