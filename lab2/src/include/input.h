#ifndef INPUT_H
#define INPUT_H

#include <stdlib.h>
#define BUFFER 255

size_t count(const char *str, char search);

char *buffered_input();

#endif //INPUT_H
