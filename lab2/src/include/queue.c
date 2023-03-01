#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

Queue *init() {
    return calloc(1, sizeof(Queue));
}

int is_empty(const Queue *q) {
    return (!q->front);
}

int enqueue(Queue *q, char *data) {
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

char *dequeue(Queue *q) {
    if (is_empty(q)) return NULL;

    Node* tmp = q->front;
    char *ret = tmp->data;
    q->front = tmp->next;

    free(tmp);

    return ret;
}

void print(const Queue *q) {
    if (is_empty(q)) return;

    Node *tmp = q->front;
    while (tmp) {
        printf("%s ", tmp->data);
        tmp = tmp->next;
    }

    putchar('\n');
}

void free_queue(Queue *q) {
    while (q->front) {
        free(dequeue(q));
    }

    free(q);
}
