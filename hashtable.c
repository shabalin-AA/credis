#include "hashtable.h"

#include <stdio.h>

#include "str.h"
#include "value.h"
#include "list.h"

#define LEN 16
#define FACTOR 2

ssize_t hash(Str key, size_t capacity)
{
  if (capacity == 0) return -1;
  static size_t a = 97;
  static size_t b = 25;
  static size_t p = 31;
  size_t sum = 0;
  for (size_t i = 0; i < key.len; i++)
    sum += key.content[i] * (i+1);
  return ((a*sum + b)%p)%capacity;
}

void ht_insert(Hashtable* table, Str key, Value value)
{
  if (table->capacity == 0) {
    table->capacity = LEN;
    table->length = 0;
    table->items = calloc(table->capacity, sizeof(Pair));
  }
  else if (table->capacity == table->length) {
    table->capacity *= FACTOR;
    table->items = realloc(table->items, sizeof(Pair) * table->capacity);
  }
  ssize_t h = hash(key, table->capacity);
  // printf("^%s^ %d\n", key.content, h);
  Pair* item = &table->items[h];
  item->key = key;
  lpush(&item->chain, value);
  table->length++;
}

void ht_remove(Hashtable* table, Str key)
{
  ssize_t h = hash(key, table->capacity);
  if (h == -1) return;
  Pair* item = &table->items[h];
  list_free(&item->chain);
  str_free(&item->key);
  table->length--;
}

List ht_get(Hashtable* table, Str key) 
{
  ssize_t h = hash(key, table->capacity);
  // printf("^%s^ %d\n", key.content, h);
  if (h == -1) return (List){0};
  Pair item = table->items[h];
  return item.chain;
}

void ht_free(Hashtable* table)
{
  for (size_t i = 0; i < table->length; i++) {
    Pair* item = &table->items[i];
    str_free(&item->key);
    list_free(&item->chain);
  }
  free(table->items);
  table->items = NULL;
  table->capacity = 0;
  table->length = 0;
}

int ht_print(Hashtable* table, char* dest)
{
  int offset = 0;
  offset += sprintf(dest + offset, "{");
  for (size_t i = 0; i < table->capacity; i++) {
    if (table->items[i].key.content) {
      offset += sprintf(dest + offset, "\n  %s => ", table->items[i].key.content);
      offset += list_print(&table->items[i].chain, dest + offset) - 1;
    }
  }
  offset += sprintf(dest + offset, "}\n");
  return offset;
}