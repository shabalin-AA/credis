#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "value.h"
#include "str.h"
#include "list.h"
#include "set.h"
#include "hashtable.h"

int main( void )
{
  Hashtable table = {0};
  ht_insert(&table, str_new("key1"), value_int(1));
  ht_insert(&table, str_new("key2"), value_int(2));
  ht_insert(&table, str_new("key3"), value_int(3));
  ht_insert(&table, str_new("key3"), value_int(33));
  ht_insert(&table, str_new("key4"), value_int(4));
  ht_insert(&table, str_new("key5"), value_int(5));
  ht_insert(&table, str_new("key6"), value_int(6));
  ht_insert(&table, str_new("key7"), value_int(7));
  for (size_t i = 0; i < table.capacity; i++) {
    Pair item = table.items[i];
    if (item.key) {
      printf("%d; %s: %d\n", i, item.key, integer_value(left(&item.chain)->value));
    }
  }
  ht_remove(&table, str_new("key3"));
  for (size_t i = 0; i < table.capacity; i++) {
    Pair item = table.items[i];
    if (item.key) {
      printf("%d; %s: %d\n", i, item.key, integer_value(left(&item.chain)->value));
    }
  }
  return 0;
}