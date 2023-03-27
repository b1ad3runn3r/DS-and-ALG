#include "dialog.h"
#include "utils.h"
#include "types.h"
#include "table.h"
#include <stdio.h>
#include <string.h>

void print_opts(const char *opts[], size_t size) {
    for (int i = 0; i < size; ++i) {
        printf("%s\n", opts[i]);
    }
}

int dialog(int opts_size ) {
    int choice = 0;
    int status = 0;

    do {
        status = get_int("Enter option: ", &choice);
        if (status != E_OK) {
            printf("Wrong input! Try again.");
            continue;
        }
    } while (choice < 0 || choice >= opts_size);

    return choice;
}

static KeySpace *create_element(char* key, char* par, int data) {
    if (!key){
        return NULL;
    }

    KeySpace *element = calloc(1, sizeof(KeySpace));
    if (!element) {
        return NULL;
    }

    Item *item = calloc(1, sizeof(Item));
    if (!item) {
        free(element);
        return NULL;
    }

    item->info = calloc(1, sizeof(InfoType));
    if (!item->info) {
        free(item);
        free(element);
        return NULL;
    }

    *(item->info) = data;
    element->key = key;
    element->par = par;
    element->info = item;

    return element;
}

int d_insert(Table *table) {
    char *key = readline("Enter key: ");
    if (!key) {
        return E_ALLOCERROR;
    }

    char *par = readline("Enter parent key: ");
    if (!par) {
        free(key);
        return E_ALLOCERROR;
    }
    if (par[0] == '\0') {
        free(par);
        par = NULL;
    }

    int data = 0;
    if (get_int("Enter data: ", &data) != E_OK) {
        free(key);
        free(par);
        return E_WRONGINPUT;
    }

    KeySpace *element = create_element(key, par, data);
    if (!element) {
        free(key);
        free(par);
        return E_WRONGINPUT;
    }

    if (insert(table, element) != E_OK) {
        free_element(element);
    }
    free(element);

    return E_OK;
}

int d_remove(Table *table) {
    char *key = readline("Enter key: ");
    if (!key) {
        return E_WRONGINPUT;
    }

    KeySpace *element = create_element(key, NULL, 0);
    if (element) {
        remove_element(table, element);
    }

    free_element(element);
    free(element);

    return E_OK;
}

int d_search(Table *table) {
    char *key = readline("Enter key: ");
    if (!key) {
        return E_WRONGINPUT;
    }
    KeySpace *element = create_element(key, NULL, 0);
    if (element) {
        IndexType found = search(table, element, 0);
        if (found != E_NOTFOUND) {
            printf("Found: %d\n", found);
            print_element(table->ks + found);
        }
    }
    free_element(element);
    free(element);
    return E_OK;
}

int d_print(Table *table) {
    print_table(table);
    return E_OK;
}

