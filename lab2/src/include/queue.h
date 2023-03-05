#ifndef QUEUE_H
#define QUEUE_H

typedef struct Node {
    void *data;
    struct Node *next;
} Node;

typedef struct Queue {
    Node *front;
    Node *rear;
} Queue;

Queue *init_queue();

int is_empty(const Queue *q);

int enqueue(Queue *q, void *data);

void *dequeue(Queue *q);

void print_queue(const Queue *q, void (*_print)(const void *));

void free_queue(Queue *q, void (*_free)(void *));

#endif //QUEUE_H
