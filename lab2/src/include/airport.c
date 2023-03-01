#include "airport.h"
#include <stdio.h>
#include <stdlib.h>

void _print(const void *c) {
    Client *cur = (Client *) c;
    printf("%s ", cur->name);
}

void _free(void *c) {
    Client *cur = (Client *)c;
    free(cur->name);
    free(cur);
}
