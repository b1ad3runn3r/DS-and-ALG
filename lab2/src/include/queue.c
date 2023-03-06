#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#if defined(Q_VECTOR)

Queue *init_queue() {
    Queue *res = calloc(1, sizeof(Queue));
    if (!res) return NULL;

    res->data = calloc(Q_MAX, sizeof(void *));
    if (!(res->data)) {
        free(res);
        return NULL;
    }

    return res;
}

int enqueue(Queue *q, void *data) {
    if (q->front == q->rear && q->len == Q_MAX) {
        return EXIT_FAILURE;
    }
    q->data[q->rear] = data;
    q->rear = (q->rear + 1) % Q_MAX;
    q->len += 1;

    return EXIT_SUCCESS;
}

void *dequeue(Queue *q) {
    if (q->front == q->rear && q->len == Q_MAX) return NULL;

    void *res = NULL;
    res = q->data[q->front];
    q->front = (q->front + 1) % Q_MAX;
    q->len -= 1;

    return res;
}

void print_queue(const Queue *q, void (*_print)(const void *)) {
    if (!first(q)) return;
    for (size_t i = q->front; i != q->rear; i = (i + 1) % Q_MAX) {
        (*_print)(q->data[i]);
    }
}

void free_queue(Queue *q, void (*_free)(void *)) {
    if (q) {
        if (q->data) {
            for (size_t i = q->front; i != q->rear; i = (i + 1) % Q_MAX) {
                (*_free)(q->data[i]);
            }
            free(q->data);
        }
        free(q);
    }
}

void *first(const Queue *q) {
    if (!q || !(q->len)) return NULL;
    return q->data[q->front];
}


#elif defined(Q_LIST)

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

#endif
