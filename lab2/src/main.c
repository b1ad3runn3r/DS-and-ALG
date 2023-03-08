#include "include/airport.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
    srand(time(NULL));
    char *input = buffered_input();
    if (!input) {
        printf("Input not allocated!\n");
        return EXIT_FAILURE;
    }
    size_t amount = 0;

    Queue *queue = parse_input(input, &amount);
    if (!queue) {
        printf("Wrong input or memory allocation error!\n");
        free(input);
        return EXIT_FAILURE;
    }

    Airport *airport = init_airport(amount);
    if (!airport) {
        printf("Airport not allocated\n");
        free(input);
        free_queue(queue, free_client);
        return EXIT_FAILURE;
    }

    if (random_choice(airport, queue)) {
        printf("Incorrect airport!\n");
        return EXIT_FAILURE;
    }

    free_queue(queue, free_client);
    free_airport(airport);
    free(input);
    return EXIT_SUCCESS;
}