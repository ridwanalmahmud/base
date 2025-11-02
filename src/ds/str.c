#include "ds.h"
#include <stdlib.h>
#include <string.h>

typedef struct __attribute__((packed)) {
    uint16_t len;
    uint16_t alloc;
    char buf[];
} _StrHdr;

// get header pointer from String
static inline _StrHdr *_str_hdr(String s) {
    return (_StrHdr *)(s - sizeof(_StrHdr));
}

static inline uint16_t _str_len(String s) {
    return _str_hdr(s)->len;
}

String strNewLen(const void *s, size_t len) {
    _StrHdr *hdr;
    String str;

    hdr = malloc(sizeof(_StrHdr) + len + 1);
    if (!hdr)
        return NULL;

    hdr->len = len;
    hdr->alloc = len;

    str = hdr->buf;

    if (s) {
        memcpy(str, s, len);
    }
    str[len] = '\0';

    return str;
}

String strNew(const char *s) {
    if (!s)
        return NULL;
    size_t len = strlen(s);
    return strNewLen(s, len);
}

String strEmpty(void) {
    return strNewLen("", 0);
}

String strDup(const String s) {
    return strNewLen(s, _str_len(s));
}

void strFree(String s) {
    if (!s)
        return;
    free(_str_hdr(s));
}
