#include "list.h"

#include <stdlib.h>
#include <stdio.h>
#include "value.h"

static ListNode* listNode(Value value)
{
  ListNode* res = malloc(sizeof(ListNode));
  res->value = value;
  res->next = NULL;
  return res;
}

void lpush(List* list, Value value)
{
  ListNode* node = listNode(value);
  if (list->head) node->next = list->head;
  list->head = node;
}

void rpush(List* list, Value value)
{
  ListNode* node = listNode(value);
  if (list->head == NULL) {
    list->head = node;
    return;
  }
  ListNode* iter = list->head;
  while (iter->next) iter = iter->next;
  iter->next = node;
}

ListNode* left(List* list)
{
  return list->head;
}

ListNode* right(List* list)
{
  if (list->head == NULL) return NULL;
  ListNode* iter = list->head;
  while (iter->next) iter = iter->next;
  return iter;
}

void free_node(ListNode* node)
{
  value_free(node->value);
  free(node);
}

Value lpop(List* list)
{
  if (list->head == NULL) return value_nil();
  ListNode* to_pop = list->head;
  list->head = list->head->next;
  free(to_pop);
  return to_pop->value;
}

Value rpop(List* list)
{
  if (list->head == NULL) return value_nil();
  if (list->head->next == NULL) {
    Value res = list->head->value;
    free(list->head);
    list->head = NULL;
    return res;
  }
  ListNode* iter = list->head;
  while (iter->next->next) iter = iter->next;
  Value res = iter->next->value;
  free(iter->next);
  iter->next = NULL;
  return res;
}

void list_reverse(List* list)
{
  if (list->head == NULL) return;
  List res = {0};
  while (list->head) {
    lpush(&res, lpop(list));
  }
  *list = res;
}

static void free_node_rec(ListNode* node)
{
  if (node->next) free_node_rec(node->next);
  free_node(node);
}

void list_free(List* list) 
{
  if (list->head == NULL) return;
  free_node_rec(list->head);
  list->head = NULL;
}

int list_print(List* list, char* dest)
{
  int offset = 0;
  ListNode* iter = list->head;
  offset += sprintf(dest + offset, "[ ")-1;
  while (iter) {
    offset += sprintf(dest + offset, "\n  ");
    offset += value_print(iter->value, dest + offset) - 1;
    iter = iter->next;
  }
  offset += sprintf(dest + offset+1, "]\n");
  return offset;
}

size_t list_length(List* list)
{
  size_t res = 0;
  ListNode* iter = list->head;
  while (iter) {
    res++;
    iter = iter->next;
  }
  return res;
}