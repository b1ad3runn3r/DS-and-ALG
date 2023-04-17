#ifndef UTILS_H
#define UTILS_H
#define CHUNK 255

#include <stdio.h>

void clear_screen();
char *readline(const char *prompt);
int get_int(const char *prompt, int *res);
int get_size_t(const char *prompt, size_t *res);

#endif //UTILS_H
