#include <stdio.h>
#include <string.h>
#include <limits.h>
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

static inline int hash1(const Table *table, KeyType key) {
    int hash = INT_MAX;
    for (int i = 0; i < sizeof(key); ++i) {
        hash = 37 * hash + ((char *)&key)[i];
    }

    return abs(hash) % table->msize;
}

static inline int hash2(const Table *table, KeyType key) {
    return key % (table->msize - 1) + 1;
}

static inline int hash(const Table *table, KeyType key, int i) {
    return (hash1(table, key) + i * hash2(table, key)) % table->msize;
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
    for (int i = 0; i < table->msize; ++i) {
        long offset = sizeof(int) + i * (sizeof(KeySpace) + sizeof(Item));
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

    for (int i = *last_idx; i < table->msize; ++i) {
        int idx = hash(table, key, i);

        long offset = sizeof(int) + idx * (sizeof(KeySpace) + sizeof(Item));
        fseek(table->fp, offset, SEEK_SET);
        fread(&cur_element, sizeof(KeySpace), 1, table->fp);

        if (cur_element.busy) {
            fseek(table->fp, cur_element.key_offset, SEEK_SET);
            fread(&cur_key, sizeof(KeyType), 1, table->fp);

            if (compare_keys(cur_key, key, cur_element.release, elem_release, has_release)) {
                *last_idx = i + 1;
                return idx;
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
        long cur_offset = sizeof(int) + idx * (sizeof(KeySpace) + sizeof(Item));
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

    RelType release = 0;
    KeyType tmp_key;
    for (int i = 0; i < table->msize; ++i) {
        int idx = hash(table, key, i);
        long offset = sizeof(int) + idx * (sizeof(KeySpace) + sizeof(Item));
        fseek(table->fp, offset, SEEK_SET);

        KeySpace cur_element;
        Item cur_item;
        fread(&cur_element, sizeof(KeySpace), 1, table->fp);
        fread(&cur_item, sizeof(Item), 1, table->fp);

        if (cur_element.busy) {
            fseek(table->fp, cur_element.key_offset, SEEK_SET);
            fread(&tmp_key, sizeof(KeyType), 1, table->fp);

            if (compare_keys(tmp_key, key, 0, 0, 0)) {
                release += 1;
                continue;
            }
        }

        if (!cur_element.busy) {
            cur_element.busy = 1;
            cur_element.release = release;

            if (cur_element.key_offset == 0) {
                fseek(table->fp, 0, SEEK_END);

                cur_element.key_offset = ftell(table->fp);
                fwrite(&key, sizeof(KeyType), 1, table->fp);

                cur_item.offset = ftell(table->fp);
                fwrite(&data, sizeof(InfoType), 1, table->fp);
            }
            else {
                fseek(table->fp, cur_element.key_offset, SEEK_SET);
                fwrite(&key, sizeof(KeyType), 1, table->fp);

                fseek(table->fp, cur_item.offset, SEEK_SET);
                fwrite(&data, sizeof(InfoType), 1, table->fp);
            }

            fseek(table->fp, offset, SEEK_SET);
            fwrite(&cur_element, sizeof(KeySpace), 1, table->fp);
            fwrite(&cur_item, sizeof(Item), 1, table->fp); // TODO: put under condition


            return E_OK;
        }
    }

    return E_OK;
}
