#include "ds.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

typedef struct {
    size_t len;
    size_t alloc;
} _StrHdr;

#define STR_INIT_ALLOC 16
#define STR_PREALLOC 1024

// get header pointer from String
static inline _StrHdr *_str_hdr(String s) {
    return ((_StrHdr *)s - 1);
}

// get available space
static inline size_t _str_avail(const String s) {
    return (_str_hdr(s)->alloc - _str_hdr(s)->len);
}

static inline size_t _str_len(const String s) {
    return _str_hdr(s)->len;
}

static inline size_t _str_alloc(const String s) {
    return _str_hdr(s)->alloc;
}

String strNewLen(const void *s, size_t len) {
    _StrHdr *hdr = malloc(sizeof(_StrHdr) + len + 1);
    if (!hdr)
        return NULL;

    hdr->len = len;
    hdr->alloc = len;

    // string data starts right after the header
    String str = (String)(hdr + 1);

    if (s && len > 0) {
        memcpy(str, s, len);
    }
    str[len] = '\0';

    return str;
}

String strNew(const char *s) {
    if (!s)
        return strEmpty();
    size_t len = strlen(s);
    return strNewLen(s, len);
}

String strEmpty(void) {
    return strNewLen("", 0);
}

String strDup(const String s) {
    if (!s)
        return strEmpty();
    return strNewLen(s, _str_len(s));
}

void strFree(String s) {
    if (!s)
        return;

    // get header by moving back one header size
    _StrHdr *hdr = (_StrHdr *)s - 1;
    free(hdr);
}

String strGrow(String s, size_t addlen) {
    if (!s)
        return NULL;

    size_t avail = _str_avail(s);
    if (avail >= addlen)
        return s;

    _StrHdr *old_hdr = _str_hdr(s);
    size_t current_alloc = old_hdr->alloc;
    size_t new_alloc = current_alloc + addlen;

    // Optimized growth calculation inlined
    if (new_alloc < STR_PREALLOC) {
        new_alloc = new_alloc * 2; // Double for small strings
    } else {
        new_alloc = new_alloc + (new_alloc / 2); // 1.5x for larger strings
    }

    // Align to 8-byte boundary for better performance
    new_alloc = (new_alloc + 7) & ~7;

    _StrHdr *new_hdr = realloc(old_hdr, sizeof(_StrHdr) + new_alloc + 1);
    if (!new_hdr)
        return NULL;

    new_hdr->alloc = new_alloc;
    return (String)(new_hdr + 1);
}

String strTrim(String s) {
    if (!s)
        return NULL;

    _StrHdr *old_hdr = _str_hdr(s);
    size_t avail = _str_avail(s);

    if (avail == 0) {
        return s;
    }

    _StrHdr *new_hdr = realloc(old_hdr, sizeof(_StrHdr) + _str_len(s) + 1);
    if (!new_hdr)
        return NULL;

    new_hdr->alloc = _str_len(s);

    return (String)(new_hdr + 1);
}

void strClear(String s) {
    if (!s)
        return;

    _StrHdr *hdr = _str_hdr(s);
    hdr->len = 0;
    s[0] = '\0';
}

size_t strLen(const String s) {
    return _str_len(s);
}

void strSetLen(String s, size_t newlen) {
    if (!s)
        return;

    _StrHdr *hdr = _str_hdr(s);
    if (newlen <= _str_alloc(s)) {
        hdr->len = newlen;
        s[newlen] = '\0';
    }
}

int strCmp(const String s1, const String s2) {
    if (!s1)
        return -1;

    size_t l1 = _str_len(s1);
    size_t l2 = _str_len(s2);
    size_t minlen = (l1 < l2) ? l1 : l2;
    int cmp = memcmp(s1, s2, minlen);
    if (cmp != 0)
        return cmp;
    return (l1 > l2) - (l1 < l2);
}

void strtoLower(String s) {
    size_t len = _str_len(s), j;

    for (j = 0; j < len; j++)
        s[j] = tolower(s[j]);
}

void strtoUpper(String s) {
    size_t len = _str_len(s), j;

    for (j = 0; j < len; j++)
        s[j] = toupper(s[j]);
}

String strnCpy(String dest, const char *src, size_t n) {
    if (!dest || !src)
        return dest;

    if (_str_alloc(dest) < n) {
        dest = strGrow(dest, n - _str_len(dest));
        if (!dest)
            return NULL;
    }

    memcpy(dest, src, n);
    strSetLen(dest, n);
    return dest;
}

String strCpy(String dest, const char *src) {
    if (!src) {
        strClear(dest);
        return dest;
    }
    return strnCpy(dest, src, strlen(src));
}

String strnCat(String s, const void *src, size_t n) {
    if (!s || !src || n == 0)
        return s;

    size_t current_len = _str_len(s);
    s = strGrow(s, n);
    if (!s)
        return NULL;

    _StrHdr *hdr = _str_hdr(s);
    memcpy(s + current_len, src, n);
    hdr->len = current_len + n;
    s[hdr->len] = '\0';

    return s;
}

String strCat(String s, const char *src) {
    return strnCat(s, src, strlen(src));
}

String strCatFmt(String s, const char *fmt, ...) {
    if (!s || !fmt)
        return s;

    va_list ap;
    int needed;

    va_start(ap, fmt);
    needed = vsnprintf(NULL, 0, fmt, ap);
    va_end(ap);

    if (needed < 0)
        return NULL;

    s = strGrow(s, needed);
    if (!s)
        return NULL;

    _StrHdr *hdr = _str_hdr(s);
    size_t len = hdr->len;

    va_start(ap, fmt);
    int written = vsnprintf(s + len, needed + 1, fmt, ap);
    va_end(ap);

    if (written < 0)
        return NULL;

    hdr->len = len + written;

    return s;
}

String strSlice(const String s, size_t start, size_t end) {
    if (!s)
        return NULL;

    size_t len = _str_len(s);

    if (start > len)
        start = len;
    if (end > len)
        end = len;
    if (start > end)
        start = end;

    size_t newlen = end - start;
    return strNewLen(s + start, newlen);
}

String *strnSplit(const char *s, size_t len, const char *sep, size_t seplen, int *count) {
    if (!s || !sep || !count || seplen < 1 || len == 0) {
        *count = 0;
        return NULL;
    }

    int elements = 0;
    int slots = len / 4; // heuristic based on input length
    if (slots < 8)
        slots = 8;
    if (slots > 1000)
        slots = 1000; // cap initial allocation

    String *tokens = malloc(sizeof(String) * slots);
    if (!tokens) {
        *count = 0;
        return NULL;
    }

    long start = 0;

    // single character separator
    if (seplen == 1) {
        char sep_char = sep[0];
        for (long j = 0; j <= (long)(len - 1); j++) {
            if (s[j] == sep_char) {
                // handle array growth
                if (elements >= slots - 1) {
                    slots *= 2;
                    String *new_tokens = realloc(tokens, sizeof(String) * slots);
                    if (!new_tokens)
                        goto cleanup;
                    tokens = new_tokens;
                }

                tokens[elements] = strNewLen(s + start, j - start);
                if (!tokens[elements])
                    goto cleanup;
                elements++;
                start = j + 1;
            }
        }
    } else {
        // multi-character separator
        for (long j = 0; j <= (long)(len - seplen); j++) {
            if (memcmp(s + j, sep, seplen) == 0) {
                if (elements >= slots - 1) {
                    slots *= 2;
                    String *new_tokens = realloc(tokens, sizeof(String) * slots);
                    if (!new_tokens)
                        goto cleanup;
                    tokens = new_tokens;
                }

                tokens[elements] = strNewLen(s + start, j - start);
                if (!tokens[elements])
                    goto cleanup;
                elements++;
                start = j + seplen;
                j = j + seplen - 1;
            }
        }
    }

    // add final token
    if (elements >= slots) {
        slots++;
        String *new_tokens = realloc(tokens, sizeof(String) * slots);
        if (!new_tokens)
            goto cleanup;
        tokens = new_tokens;
    }

    tokens[elements] = strNewLen(s + start, len - start);
    if (!tokens[elements])
        goto cleanup;
    elements++;

    *count = elements;
    return tokens;

cleanup:
    for (int i = 0; i < elements; i++) {
        strFree(tokens[i]);
    }
    free(tokens);
    *count = 0;
    return NULL;
}

String *strSplit(const String s, const char *sep, int *count) {
    if (!s || !sep || !count) {
        *count = 0;
        return NULL;
    }
    return strnSplit(s, _str_len(s), sep, strlen(sep), count);
}

void strFreeSplitRes(String *tokens, int count) {
    if (!tokens)
        return;

    for (int i = 0; i < count; i++) {
        strFree(tokens[i]);
    }
    free(tokens);
}

String strMapChars(String s, const char *from, const char *to, size_t setlen) {
    if (!s || !from || !to)
        return s;

    size_t j, i, l = _str_len(s);

    for (j = 0; j < l; j++) {
        for (i = 0; i < setlen; i++) {
            if (s[j] == from[i]) {
                s[j] = to[i];
                break;
            }
        }
    }
    return s;
}
