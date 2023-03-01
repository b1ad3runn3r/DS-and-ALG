#ifndef QUEUE_H
#define QUEUE_H

typedef struct Node {
    char *data; //TODO: change to void* in future, currently using char* for testing ease
    struct Node *next;
} Node;

typedef struct Queue {
    Node *front;
    Node *rear;
} Queue;

Queue *init();

int is_empty(const Queue *q);

int enqueue(Queue *q, char *data);

char *dequeue(Queue *q);

void print(const Queue *q);

void free_queue(Queue *q);

#endif //QUEUE_H
