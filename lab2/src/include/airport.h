#ifndef AIRPORT_H
#define AIRPORT_H

typedef struct Client {
    char *name;
} Client;

void _print(const void *c);

void _free(void *);

#endif //AIRPORT_H
