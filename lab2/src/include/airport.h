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

char *buffered_input();

Queue *parse_input(char *input, size_t *amt);

Airport *init_airport(size_t size);

void print_airport(Airport *airport);

void free_airport(Airport *airport);

void print_client(const void *c);

void free_client(void *);

#endif //AIRPORT_H
