#ifndef DS_H
#define DS_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// === DYNAMIC STRINGS ===
/*
 * SDS style strings
 * (FYI not so memory efficient 16 bytes header for String)
 */

typedef char *String;

String strNew(const char *s);
String strNewLen(const void *s, size_t len);
String strEmpty(void);
String strDup(const String s);
void strFree(String s);

String strGrow(String s, size_t addlen);
String strTrim(String s);
size_t strLen(const String s);
void strClear(String s);
void strSetLen(String s, size_t newlen);

int strCmp(const String s1, const String s2);
void strtoLower(String s);
void strtoUpper(String s);
String strCpy(String dest, const char *src);
String strnCpy(String dest, const char *src, size_t n);
String strCat(String s, const char *src);
String strnCat(String s, const void *src, size_t n);
String strCatFmt(String s, const char *fmt, ...);
String strSlice(const String s, size_t start, size_t end);
String *strnSplit(const char *s, size_t len, const char *sep, size_t seplen, int *count);
String *strSplit(const String s, const char *sep, int *count);
void strFreeSplitRes(String *tokens, int count);
String strMapChars(String s, const char *from, const char *to, size_t setlen);

// === DYNAMIC ARRAYS ===

typedef struct {
    void *data;
    size_t len;
    size_t capacity;
    size_t elem_size;
} Array;

Array *arrNew(size_t elem_size, size_t initial_capacity);
Array *arrNewFromData(void *data, size_t elem_size, size_t count);
void arrFree(Array *arr);

void *arrGet(const Array *arr, size_t index);
void arrSet(Array *arr, size_t index, const void *value);
int arrPush(Array *arr, const void *value);
void arrPop(Array *arr, void *out_value);
void arrClear(Array *arr);

int arrReserve(Array *arr, size_t new_capacity);
int arrShrink(Array *arr);
size_t arrLength(const Array *arr);
size_t arrCapacity(const Array *arr);

int arrInsert(Array *arr, size_t index, const void *value);
void arrRemove(Array *arr, size_t index);
void arrRemoveRange(Array *arr, size_t start, size_t end);

void *arrFind(const Array *arr, const void *value, int (*compare)(const void *, const void *));
int arrContains(const Array *arr, const void *value, int (*compare)(const void *, const void *));
void arrSort(Array *arr, int (*compare)(const void *, const void *));
Array *arrSlice(const Array *arr, size_t start, size_t end);
Array *arrDup(const Array *arr);

#define arrTop(arr) ((arr)->len > 0 ? arrGet((arr), (arr)->len - 1) : NULL)
#define arrEmpty(arr) ((arr)->len == 0)

#ifdef __cplusplus
}
#endif

#endif // DS_H
