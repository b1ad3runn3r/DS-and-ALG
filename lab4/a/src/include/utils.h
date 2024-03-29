#ifndef UTILS_H
#define UTILS_H
#define CHUNK 255

#include <stdlib.h>

char *readline(const char *prompt);
int get_int(const char *prompt, int *res);
int get_size_t(const char *prompt, size_t *res);

#endif //UTILS_H
