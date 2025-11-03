#ifndef DS_H
#define DS_H

/*
 * SDS style strings
 */

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef char *String;

String strNew(const char *s);
String strNewLen(const void *s, size_t len);
String strEmpty(void);
String strDup(const String s);
void strFree(String s);

String strGrow(String s, size_t addlen);
String strTrim(String s);
void strClear(String s);
uint16_t strLen(const String s);
void strSetLen(String s, size_t newlen);

int strCmp(const String s1, const String s2);
void strtoLower(String s);
void strtoUpper(String s);
String strCpy(String dest, const char *src);
String strnCpy(String dest, const char *src, size_t n);
String strCat(String s, const char *src);
String strnCat(String s, const char *src, size_t n);
String strCatFmt(String s, const char *fmt, ...);
String strSlice(const String s, size_t start, size_t end);
String strTok(String s, const char *delim);
String strMapChars(String s, const char *from, const char *to, size_t setlen);

#ifdef __cplusplus
}
#endif

#endif // DS_H
