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

int dialog(int opts_size) {
    int choice = 0;
    int status = 0;

    do {
        status = get_int("Enter option: ", &choice);
        if (status != E_OK) {
            printf("Wrong input! Try again.\n");
        }
    } while (choice < 0 || choice >= opts_size || status != E_OK);

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

    item->info = calloc(1, sizeof(int));
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
        return E_ALLOC;
    }

    char *par = readline("Enter parent key: ");
    if (!par) {
        free(key);
        return E_ALLOC;
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

    int status = insert(table, element);
    if (status != E_OK) {
        free_element(element);
    }
    parse_result(status);
    free(element);

    return E_OK;
}

int d_remove(Table *table) {
    char *key = readline("Enter key: ");
    if (!key) {
        return E_WRONGINPUT;
    }

    KeySpace *element = create_element(key, NULL, 0);
    int status = E_ALLOC;

    if (element) {
        status = remove_element(table, element);
    }

    parse_result(status);

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
        int found = search(table, element, 0);
        if (found >= E_OK) {
            printf("Found: %d\n", found);
            print_element(table->ks + found);
        }
        else {
            parse_result(E_NOTFOUND);
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

int d_garbage(Table *table) {
    remove_garbage(table);
    return E_OK;
}

void parse_result(int result) {
    switch (result) {
        case E_OK:
            break;
        case E_ALLOC:
            printf ("Memory allocation error!\n");
            break;
        case E_WRONGINPUT:
            printf ("Wrong input!\n");
            break;
        case E_NOTFOUND:
            printf ("Not found!\n");
            break;
        case E_INSERT:
            printf ("Insert error!\n");
            break;
        case E_OVERFLOW:
            printf ("Overflow!\n");
            break;
        case E_NULLPTR:
            printf ("Null pointer!\n");
            break;
        default:
            printf ("Unknown error!\n");
            break;
    }
}
