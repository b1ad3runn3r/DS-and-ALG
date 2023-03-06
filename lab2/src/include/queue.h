#ifndef QUEUE_H
#define QUEUE_H
#include <stdlib.h>

#define Q_MAX 1024

#if defined(Q_VECTOR)

typedef struct Queue {
    void **data;
    size_t len;
    size_t front;
    size_t rear;
} Queue;

#elif defined(Q_LIST)

typedef struct Node {
    void *data;
    struct Node *next;
} Node;

typedef struct Queue {
    Node *front;
    Node *rear;
} Queue;

#endif

Queue *init_queue();

int enqueue(Queue *q, void *data);

void *dequeue(Queue *q);

void print_queue(const Queue *q, void (*_print)(const void *));

void free_queue(Queue *q, void (*_free)(void *));

void *first(const Queue *q);

#endif //QUEUE_H
