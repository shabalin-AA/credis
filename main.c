#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "value.h"
#include "str.h"
#include "list.h"
#include "set.h"

int main( void )
{
  Set set = {0};
  set_insert(&set, value_int(1));
  set_insert(&set, value_int(2));
  set_insert(&set, value_int(1));
  set_insert(&set, value_int(3));
  set_insert(&set, value_int(4));
  for (size_t i = 0; i < set.length; i++) {
    printf("%d ", set.items[i].integer);
  } printf("\n");
  printf("%zd\n", set_indexof(&set, value_int(5)));
  set_remove(&set, set_indexof(&set, value_int(5)));
  for (size_t i = 0; i < set.length; i++) {
    printf("%d ", set.items[i].integer);
  } printf("\n");
  return 0;
}