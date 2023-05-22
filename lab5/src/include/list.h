#ifndef LIST_H
#define LIST_H

#include <stdint.h>

typedef struct Node {
    void *data;
    struct Node *next;
} Node;

typedef struct List {
    Node *head;
    uint64_t size;
} List;

List *list_init();

int list_push(List *list, void *data, int (*cmp)(void *, void *));

void *list_get(List *list, void *data, int (*cmp)(void *, void *));

int list_delete(List *list, void *data, int (*cmp)(void *, void *), void (*free_data)(void *));

void list_free(List *list, void (*free_data)(void *));

#endif // LIST_H
