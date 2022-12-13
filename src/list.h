#ifndef CLOX_LIST_H
#define CLOX_LIST_H

#include <stdlib.h>

#ifndef LIST_MALLOC
#define LIST_MALLOC malloc
#endif

#ifndef LIST_FREE
#define LIST_FREE free
#endif

struct list_node {
  struct list_node *prev;
  struct list_node *next;
  void *val;
};

struct list {
  struct list_node *head;
  struct list_node *tail;
  unsigned int len;

  void (*free)(void *val);

  int (*match)(void *a, void *b);
};

// list_node prototypes;

struct list_node *list_node_new(void *val);

// list prototypes;

struct list *list_new();

void list_destroy(struct list *self);

void list_rpush(struct list *self, struct list_node *node);

void list_lpush(struct list *self, struct list_node *node);

struct list_node *list_rpop(struct list *self);

struct list_node *list_lpop(struct list *self);

struct list_node *list_find(struct list *self, void *val);

void list_remove(struct list *self, struct list_node *node);

#endif
