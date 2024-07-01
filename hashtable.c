#include "hashtable.h"

#include <stdio.h>

#include "str.h"
#include "value.h"
#include "list.h"

#define LEN 16
#define FACTOR 2

size_t hash(Str key, size_t capacity)
{
  if (capacity == 0) return -1;
  static size_t a = 97;
  static size_t b = 25;
  static size_t p = 31;
  size_t sum = 0;
  for (size_t i = 0; i < key.len; i++)
    sum += key.content[i];
  return ((a*sum + b)%p)%capacity;
}

void ht_insert(Hashtable* table, Str key, Value value)
{
  if (table->capacity == table->length) {
    table->capacity *= FACTOR;
    table->items = realloc(table->items, sizeof(Pair) * table->capacity);
  }
  if (table->capacity == 0) {
    table->capacity = LEN;
    table->length = 0;
    table->items = malloc(sizeof(Pair) * table->capacity);
  }
  size_t h = hash(key, table->capacity);
  Pair* item = &table->items[h];
  item->key = str_new(key.content);
  lpush(&item->chain, value);
  table->length++;
}

void ht_remove(Hashtable* table, Str key)
{
  size_t h = hash(key, table->capacity);
  Pair* item = &table->items[h];
  list_free(&item->chain);
  str_free(&item->key);
  table->length--;
}

List ht_get(Hashtable* table, Str key) 
{
  size_t h = hash(key, table->capacity);
  if (h > table->capacity) return (List){0};
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

void ht_print(Hashtable* table)
{
  printf("{");
  for (size_t i = 0; i < table->capacity; i++) {
    if (table->items[i].key.content) {
      printf("\n  %s => ", table->items[i].key.content);
      list_print(&table->items[i].chain);
    }
  }
  printf("}\n");
}