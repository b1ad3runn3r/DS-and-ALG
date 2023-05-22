#ifndef QUEUE_H
#define QUEUE_H

#include "list.h"

typedef struct Queue {
    Node *front;
    Node *rear;
} Queue;

Queue *init_queue();

int enqueue(Queue *queue, void *data);

void *dequeue(Queue *queue);

void free_queue(Queue *queue);

void *first(Queue *queue);

#endif // QUEUE_H
