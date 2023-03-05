#include "include/queue.h"
#include "include/input.h"
#include "include/airport.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    char *input = buffered_input();
    if (!input) return EXIT_FAILURE;
    size_t amt = 0;

    Queue *res = parse_input(input, &amt);
    if (!res) return EXIT_FAILURE;

    printf("%zu\n", amt);
    print_queue(res, print_client);

    free_queue(res, free_client);
    free(input);
    return EXIT_SUCCESS;
}