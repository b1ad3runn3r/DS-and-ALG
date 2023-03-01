#include "include/queue.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    Queue *q = init();

    char *s1 = calloc(3, 1);
    s1[0] = 'a', s1[1] = 'b';

    char *s2 = calloc(3, 1);
    s2[0] = 'b', s2[1] = 'a';

    enqueue(q, s1);
    print(q);

    enqueue(q, s2);
    print(q);

    free_queue(q);
    return 0;
}