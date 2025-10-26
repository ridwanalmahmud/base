#ifndef STR_H
#define STR_H

/*
 * SDS style strings
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>

typedef char *String;

// Construction & destruction
String str_new(const char *init);
String str_new_len(const void *init, size_t initlen);
String str_empty(void);
String str_dup(const String s);
String str_from_num(long long value);
void str_free(String s);

// Basic operations
size_t str_len(const String s);
bool str_cmp(const String s1, const String s2);
void str_update_len(String s);
void str_clear(String s);
String str_cpy(String s, const char *t, size_t len);

// Memory management
String str_grow_zero(String s, size_t len);
String str_shrink(String s);

// Concatenation
String str_cat(String s, const void *t, size_t len);
String str_cat_str(String s, const String t);
String str_cat_fmt(String s, const char *fmt, ...);
// Turn a string that may contain binary data or special characters into a quoted string
String str_cat_repr(String s, const char *p, size_t len);

// Manipulation
String str_trim(String s, const char *cset);
void str_slice(String s, size_t start, size_t end);

// Joining
String str_join(char **argv, int argc, char *sep);
String str_join_str(String *argv, int argc, const char *sep, size_t seplen);

// Transformation
String str_map_chars(String s, const char *from, const char *to, size_t setlen);

// Tokenization
String *str_split_len(const char *s, int len, const char *sep, int seplen, int *count);
void str_free_split_res(String *tokens, int count);
String *str_split_args(const char *line, int *argc);

// Case conversion
void str_to_lower(String s);
void str_to_upper(String s);

#endif
