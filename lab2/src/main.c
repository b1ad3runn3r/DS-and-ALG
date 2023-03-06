#include "include/airport.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
    srand(time(NULL));
    char *input = buffered_input();
    if (!input) {
        printf("Wrong input!\n");
        return EXIT_FAILURE;
    }
    size_t amt = 0;

    Queue *queue = parse_input(input, &amt);
    if (!queue) {
        printf("Queue not allocated!\n");
        free(input);
        return EXIT_FAILURE;
    }

    Airport *airport = init_airport(amt);
    if (!airport) {
        printf("Airport not allocated\n");
        free(input);
        free_queue(queue, free_client);
        return EXIT_FAILURE;
    }

    random_choice(airport, queue);
    free_queue(queue, free_client);
    free_airport(airport);
    free(input);
    return EXIT_SUCCESS;
}