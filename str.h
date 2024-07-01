#ifndef _STR_H_
#define _STR_H_


#include <string.h>
#include <stdbool.h>

typedef struct {
  char* content;
  size_t len;
} Str;

Str str_new(const char* src);
void str_free(Str* str);
void str_print(Str* str);
bool str_eq(Str s1, Str s2);


#endif