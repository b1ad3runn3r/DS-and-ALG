#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "include/table.h"
#include "include/types.h"

static void read_params(FILE *fp, int* param1, int *param2) {
    if (!fp) {
        return;
    }

    rewind(fp);
    fread(param1, sizeof(int), 1, fp);
    fread(param2, sizeof(int), 1, fp);
}

static void read_at(FILE *fp, long offset, void *ptr, size_t size, int mode) {
    if (!fp) {
        return ;
    }

    fseek(fp, offset, mode);
    fread(ptr, size, 1, fp);
}

static void write_at(FILE *fp, long offset, void *ptr, size_t size, int mode) {
    if (!fp) {
        return ;
    }

    fseek(fp, offset, mode);
    fwrite(ptr, size, 1, fp);
}

int load_table(Table *table, const char *filename) {
    if (!table) {
        return E_NULLPTR;
    }

    table->fp = fopen(filename, "rb+");
    if (!table->fp) {
        return E_NOFILE;
    }

    int buf = 0;
    read_params(table->fp, &(table->msize), &(buf));

    return E_OK;
}

static void write_params(FILE *fp, int *param1, int *param2) {
    if (!fp) {
        return;
    }

    rewind(fp);
    fwrite(param1, sizeof(int), 1, fp);
    fwrite(param2, sizeof(int), 1, fp);
}

int save_table(Table *table) {
    if (!table) {
        return E_NULLPTR;
    }

    if (!table->fp) {
        return E_NOFILE;
    }

    int buf = 0;
    write_params(table->fp, &(table->msize), &(buf));
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

    free(table);
}

int f_print_element(FILE *fp, const KeySpace *element, const Item *item) {
    if (!fp || !element || !item) {
        return E_NULLPTR;
    }

    if (element->key_offset == 0) {
        return E_NULLPTR;
    }

    KeyType key;
    InfoType data;

    read_at(fp, element->key_offset, &key, sizeof(KeyType), SEEK_SET);
    read_at(fp, item->offset, &data, sizeof(InfoType), SEEK_SET);

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

    printf("Busy");
    printf("\tKey");
    printf("\tPar");
    printf("\tInfo\n");
    for (int i = 0; i < table->msize; ++i) {
        long offset = sizeof(int) + i * (sizeof(KeySpace) + sizeof(Item));

        read_at(table->fp, offset, &cur_element, sizeof(KeySpace), SEEK_SET);
        read_at(table->fp, offset + sizeof(KeySpace), &cur_item, sizeof(Item), SEEK_SET);

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
        read_at(table->fp, offset, &cur_element, sizeof(KeySpace), SEEK_SET);

        if (cur_element.busy) {
            read_at(table->fp, cur_element.key_offset, &cur_key, sizeof(size_t), SEEK_SET);

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
        read_at(table->fp, cur_offset, &cur_element, sizeof(KeySpace), SEEK_SET);

        cur_element.busy = 0;

        write_at(table->fp, cur_offset, &cur_element, sizeof(KeySpace), SEEK_SET);

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

        KeySpace cur_element;
        Item cur_item;

        read_at(table->fp, offset, &cur_element, sizeof(KeySpace), SEEK_SET);
        read_at(table->fp, offset + sizeof(KeySpace), &cur_item, sizeof(Item), SEEK_SET);

        if (cur_element.busy) {
            read_at(table->fp, cur_element.key_offset, &tmp_key, sizeof(KeyType), SEEK_SET);

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
                write_at(table->fp, 0, &key, sizeof(KeyType), SEEK_END);

                cur_item.offset = ftell(table->fp);
                write_at(table->fp, 0 + cur_element.key_offset, &data, sizeof(InfoType), SEEK_END);


            }
            else {
                write_at(table->fp, cur_element.key_offset, &key, sizeof(KeyType), SEEK_SET);
                write_at(table->fp ,cur_item.offset, &data, sizeof(InfoType), SEEK_SET);
            }

            write_at(table->fp, offset, &cur_element, sizeof(KeySpace), SEEK_SET);
            write_at(table->fp, offset + sizeof(KeySpace), &cur_item, sizeof(Item), SEEK_SET);

            return E_OK;
        }
    }

    return E_OK;
}
