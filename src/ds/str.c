#include "ds.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

typedef struct {
    uint16_t len;
    uint16_t alloc;
} _StrHdr;

// get header pointer from String
static inline _StrHdr *_str_hdr(String s) {
    return ((_StrHdr *)s - 1);
}

// get available space
static inline uint16_t _str_avail(const String s) {
    return (_str_hdr(s)->alloc - _str_hdr(s)->len);
}

static inline uint16_t _str_len(const String s) {
    return _str_hdr(s)->len;
}

static inline uint16_t _str_alloc(const String s) {
    return _str_hdr(s)->alloc;
}

String strNewLen(const void *s, size_t len) {
    if (len > UINT16_MAX) {
        return NULL; // for strings longer than 64KB
    }

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

    _StrHdr *old_hdr = _str_hdr(s);

    if (_str_avail(s) >= addlen) {
        return s;
    }

    size_t len = old_hdr->len;

    if (len > UINT16_MAX - addlen) {
        return NULL;
    }

    size_t min_needed = len + addlen;
    size_t new_alloc = old_hdr->alloc;

    // exponential growth
    while (new_alloc < min_needed) {
        if (new_alloc > UINT16_MAX / 2) {
            new_alloc = UINT16_MAX;
            break;
        }
        new_alloc *= 2;
    }

    if (new_alloc < min_needed) {
        new_alloc = UINT16_MAX;
        if (new_alloc - len < addlen) {
            return NULL;
        }
    }

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

uint16_t strLen(const String s) {
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
    if (cmp == 0)
        return l1 > l2 ? 1 : (l1 < l2 ? -1 : 0);
    return cmp;
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

    if (n > UINT16_MAX) {
        n = UINT16_MAX;
    }

    dest = strGrow(dest, n - _str_len(dest));
    if (!dest)
        return NULL;

    memcpy(dest, src, n);
    strSetLen(dest, n);

    return dest;
}

String strCpy(String dest, const char *src) {
    return strnCpy(dest, src, strlen(src));
}

String strnCat(String s, const char *src, size_t n) {
    if (!s || !src)
        return s;

    size_t tlen = strlen(src);
    if (tlen > n)
        tlen = n;
    if (tlen == 0)
        return s;

    s = strGrow(s, tlen);
    if (!s)
        return NULL;

    _StrHdr *hdr = _str_hdr(s);
    size_t len = hdr->len;

    memcpy(s + len, src, tlen);
    hdr->len = len + tlen;
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

static String _str_tok_r(String s, const char *delim, String *saveptr) {
    if (!delim)
        return NULL;

    String token;

    if (s) {
        *saveptr = s;
    } else if (!*saveptr) {
        return NULL;
    }

    // skip leading delimiters
    *saveptr += strspn(*saveptr, delim);
    if (**saveptr == '\0') {
        *saveptr = NULL;
        return NULL;
    }

    // find end of token
    token = *saveptr;
    *saveptr = token + strcspn(token, delim);

    if (**saveptr != '\0') {
        **saveptr = '\0';
        (*saveptr)++;
    } else {
        *saveptr = NULL;
    }

    return token;
}

String strTok(String s, const char *delim) {
    static String saveptr = NULL;
    return _str_tok_r(s, delim, &saveptr);
}

String strMapChars(String s, const char *from, const char *to, size_t setlen) {
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
