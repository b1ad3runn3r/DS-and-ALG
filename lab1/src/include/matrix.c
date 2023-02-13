#include "matrix.h"

#include <stdlib.h>
#include <stdio.h>

void print(Matrix matrix) {
    Line* cur_row = NULL;
    if (matrix.size != 0 && matrix.lines != NULL) {
        for (int i = 0; i < matrix.size; i++) {
            cur_row = matrix.lines + i;
            for (int j = 0; j < cur_row->size; j++) {
                printf("%d ", (cur_row->data)[j]);
            }
            printf("\n");
        }
    }
}

int input(Matrix* matrix) {
    Line *cur_line = NULL;

    int n = 0, m = 0;
    printf("%s", "Enter m: ");
    scanf("%d", &m);

    while (m <= 0) {
        printf("%s","Wrong input. Try again: ");
        scanf("%d", &m);
    }

    matrix->size = m;
    matrix->lines = (Line *)calloc(m, sizeof(Line));

    for (int i = 0; i < m; i++) {
        cur_line = (matrix->lines + i);

        printf("%s", "Enter current n: ");
        scanf("%d", &n);
        
        while (n <= 0) {
            printf("%s","Wrong input. Try again: ");
            scanf("%d", &n);
        }
        
        cur_line->size = n;
        cur_line->data = (int *)calloc(n, sizeof(int));
        
        printf("%s", "Now enter desired numbers: ");
        for (int j = 0; j < n; j++) {
            scanf("%d", cur_line->data + j);
        }
    }

    return EXIT_SUCCESS;
}

static int get_first_smallest(int size, int* data) {
    int idx = -1;
    for (int i = 0; i < size - 1; i++) {
        if (data[i + 1] > data[i]) {
            idx = i + 1;
            break;
        }
    }

    return idx;
}

static int get_last_largest(int size, int* data) {
    int idx = -1;
    for (int i = size; i > 1; i--) {
        if (data[i] < data[i - 1]) {
            idx = i;
            break;
        }
    }

    return idx;
}

static inline void swap(int *ptr1, int *ptr2) {
    int tmp = 0;
    *ptr1 = *ptr2;
    *ptr2 = tmp;
}

void _free(Matrix matrix) {
    Line* line = NULL;
    if (matrix.size != 0 && matrix.lines != NULL) {
        for (int i = 0; i < matrix.size; i++) {
            line = matrix.lines + i;
            if (line->size != 0 && line->data != NULL) {
                free(line->data);
            }
        }
        free(matrix.lines);
    }

    matrix.size = 0;
    matrix.lines = NULL;
}
