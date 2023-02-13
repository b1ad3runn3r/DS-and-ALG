#ifndef MATRIX_H
#define MATRIX_H

typedef struct Line {
    int     size;
    int     *data;
} Line;

typedef struct Matrix {
    int     size;
    Line    *lines;
} Matrix;

void print(Matrix matrix);

int input(Matrix *matrix);

int get_first_smallest(int size, int *data);

int get_last_largest(int size, int *data);

void task(Matrix* matrix);

#endif // MATRIX_H