#include <stdio.h>
#include <string.h>
#include "include/table.h"
#include "include/types.h"
#include "include/files.h"

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

    fseek(table->fp, 0, SEEK_SET);
    fread(&table->msize, sizeof(int), 1, table->fp);
    // fread(&table->csize, sizeof(int), 1, table->fp);

    return E_OK;
}

int save_table(Table *table) {
    if (!table) {
        return E_NULLPTR;
    }

    if (!table->fp) {
        return E_NOFILE;
    }

    fseek(table->fp, 0, SEEK_SET);
    fwrite(&table->msize, sizeof(int), 1, table->fp);
    fwrite(&table->csize, sizeof(int), 1, table->fp);

    return E_OK;
}

static inline int compare_keys(const char *k1, const char *k2) {
    return strcmp(k1, k2);
}

Table *init_table() {
    return calloc(1, sizeof(Table));
}

void print_element(const KeySpace *element) {
    if (!element || !element->key) {
        return ;
    }

    printf( 
            ELEM_FMT, 
            element->busy, 
            element->key, 
            element->par, 
            *(element->info->info)
        );
}

void print_table(const Table *table) {
    printf("Busy\tKey\tPar\tInfo\n");
    for (int i = 0; i < table->csize; ++i) {
        print_element(table->ks + i);
    }
}

void free_element(KeySpace *element) {
    if (!element || !element->key){
        return ;
    }
        
    free(element->info->info);
    free(element->info);
    free(element->key);
    free(element->par);
}

void free_table(Table *table) {
    if (!table) {
        return ;
    }

    for (int i = 0; i < table->csize; ++i) {
        free_element(table->ks + i);
    }

    if (table->csize > 0) {
        free(table->ks);
    }

    free(table->filename);
    free(table);
}

int remove_garbage(Table *table) {
    if (!table) {
        return E_NULLPTR;
    }

    if (!table->ks) {
        return E_NULLPTR;
    }

    for (int i = 0; i < table->csize; ++i) {
        if ((table->ks + i)->busy == 0) {
            free_element(table->ks + i);
            memmove(table->ks + i, table->ks + i + 1, (table->csize - i - 1) * sizeof(KeySpace));
            --table->csize;
            --i;
        }
    }

    if (table->msize == table->csize) {
        return E_OVERFLOW;
    }
    else {
        KeySpace *tmp_ptr = NULL;
        tmp_ptr = realloc(table->ks, table->csize * sizeof(KeySpace));

        if (!tmp_ptr) {
            return E_ALLOC;
        }

        table->ks = tmp_ptr;
        return E_OK;
    }
}

int search(const Table *table, const KeySpace *element, int parent) {
    if (!table || !element) {
        return E_ALLOC;
    }

    if (parent && !(element->par)) {
        return E_NULLPTR;
    }

    for (int i = 0; i < table->csize; ++i) {
        if (parent) {
            if ((table->ks + i)->busy && !compare_keys((table->ks + i)->key, element->par)) {
                return i;
            }
        }
        else {
            if ((table->ks + i)->busy && !compare_keys((table->ks + i)->key, element->key)) {
                return i;
            }
        }
    }

    return E_NOTFOUND;
}

int remove_element(Table *table, const KeySpace *element) {
    if (!table || !element) {
        return E_NULLPTR;
    }

    int idx = search(table, element, 0);
    if (idx == E_NOTFOUND) {
        return E_NOTFOUND;
    }
    KeySpace *cur_elem = table->ks + idx;
    cur_elem->busy = 0;

    for (int i = 0; i < table->csize; ++i) {
        if ((table->ks + i)->par) {
            if (!compare_keys(cur_elem->key, (table->ks + i)->par)) {
                remove_element(table, table->ks + i);
            }
        }
    }

    return E_OK;
}

int insert(Table *table, const KeySpace *element) {
    if (!table || !element) {
        return E_NULLPTR;
    }

    if (element->par) {
        if (search(table, element, 1) == E_NOTFOUND) {
            return E_INSERT;
        }
    }

    if (table->csize == table->msize) {
        if (remove_garbage(table) != E_OK) {
            if (table->csize > 0) {
                return E_OVERFLOW;
            }
        }
    }

    if (search(table, element, 0) != E_NOTFOUND) {
        return E_INSERT;
    }

    KeySpace *tmp_ptr = NULL;
    if (table->csize == 0) {
        tmp_ptr = calloc(1, sizeof(KeySpace));
    }
    else {
        tmp_ptr = realloc(table->ks, (table->csize + 1) * sizeof(KeySpace));
    }

    if (!tmp_ptr) {
        return E_ALLOC;
    }

    table->ks = tmp_ptr;
    *(table->ks + table->csize) = *element;
    (table->ks + table->csize)->busy = 1;
    table->csize += 1;

    return E_OK;
}

int f_print_element(FILE *fp, KeySpace *element, Item *item) {
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
    fread(key, sizeof(char), element->key_len, fp);

    par = calloc(element->par_len + 1, sizeof(char));
    if (!par) {
        free(key);
        return E_ALLOC;
    }
    fseek(fp, element->par_offset, SEEK_SET);
    fread(par, sizeof(char), element->par_len, fp);

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

    KeySpace cur_element;
    Item cur_item;
    int cur_data;

    FILE *tmp = fopen(".tmp", "w+b");

    if (!tmp) {
        return E_NOFILE;
    }

    table->csize = 0;
    fseek(tmp, 0, SEEK_SET);
    fwrite(&table->msize, sizeof(int), 1, tmp);
    fwrite(&table->csize, sizeof(int), 1, tmp);

    char *tmp_str = NULL, *tmp_ptr = NULL;

    for (int i = 0; i < table->csize; i++) {
        long offset = 2 * sizeof(int) + i * (sizeof(KeySpace) + sizeof(Item));
        fseek(table->fp, offset, SEEK_SET);
        fread(&cur_element, sizeof(KeySpace), 1, table->fp);
        fread(&cur_item, sizeof(Item), 1, table->fp);
        if (cur_element.busy) {
            fseek(tmp, 0, SEEK_END);

            tmp_ptr = realloc(tmp_str, (cur_element.key_len + 1) * sizeof(char));
            if (!tmp_ptr) {
                free(tmp_str);
                fclose(tmp);
                return E_ALLOC;
            }
            tmp_str = tmp_ptr;
            fseek(table->fp, cur_element.key_offset, SEEK_SET);
            fread(tmp_str, sizeof(char), cur_element.key_len + 1, table->fp);
            cur_element.key_offset = ftell(tmp);
            fwrite(tmp_str, sizeof(char), cur_element.key_len + 1, tmp);

            if (cur_element.par_len > 0) {
                tmp_ptr = realloc(tmp_str, (cur_element.par_len + 1) * sizeof(char));
                if (!tmp_ptr) {
                    free(tmp_str);
                    fclose(tmp);
                    return E_ALLOC;
                }
                tmp_str = tmp_ptr;
                fseek(table->fp, cur_element.par_offset, SEEK_SET);
                fread(tmp_str, sizeof(char), cur_element.par_len + 1, table->fp);
                cur_element.par_offset = ftell(tmp);
                fwrite(tmp_str, sizeof(char), cur_element.par_len + 1, tmp);
            }

            fseek(table->fp, cur_item.offset, SEEK_SET);
            fread(&cur_data, sizeof(int), 1, table->fp);
            cur_item.offset = ftell(tmp);
            fwrite(&cur_data, sizeof(int), 1, tmp);

            table->csize += 1;
        }
    }

    fclose(tmp);
    fclose(table->fp);
    free(tmp_str);

    remove(table->filename);
    rename(".tmp", table->filename);

    table->fp = fopen(table->filename, "wb+");
    if (!table->fp) {
        return E_NOFILE;
    }
    save_table(table);

    return table->csize == table->msize;
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
    for (int i = 0; i < table->csize; i++) {
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
        }
        free(cur_key);
        cur_key = NULL;
    }

    return E_NOTFOUND;
}

// TODO: maybe pass index instead of key to avoid recursive memory allocation
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
    char *cur_key = NULL;
    long offset = 2 * sizeof(int) + idx * (sizeof(KeySpace) + sizeof(Item));

    fseek(table->fp, offset, SEEK_SET);
    fread(&cur_element, sizeof(KeySpace), 1, table->fp);
    cur_element.busy = 0;
    fseek(table->fp, offset, SEEK_SET);
    fwrite(&cur_element, sizeof(KeySpace), 1, table->fp);

    KeySpace loop_element; // TODO: check if local variable fails in recursion
    char *loop_par = NULL;
    long loop_offset;

    for (int i = 0; i < table->csize; ++i) {
        loop_offset = 2 * sizeof(int) + i * (sizeof(KeySpace) + sizeof(Item));
        fseek(table->fp, loop_offset, SEEK_SET);
        fread(&loop_element, sizeof(KeySpace), 1, table->fp);

        if (loop_element.par_len == 0) {
            continue;
        }

        loop_par = calloc(loop_element.par_len + 1, sizeof(char));
        if (!loop_par) {
            return E_ALLOC;
        }
        fseek(table->fp, loop_element.par_offset, SEEK_SET);
        fread(loop_par, sizeof(char), loop_element.par_len + 1, table->fp);
        if (!compare_keys(loop_par, key)) {
            f_remove_element(table, loop_par);
            // free(loop_par);
            // loop_par = NULL;
        }
    }

    return E_OK;
}

int f_insert(Table *table, const char *key, const char *par, int data) {
    if (!table) {
        return E_NULLPTR;
    }

    if (!table->fp) {
        return E_NOFILE;
    }

    int idx;
    if ((idx = f_search(table, key)) != E_NOTFOUND) {
        return E_INSERT;
    }

    if (table->csize == table->msize) {
        /* if (f_remove_garbage(table) != E_OK) {
            return E_OVERFLOW;
        } */
        return E_OVERFLOW;
    }

    KeySpace cur_element;
    Item cur_item;

    fseek(table->fp, 0, SEEK_END);

    cur_element.key_offset = ftell(table->fp);
    cur_element.key_len = strlen(key);
    fwrite(key, sizeof(char), cur_element.key_len + 1, table->fp);

    if (par) {
        cur_element.par_offset = ftell(table->fp);
        cur_element.par_len = strlen(par);
        fwrite(par, sizeof(char), cur_element.par_len + 1, table->fp);
    } else {
        cur_element.par_offset = -1;
        cur_element.par_len = 0;
    }

    cur_item.offset = ftell(table->fp);
    fwrite(&data, sizeof(int), 1, table->fp);

    cur_element.busy = 1;

    long cur_offset = 2 * sizeof(int) + table->csize * (sizeof(KeySpace) + sizeof(Item));
    fseek(table->fp, cur_offset, SEEK_SET);
    fwrite(&cur_element, sizeof(KeySpace), 1, table->fp);
    fwrite(&cur_item, sizeof(Item), 1, table->fp);
    table->csize += 1;

    return E_OK;
}
