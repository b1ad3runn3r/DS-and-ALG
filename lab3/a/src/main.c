#include <stdio.h>
#include "include/types.h"
#include "include/table.h"

int main() {
    IndexType msize = 10;
    Table *table = init_table(msize);

    if (!table) {
        return 1;
    }

    free_table(table);
    return 0;
}