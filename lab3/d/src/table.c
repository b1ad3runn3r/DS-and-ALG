#include <stdio.h>
#include <string.h>
#include "include/table.h"
#include "include/types.h"

int load_table(Table *table, const char *filename) {
    if (!table) {
        return E_NULLPTR;
    }

    table->filename = strdup(filename);
    if (!table->filename) {
        return E_ALLOC;
    }

    table->fp = fopen(filename, "rb+");
    if (!table->fp) {
        return E_NOFILE;
    }

    rewind(table->fp);
    fread(&table->msize, sizeof(int), 1, table->fp);
    fread(&table->csize, sizeof(int), 1, table->fp);

    return E_OK;
}

int save_table(Table *table) {
    if (!table) {
        return E_NULLPTR;
    }

    if (!table->fp) {
        return E_NOFILE;
    }

    rewind(table->fp);
    fwrite(&table->msize, sizeof(int), 1, table->fp);
    fwrite(&table->csize, sizeof(int), 1, table->fp);
    fflush(table->fp);

    return E_OK;
}

static inline int compare_keys(KeyType k1, KeyType k2, RelType r1, RelType r2, int release) {
    if (release) {
        return (k1 == k2) && (r1 == r2);
    }

    return k1 == k2;
}

Table *init_table() {
    return calloc(1, sizeof(Table));
}

void free_table(Table *table) {
    if (!table) {
        return ;
    }

    free(table->filename);
    free(table);
}

int f_print_element(FILE *fp, const KeySpace *element, const Item *item) {
    if (!fp || !element || !item) {
        return E_NULLPTR;
    }

    KeyType key;
    InfoType data;

    fseek(fp, element->key_offset, SEEK_SET);
    fread(&key, sizeof(KeyType), 1, fp);

    fseek(fp, item->offset, SEEK_SET);
    fread(&data, sizeof(InfoType), 1, fp);

    printf(
            ELEM_FMT,
            element->busy,
            key,
            element->release,
            data
            );

    return E_OK;
}

int f_print_table(Table *table) {
    if (!table) {
        return E_NULLPTR;
    }

    if (!table->fp) {
        return E_NOFILE;
    }

    KeySpace cur_element;
    Item cur_item;

    printf("Busy\tKey\tRelease\tInfo\n");
    for (int i = 0; i < table->csize; ++i) {
        long offset = 2 * sizeof(int) + i * (sizeof(KeySpace) + sizeof(Item));
        fseek(table->fp, offset, SEEK_SET);
        fread(&cur_element, sizeof(KeySpace), 1, table->fp);
        fread(&cur_item, sizeof(Item), 1, table->fp);

        f_print_element(table->fp, &cur_element, &cur_item);
    }

    return E_OK;
}

int f_search(const Table *table, KeyType key, RelType elem_release, int has_release, int *last_idx) {
    if (!table) {
        return E_NULLPTR;
    }

    if (!table->fp) {
        return E_NOFILE;
    }

    KeySpace cur_element;
    KeyType cur_key;

    for (int i = *last_idx; i < table->csize; ++i) {
        long offset = 2 * sizeof(int) + i * (sizeof(KeySpace) + sizeof(Item));
        fseek(table->fp, offset, SEEK_SET);
        fread(&cur_element, sizeof(KeySpace), 1, table->fp);

        if (cur_element.busy) {
            fseek(table->fp, cur_element.key_offset, SEEK_SET);
            fread(&cur_key, sizeof(KeyType), 1, table->fp);

            if (compare_keys(cur_key, key, cur_element.release, elem_release, has_release)) {
                *last_idx = i + 1;
                return i;
            }
        }
    }

    return E_NOTFOUND;
}

int f_remove_element(Table *table, KeyType key, RelType elem_release, int has_release) {
    if (!table) {
        return E_NULLPTR;
    }

    if (!table->fp) {
        return E_NOFILE;
    }

    int last_idx = 0, idx = f_search(table, key, elem_release, has_release, &last_idx);
    while (idx != E_NOTFOUND) {
        KeySpace cur_element;
        long cur_offset = 2 * sizeof(int) + idx * (sizeof(KeySpace) + sizeof(Item));
        fseek(table->fp, cur_offset, SEEK_SET);
        fread(&cur_element, sizeof(KeySpace), 1, table->fp);

        cur_element.busy = 0;

        fseek(table->fp, cur_offset, SEEK_SET);
        fwrite(&cur_element, sizeof(KeySpace), 1, table->fp);

        idx = f_search(table, key, elem_release, has_release, &last_idx);
    }

    return E_OK;
}

int f_insert(Table *table, KeyType key, InfoType data) {
    if (!table) {
        return E_NULLPTR;
    }

    if (!table->fp) {
        return E_NOFILE;
    }

    if (table->csize == table->msize) {
        return E_OVERFLOW;
    }

    int last_idx = 0, tmp_idx = -1;
    int idx = f_search(table, key, 0, 0, &last_idx);
    RelType release = 0;

    while (idx >= 0) {
        tmp_idx = idx;
        idx = f_search(table, key, 0, 0, &last_idx);
    }

    KeySpace tmp_elem;
    if (table->csize && tmp_idx != -1) {
        fseek(table->fp, 2 * sizeof(int) + tmp_idx * (sizeof(KeySpace) + sizeof(Item)), SEEK_SET);
        fread(&tmp_elem, sizeof(KeySpace), 1, table->fp);
        release = tmp_elem.release + 1;
    }

    KeySpace cur_element;
    Item cur_item;

    fseek(table->fp, 0, SEEK_END);

    cur_element.key_offset = ftell(table->fp);
    fwrite(&key, sizeof(KeyType), 1, table->fp);

    cur_item.offset = ftell(table->fp);
    fwrite(&data, sizeof(InfoType), 1, table->fp);

    cur_element.busy = 1;
    cur_element.release = release; // TODO: optimize here without temp element

    long cur_offset = 2 * sizeof(int) + (table->csize) * (sizeof(KeySpace) + sizeof(Item));
    fseek(table->fp, cur_offset, SEEK_SET);
    fwrite(&cur_element, sizeof(KeySpace), 1, table->fp);
    fwrite(&cur_item, sizeof(Item), 1, table->fp);
    table->csize += 1;

    return E_OK;
}
