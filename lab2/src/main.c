#include "include/queue.h"
#include "include/airport.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    Queue *q = init();

    Client *c1 = calloc(1, sizeof(Client));
    Client *c2 = calloc(1, sizeof(Client));

    char *s1 = calloc(3, 1);
    s1[0] = 'a'; s1[1] = 'b';
    c1->name = s1;

    char *s2 = calloc(3, 1);
    s2[0] = 'b'; s2[1] = 'a';
    c2->name = s2;

    enqueue(q, c1);
    print(q, _print);

    enqueue(q, c2);
    print(q, _print);

    free_queue(q, _free);

    return 0;
}