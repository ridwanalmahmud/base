#ifndef DS_H
#define DS_H
/*
 * SDS style strings
 */

#include <stddef.h>
#include <stdint.h>

typedef char *String;

// initialize and free
String strNew(const char *s);
String strNewLen(const void *s, size_t len); // string from binary data
String strEmpty(void);
String strDup(const String s);
void strFree(String s);

#endif
