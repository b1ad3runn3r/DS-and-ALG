#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdint.h>

typedef struct PQ_Node {
    void *data;
    uint64_t priority;
    struct PQ_Node *next;
} PQNode;

typedef struct Priority_Queue {
    PQNode *head;
} PriorityQueue;

PriorityQueue *pq_init();

void *first_pq(PriorityQueue *pq);

void *pop(PriorityQueue *pq);

void push(PriorityQueue *pq, void *data, uint64_t priority);

void free_pq(PriorityQueue *pq);

int pq_is_empty(PriorityQueue *pq);

#endif // PRIORITY_QUEUE_H
