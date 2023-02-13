#include <stdlib.h>
#include "include/matrix.h"

int main() {
    Matrix matrix = {0, NULL};

    if (input(&matrix)) {
        return EXIT_FAILURE;
    }

    printf("%s\n", "Original matrix");
    print(matrix);

    

    return EXIT_SUCCESS;
}