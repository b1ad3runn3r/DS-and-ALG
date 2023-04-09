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

static inline int compare_keys(const char *k1, const char *k2) {
    if (!k1 || !k2) {
        return 1;
    }

    return strcmp(k1, k2);
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

    char *key = NULL, *par = NULL;
    int data;

    key = calloc(element->key_len + 1, sizeof(char));
    if (!key) {
        return E_ALLOC;
    }
    fseek(fp, element->key_offset, SEEK_SET);
    fread(key, sizeof(char), element->key_len + 1, fp);

    if (element->par_len > 0) {
        par = calloc(element->par_len + 1, sizeof(char));
        if (!par) {
            free(key);
            return E_ALLOC;
        }
        fseek(fp, element->par_offset, SEEK_SET);
        fread(par, sizeof(char), element->par_len + 1, fp);
    }

    fseek(fp, item->offset, SEEK_SET);
    fread(&data, sizeof(int), 1, fp);

    printf(
            ELEM_FMT,
            element->busy,
            key,
            par,
            data
            );

    free(par);
    free(key);
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

    printf("Busy\tKey\tPar\tInfo\n");
    for (int i = 0; i < table->csize; ++i) {
        long offset = 2 * sizeof(int) + i * (sizeof(KeySpace) + sizeof(Item));
        fseek(table->fp, offset, SEEK_SET);
        fread(&cur_element, sizeof(KeySpace), 1, table->fp);
        fread(&cur_item, sizeof(Item), 1, table->fp);
        f_print_element(table->fp, &cur_element, &cur_item);
    }

    return E_OK;
}

int f_remove_garbage(Table *table) {
    if (!table) {
        return E_NULLPTR;
    }

    if (!table->fp) {
        return E_NOFILE;
    }

    FILE *tmp = fopen(".tmp", "w+b");
    if (!tmp) {
        return E_NOFILE;
    }

    rewind(tmp);
    fwrite(&table->msize, sizeof(int), 1, tmp);
    fwrite(&table->csize, sizeof(int), 1, tmp);

    char *tmp_s = calloc(table->msize, sizeof(KeySpace) + sizeof(Item));
    if (!tmp_s) {
        return E_ALLOC;
    }

    fwrite(tmp_s, sizeof(KeySpace) + sizeof(Item), table->msize, tmp);
    free(tmp_s);

    char *key = NULL, *par = NULL;
    int cur_size = 0;


    for (int i = 0; i < table->csize; ++i) {
        KeySpace cur_element;
        memset(&cur_element, 0, sizeof(KeySpace));
        Item cur_item;
        memset(&cur_item, 0, sizeof(Item));
        int data = 0;

        long offset = 2 * sizeof(int) + i * (sizeof(KeySpace) + sizeof(Item));
        fseek(table->fp, offset, SEEK_SET);
        fread(&cur_element, sizeof(KeySpace), 1, table->fp);

        if (cur_element.busy) {
            fread(&cur_item, sizeof(Item), 1, table->fp);

            key = calloc(cur_element.key_len + 1, sizeof(char));
            if (!key) {
                fclose(tmp);
                return E_ALLOC;
            }

            fseek(table->fp, cur_element.key_offset, SEEK_SET);
            fread(key, cur_element.key_len + 1, sizeof(char), table->fp);

            fseek(tmp, 0, SEEK_END);
            cur_element.key_offset = ftell(tmp);
            fwrite(key, sizeof(char), cur_element.key_len + 1, tmp);
            free(key);

            if (cur_element.par_len > 0) {
                par = calloc(cur_element.par_len + 1, sizeof(char));
                if (!par) {
                    fclose(tmp);
                    return E_ALLOC;
                }

                fseek(table->fp, cur_element.par_offset, SEEK_SET);
                fread(par, cur_element.par_len + 1, sizeof(char), table->fp);

                cur_element.par_offset = ftell(tmp);
                fwrite(par, sizeof(char), cur_element.par_len + 1, tmp);
                free(par);
            }

            fseek(table->fp, cur_item.offset, SEEK_SET);
            fread(&data, sizeof(int), 1, table->fp);

            cur_item.offset = ftell(tmp);
            fwrite(&data, sizeof(int), 1, tmp);

            long tmp_offset = 2 * sizeof(int) + cur_size * (sizeof(KeySpace) + sizeof(Item));
            fseek(tmp, tmp_offset, SEEK_SET);
            fwrite(&cur_element, sizeof(KeySpace), 1, tmp);
            fwrite(&cur_item, sizeof(Item), 1, tmp);
            cur_size += 1;
        }
    }
    table->csize = cur_size;

    fclose(tmp);
    fclose(table->fp);

    rename(".tmp", table->filename);
    table->fp = fopen(table->filename, "r+b");
    if (!table->fp) {
        return E_NOFILE;
    }

    rewind(table->fp);
    fwrite(&table->msize, sizeof(int), 1, table->fp);
    fwrite(&table->csize, sizeof(int), 1, table->fp);

    return table->msize == table->csize;
}

int f_search(const Table *table, const char *key) {
    if (!table) {
        return E_NULLPTR;
    }

    if (!table->fp) {
        return E_NOFILE;
    }

    KeySpace cur_element;
    char *cur_key = NULL;
    for (int i = 0; i < table->csize; ++i) {
        long offset = 2 * sizeof(int) + i * (sizeof(KeySpace) + sizeof(Item));
        fseek(table->fp, offset, SEEK_SET);
        fread(&cur_element, sizeof(KeySpace), 1, table->fp);
        if (cur_element.busy) {
            cur_key = calloc(cur_element.key_len + 1, sizeof(char));
            if (!cur_key) {
                return E_ALLOC;
            }
            fseek(table->fp, cur_element.key_offset, SEEK_SET);
            fread(cur_key, sizeof(char), cur_element.key_len + 1, table->fp);
            if (!compare_keys(cur_key, key)) {
                free(cur_key);
                return i;
            }
            free(cur_key);
            cur_key = NULL;
        }
    }

    return E_NOTFOUND;
}

int f_remove_element(Table *table, const char *key) {
    if (!table) {
        return E_NULLPTR;
    }

    if (!table->fp) {
        return E_NOFILE;
    }

    int idx = f_search(table, key);
    if (idx == E_NOTFOUND) {
        return E_NOTFOUND;
    }

    KeySpace cur_element;
    long cur_offset = 2 * sizeof(int) + idx * (sizeof(KeySpace) + sizeof(Item));
    fseek(table->fp, cur_offset, SEEK_SET);
    fread(&cur_element, sizeof(KeySpace), 1, table->fp);

    cur_element.busy = 0;

    fseek(table->fp, cur_offset, SEEK_SET);
    fwrite(&cur_element, sizeof(KeySpace), 1, table->fp);

    for (int i = 0; i < table->csize; ++i) {
        KeySpace loop_element;
        long loop_offset = 2 * sizeof(int) + i * (sizeof(KeySpace) + sizeof(Item));
        fseek(table->fp, loop_offset, SEEK_SET);
        fread(&loop_element, sizeof(KeySpace), 1, table->fp);

        if (loop_element.par_len > 0) {
            char *par = calloc(loop_element.par_len + 1, sizeof(char));
            if (!par) {
                return E_ALLOC;
            }
            fseek(table->fp, loop_element.par_offset, SEEK_SET);
            fread(par, sizeof(char), loop_element.par_len + 1, table->fp);
            if (!compare_keys(par, key)) {
                f_remove_element(table, par);
            }
            free(par);
        }
    }

    return E_OK;
}

int f_insert(Table *table, const char *key, const char *par, const int data) {
    if (!table) {
        return E_NULLPTR;
    }

    if (!table->fp) {
        return E_NOFILE;
    }

    if (par) {
        if (f_search(table, par) == E_NOTFOUND) {
            return E_INSERT;
        }
    }

    if (f_search(table, key) != E_NOTFOUND) {
        return E_INSERT;
    }

    if (table->csize == table->msize) {
        if (f_remove_garbage(table) != 0) {
            return E_OVERFLOW;
        }
    }

    KeySpace cur_element;
    Item cur_item;

    fseek(table->fp, 0, SEEK_END);

    cur_element.key_offset = ftell(table->fp);
    cur_element.key_len = (long) strlen(key);
    fwrite(key, sizeof(char), cur_element.key_len + 1, table->fp);

    if (par) {
        cur_element.par_offset = ftell(table->fp);
        cur_element.par_len = (long) strlen(par);
        fwrite(par, sizeof(char), cur_element.par_len + 1, table->fp);
        fflush(table->fp);
    } else {
        cur_element.par_offset = 0;
        cur_element.par_len = 0;
    }

    cur_item.offset = ftell(table->fp);
    fwrite(&data, sizeof(int), 1, table->fp);

    cur_element.busy = 1;

    long cur_offset = 2 * sizeof(int) + (table->csize) * (sizeof(KeySpace) + sizeof(Item));
    fseek(table->fp, cur_offset, SEEK_SET);
    fwrite(&cur_element, sizeof(KeySpace), 1, table->fp);
    fwrite(&cur_item, sizeof(Item), 1, table->fp);
    table->csize += 1;

    return E_OK;
}
