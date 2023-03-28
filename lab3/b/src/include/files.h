#ifndef FILES_H
#define FILES_H

#include <stdlib.h>
#include "table.h"

int fmemmove(FILE *fp, long off_src, long off_dst, size_t size);
int change_offsets(Table *table, long delta);

#endif //FILES_H
