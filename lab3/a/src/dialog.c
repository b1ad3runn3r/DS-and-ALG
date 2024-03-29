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

    InfoType *data_p = calloc(1, sizeof(InfoType));
    if (!data_p) {
        free(key);
        free(par);
        return E_ALLOC;
    }

    int status = insert(table, key, par, data_p);
    if (status != E_OK) {
        free(data_p);
        free(key);
        free(par);
        return status;
    }

    parse_result(status);
    return E_OK;
}

int d_remove(Table *table) {
    char *key = readline("Enter key: ");
    if (!key) {
        return E_WRONGINPUT;
    }

    int status = remove_element(table, key);
    parse_result(status);

    free(key);
    return E_OK;
}

int d_search(Table *table) {
    char *key = readline("Enter key: ");
    if (!key) {
        return E_WRONGINPUT;
    }

    IndexType found = search(table, key, NULL, 0);
    if (found != E_NOTFOUND) {
        printf("Found: %d\n", found);
        print_element(table->ks + found);
    }
    else {
        parse_result(E_NOTFOUND);
    }

    free(key);
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

