#include "str.h"

#include <stdlib.h>

Str str_new(const char* src)
{
  Str res = calloc(strlen(src), sizeof(char));
  strcpy(res, src);
  return res;
}