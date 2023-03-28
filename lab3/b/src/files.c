#include <stdio.h>
#include "include/files.h"
#include "include/types.h"
#include "include/table.h"

int fmemmove(FILE *fp, long off_src, long off_dst, size_t size) {
    if (!fp || off_src < 0 || off_dst < 0) {
        return E_NOFILE;
    }
    char *p_src = NULL;
    fseek(fp, off_src, SEEK_SET);
    p_src = calloc(size, sizeof(char));
    if (!p_src) {
        return E_ALLOC;
    }

    fread(p_src, sizeof(char), size, fp);
    fseek(fp, off_dst, SEEK_SET);
    fwrite(p_src, sizeof(char), size, fp);
    free(p_src);

    return E_OK;
}

int change_offsets(Table *table, long delta) {
    if (!table) {
        return E_NULLPTR;
    }

    if (!table->fp || delta < 0) {
        return E_NOFILE;
    }

    KeySpace cur_element;
    Item cur_item;

    for (int i = 0; i < table->csize; i++) {
        long offset = 2 * sizeof(int) + i * (sizeof(KeySpace) + sizeof(Item));

        fseek(table->fp, offset, SEEK_SET);
        fread(&cur_element, sizeof(KeySpace), 1, table->fp);
        fread(&cur_item, sizeof(Item), 1, table->fp);

        cur_element.key_offset += delta;
        cur_element.par_offset += delta;
        cur_item.offset += delta;

        fseek(table->fp, offset, SEEK_SET);
        fwrite(&cur_element, sizeof(KeySpace), 1, table->fp);
        fwrite(&cur_item, sizeof(Item), 1, table->fp);
    }

    return E_OK;
}
