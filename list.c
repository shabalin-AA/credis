#include "list.h"

#include <stdlib.h>
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

static void free_node(ListNode* node)
{
  free_value(node->value);
  free(node);
}

void lpop(List* list)
{
  if (list->head == NULL) return;
  ListNode* to_pop = list->head;
  list->head = list->head->next;
  free_node(to_pop);
}

void rpop(List* list)
{
  if (list->head == NULL) return;
  if (list->head->next == NULL) {
    free_value(list->head->value);
    free(list->head);
    list->head = NULL;
  }
  ListNode* iter = list->head;
  while (iter->next->next) iter = iter->next;
  free_node(iter->next);
  iter->next = NULL;
}

static void free_node_rec(ListNode* node)
{
  if (node->next) free_node_rec(node->next);
  free_node(node);
}

void free_list(List* list) 
{
  if (list->head == NULL) return;
  free_node_rec(list->head);
  list->head = NULL;
}
