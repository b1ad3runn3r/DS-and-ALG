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

static inline int compare_keys(KeyType k1, KeyType k2) {
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

    printf("Busy\tKey\tInfo\n");
    for (int i = 0; i < table->csize; ++i) {
        long offset = 2 * sizeof(int) + i * (sizeof(KeySpace) + sizeof(Item));
        fseek(table->fp, offset, SEEK_SET);
        fread(&cur_element, sizeof(KeySpace), 1, table->fp);
        fread(&cur_item, sizeof(Item), 1, table->fp);

        f_print_element(table->fp, &cur_element, &cur_item);
    }

    return E_OK;
}

int f_search(const Table *table, KeyType key) {
    if (!table) {
        return E_NULLPTR;
    }

    if (!table->fp) {
        return E_NOFILE;
    }

    KeySpace cur_element;
    KeyType cur_key;

    for (int i = 0; i < table->csize; ++i) {
        long offset = 2 * sizeof(int) + i * (sizeof(KeySpace) + sizeof(Item));
        fseek(table->fp, offset, SEEK_SET);
        fread(&cur_element, sizeof(KeySpace), 1, table->fp);

        if (cur_element.busy) {
            fseek(table->fp, cur_element.key_offset, SEEK_SET);
            fread(&cur_key, sizeof(KeyType), 1, table->fp);

            if (compare_keys(cur_key, key)) {
                return i;
            }
        }
    }

    return E_NOTFOUND;
}

int f_remove_element(Table *table, KeyType key) {
    if (!table) {
        return E_NULLPTR;
    }

    if (!table->fp) {
        return E_NOFILE;
    }

    int idx = f_search(table, key);
    if (idx == E_NOTFOUND) {
        return E_WRONGINPUT;
    }

    KeySpace cur_element;
    long cur_offset = 2 * sizeof(int) + idx * (sizeof(KeySpace) + sizeof(Item));
    fseek(table->fp, cur_offset, SEEK_SET);
    fread(&cur_element, sizeof(KeySpace), 1, table->fp);

    cur_element.busy = 0;

    fseek(table->fp, cur_offset, SEEK_SET);
    fwrite(&cur_element, sizeof(KeySpace), 1, table->fp);

    return E_OK;
}

int f_insert(Table *table, KeyType key, InfoType data) {
    if (!table) {
        return E_NULLPTR;
    }

    if (!table->fp) {
        return E_NOFILE;
    }

    if (f_search(table, key) != E_NOTFOUND) {
        return E_INSERT;
    }

    if (table->csize == table->msize) {
        return E_OVERFLOW;
    }

    KeySpace cur_element;
    Item cur_item;

    fseek(table->fp, 0, SEEK_END);

    cur_element.key_offset = ftell(table->fp);
    fwrite(&key, sizeof(KeyType), 1, table->fp);

    cur_item.offset = ftell(table->fp);
    fwrite(&data, sizeof(InfoType), 1, table->fp);

    cur_element.busy = 1;

    long cur_offset = 2 * sizeof(int) + (table->csize) * (sizeof(KeySpace) + sizeof(Item));
    fseek(table->fp, cur_offset, SEEK_SET);
    fwrite(&cur_element, sizeof(KeySpace), 1, table->fp);
    fwrite(&cur_item, sizeof(Item), 1, table->fp);
    table->csize += 1;

    return E_OK;
}
