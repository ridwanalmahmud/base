#include "str.h"

// sDS-like header types
#define STR_TYPE_5 0
#define STR_TYPE_8 1
#define STR_TYPE_16 2
#define STR_TYPE_32 3
#define STR_TYPE_64 4
#define STR_TYPE_MASK 7
#define STR_TYPE_BITS 3

#define STR_HDR_VAR(T, s) struct strhdr##T *sh = (void *)((s) - sizeof(struct strhdr##T))
#define STR_HDR(T, s) ((struct strhdr##T *)((s) - sizeof(struct strhdr##T)))
#define STR_TYPE_5_LEN(f) ((f) >> STR_TYPE_BITS)
