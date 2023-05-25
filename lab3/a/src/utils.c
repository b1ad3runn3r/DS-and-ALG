#include "utils.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void clear_screen() {
    printf("\033[;H\033[J");
}

char *readline(const char* prompt) {
    printf("%s", prompt);

    char *buf = NULL;
    size_t len = 0;
    getline(&buf, &len, stdin);

    return buf;
}

int get_int(const char *prompt, int *res) {
    char *line = readline(prompt);
    if (!line) {
        return E_ALLOC;
    }

    int buf = 0;
    if ((buf = strtol(line, NULL, 10)) == 0 && errno == EINVAL) {
        free(line);
        return E_WRONGINPUT;
    }

    free(line);
    *res = buf;
    return E_OK;
}





