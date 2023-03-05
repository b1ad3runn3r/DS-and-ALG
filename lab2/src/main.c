#include "include/queue.h"
#include "include/airport.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    char *input = buffered_input();
    if (!input) return EXIT_FAILURE;
    size_t amt = 0, l_size = 0;

    Client **line = parse_input(input, &amt, &l_size);
    if (!line) {
        free(input);
        return EXIT_FAILURE;
    }

    Airport *airport = init_airport(amt);
    if (!airport) {
        free(input);
        return EXIT_FAILURE;
    }

    print_airport(airport);
    for (size_t i = 0; i < l_size; ++i) {
        free_client(line[i]);
    }
    free(line);
    free_airport(airport);
    free(input);
    return EXIT_SUCCESS;
}