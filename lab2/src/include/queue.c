#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

Queue *init() {
    return calloc(1, sizeof(Queue));
}

int is_empty(const Queue *q) {
    return (!q->front);
}

int enqueue(Queue *q, void *data) {
    Node *tmp = calloc(1, sizeof(Node));
    if (!tmp) return EXIT_FAILURE;

    tmp->data = data;
    tmp->next = NULL;

    if (is_empty(q)) {
        q->front = tmp;
    } else {
        q->rear->next = tmp;
    }

    q->rear = tmp;

    return EXIT_SUCCESS;
}

void *dequeue(Queue *q) {
    if (is_empty(q)) return NULL;

    Node* tmp = q->front;
    void *ret = tmp->data;
    q->front = tmp->next;
    if (is_empty(q)) q->rear = NULL;

    free(tmp);

    return ret;
}

void print(const Queue *q, void (*_print)(const void *)) {
    if (is_empty(q)) return;

    Node *tmp = q->front;
    while (tmp) {
        (*_print)(tmp->data);
        tmp = tmp->next;
    }

    putchar('\n');
}

void free_queue(Queue *q, void (*_free)(void *)) {
    if (!q) return;

    while (q->front) {
        (*_free)(dequeue(q));
    }

    free(q);
}
