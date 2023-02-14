#include <stdlib.h>
#include <stdio.h>
#include "include/matrix.h"

int main() {
    Matrix matrix_old = {0, NULL};
    Matrix matrix_new = {0, NULL};

    if (input(&matrix_old)) {
        _free(matrix_old);
        return EXIT_FAILURE;
    }

    printf("%s\n", "Original matrix: ");
    print(matrix_old);

    if (task(matrix_old, &matrix_new)) {
        _free(matrix_old);
        _free(matrix_new);
        return EXIT_FAILURE;
    }

    printf("%s\n", "Result matrix: ");
    print(matrix_new);

    _free(matrix_old);
    _free(matrix_new);
    return EXIT_SUCCESS;
}