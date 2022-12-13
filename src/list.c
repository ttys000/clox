#include "list.h"

#include <stdio.h>
#include <stdlib.h>

// Allocates a new list_node. NULL on failure.
struct list_node *list_node_new(void *val) {
  struct list_node *self;
  if (!(self = LIST_MALLOC(sizeof(struct list_node)))) return NULL;
  self->prev = NULL;
  self->next = NULL;
  self->val = val;
  return self;
}

// Allocates a new list. NULL on failure.
struct list *list_new() {
  struct list *self;
  if (!(self = LIST_MALLOC(sizeof(struct list)))) return NULL;
  self->head = NULL;
  self->tail = NULL;
  self->len = 0;
  self->free = NULL;
  self->match = NULL;
  return self;
}

// Free the list.
void list_destroy(struct list *self) {
  unsigned int len = self->len;
  struct list_node *current = self->head;
  struct list_node *next;

  while (len--) {
    next = current->next;
    if (self->free) self->free(current->val);
    LIST_FREE(current);
    current = next;
  }

  LIST_FREE(self);
}

// Append the given node to the list.
void list_rpush(struct list *self, struct list_node *node) {
  if (!node) return;

  if (self->len) {
    node->prev = self->tail;
    node->next = NULL;
    self->tail->next = node;
    self->tail = node;
  } else {
    node->prev = node->next = NULL;
    self->head = self->tail = node;
  }
  ++self->len;
}

// Prepend the given node to the list.
void list_lpush(struct list *self, struct list_node *node) {
  if (!node) return;

  if (self->len) {
    node->prev = NULL;
    node->next = self->head;
    self->head->prev = node;
    self->head = node;
  } else {
    node->prev = node->next = NULL;
    self->head = self->tail = node;
  }
  ++self->len;
}

// Return and detach the last node in the list.
struct list_node *list_rpop(struct list *self) {
  if (!self->len) return NULL;

  struct list_node *node = self->tail;

  if (--self->len) {
    (self->tail = node->prev)->next = NULL;
  } else {
    self->head = self->tail = NULL;
  }

  node->prev = node->next = NULL;
  return node;
}

// Return and detach the first node in the list.
struct list_node *list_lpop(struct list *self) {
  if (!self->len) return NULL;

  struct list_node *node = self->head;

  if (--self->len) {
    (self->head = node->next)->prev = NULL;
  } else {
    self->head = self->tail = NULL;
  }

  node->prev = node->next = NULL;
  return node;
}

// Return the leftest node which it's val is equal to val.
struct list_node *list_find(struct list *self, void *val) {
  if (!val) return NULL;

  struct list_node *current = self->head;

  while (current) {
    if (self->match) {
      if (self->match(val, current->val)) return current;
    } else {
      if (current->val == val) return current;
    }
    current = current->next;
  }

  return NULL;
}

// Remove the given node from the list.
void list_remove(struct list *self, struct list_node *node) {
  if (!node) return;

  node->prev ? (node->prev->next = node->next) : (self->head = node->next);
  node->next ? (node->next->prev = node->prev) : (self->tail = node->prev);
  if (self->free) self->free(node->val);
  LIST_FREE(node);
  --self->len;
}

void list_printer(struct list *self) {
  struct list_node *cur = self->head;
  printf("[ ");
  while (cur) {
    printf("%s ", (char *)cur->val);
    cur = cur->next;
  }
  printf("] len=%d\n", self->len);
}

int matchz(void *x, void *y) { return x == y; }

int main() {
  struct list *langs = list_new();

  printf("rpush: c, go, js\n");
  list_rpush(langs, list_node_new("c"));
  list_rpush(langs, list_node_new("go"));
  list_rpush(langs, list_node_new("js"));
  list_printer(langs);

  struct list_node *last = list_rpop(langs);
  printf("rpop: %s\n", (char *)last->val);
  list_printer(langs);

  struct list_node *first = list_lpop(langs);
  printf("lpop: %s\n", (char *)first->val);
  list_printer(langs);

  printf("lpush: c, js\n");
  list_lpush(langs, list_node_new("c"));
  list_lpush(langs, list_node_new("js"));
  list_printer(langs);

  langs->match = matchz;
  struct list_node *node = list_find(langs, "js");
  if (node) {
    printf("remove: %s\n", (char *)node->val);
    list_remove(langs, node);
  }
  list_printer(langs);

  printf("destroy list\n");
  list_destroy(langs);

  return 0;
}
