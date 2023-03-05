#ifndef AIRPORT_H
#define AIRPORT_H

#include <stdlib.h>
#include "queue.h"

typedef struct Client {
    char *id;
    size_t ta;
    size_t ts;
} Client;

typedef struct Reception {
    Queue *queue;
} Reception;

typedef struct Airport {
    Reception *receptions;
    size_t size;
} Airport;

#define BUFFER 255

size_t count(const char *str, char search);

int compare(const void *p1, const void *p2);

char *buffered_input();

Client **parse_input(char *input, size_t *amt, size_t *l_size);

Airport *init_airport(size_t size);

void print_airport(const Airport *airport);

void free_airport(Airport *airport);

void print_client(const void *c);

void free_client(void *);

#endif //AIRPORT_H
