#include "str.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

Str str_new(const char* src)
{
  Str res = {0};
  res.len = strlen(src);
  res.content = calloc(res.len, sizeof(char));
  strcpy(res.content, src);
  return res;
}

void str_free(Str* str)
{
  free(str->content);
  str->content = NULL;
  str->len = 0;
}

int str_print(Str* str, char* dest)
{
  return sprintf(dest, "%s\n", str->content);
}

bool str_eq(Str s1, Str s2) 
{
  return !strcmp(s1.content, s2.content);
}