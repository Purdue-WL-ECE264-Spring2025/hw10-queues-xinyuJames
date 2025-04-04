#include "linked_list.h"

#include <stdlib.h>

struct list_node *new_node(size_t value) 
{
  struct list_node* node = malloc(sizeof(struct list_node));
  node->value = value;
  node->next = NULL;
  return node;
}

void insert_at_head(struct linked_list *list, size_t value) {}

void insert_at_tail(struct linked_list *list, size_t value) 
{
  //no list
  // if (list == NULL)
  // {
  //   return;
  // }
  // when there is no head, add it to be head
  if (list->head == NULL)
  {
    list->head = new_node(value);
    return;
  }

  // create a node and end node point to the node
  struct list_node* node = new_node(value);
  struct list_node* pos = list->head;
  while (pos->next != NULL)
  {
    // if not the end node, move to next node
    pos = pos->next;
  }
  // it is end node now
  pos->next = node;

  return;
}

size_t remove_from_head(struct linked_list *list) 
{
  //save first node
  struct list_node* removed = list->head;
  list->head = list->head->next;
  size_t return_val = removed->value;
  free(removed);
  return return_val;
}

size_t remove_from_tail(struct linked_list *list) { return 0; }

void free_list(struct linked_list list) 
{
  struct list_node* head = list.head;
  while (head != NULL)
  {
    struct list_node* tmp_node = head;
    head = head->next;
    free(tmp_node);
  }
}

// Utility function to help you debugging, do not modify
void dump_list(FILE *fp, struct linked_list list) {
  fprintf(fp, "[ ");
  for (struct list_node *cur = list.head; cur != NULL; cur = cur->next) {
    fprintf(fp, "%zu ", cur->value);
  }
  fprintf(fp, "]\n");
}
