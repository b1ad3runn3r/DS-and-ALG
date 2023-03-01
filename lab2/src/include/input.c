#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t count(const char *str, char search) {
    size_t cnt = 0;
    const size_t len = strlen(str);
    for (size_t i = 0; i < len; i++) {
        if (str[i] == search) ++cnt;
    }

    return cnt;
}

char *buffered_input() {
    char buffer[BUFFER] = {0};
    size_t res_size = 1; // it's for \0

    char *res = calloc(BUFFER, sizeof(char));
    if (!res) return NULL;

    while(fgets(buffer, BUFFER, stdin)) {
        res_size += strlen(buffer);

        res = realloc(res, res_size * sizeof(char));
        if (!res) {
            return NULL;
        }

        strcat(res, buffer);
    }

    return res;
}