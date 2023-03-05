#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

Queue *init_queue() {
    return calloc(1, sizeof(Queue));
}

int enqueue(Queue *q, void *data) {
    Node *tmp = calloc(1, sizeof(Node));
    if (!tmp) return EXIT_FAILURE;

    tmp->data = data;
    tmp->next = NULL;

    if (!first(q)) {
        q->front = tmp;
    } else {
        q->rear->next = tmp;
    }

    q->rear = tmp;

    return EXIT_SUCCESS;
}

void *dequeue(Queue *q) {
    if (!first(q)) return NULL;

    Node* tmp = q->front;
    void *ret = tmp->data;
    q->front = tmp->next;
    if (!first(q)) q->rear = NULL;

    free(tmp);

    return ret;
}

void print_queue(const Queue *q, void (*_print)(const void *)) {
    if (!first(q)) return;

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

void *first(const Queue *q) {
    if (!(q->front)) return NULL;
    return q->front->data;
}
