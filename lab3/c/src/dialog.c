#include "include/dialog.h"
#include "include/utils.h"
#include "include/types.h"
#include "include/table.h"
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

int choose_release(KeySpace *element) {
    const char *opts[2] = {
        "0. Compare without release",
        "1. Compare with release"
    };

    RelType release = 0;

    print_opts(opts, 2);
    int choice = dialog(2);
    if (choice) {
        do {
            if (get_size_t("Enter release: ", &release) != E_OK) {
                return E_WRONGRELEASE;
            }
        } while (release < 0);
    }

    element->release = release;
    return choice;
}

static KeySpace *create_element(KeyType key, InfoType data) {
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
    element->info = item;

    return element;
}

int d_insert(Table *table) {
    KeyType key = 0;
    if (get_size_t("Enter key: ", &key) != E_OK) {
        return E_WRONGINPUT;
    }

    InfoType data = 0;
    if (get_size_t("Enter data: ", &data) != E_OK) {
        return E_WRONGINPUT;
    }

    KeySpace *element = create_element(key, data);
    if (!element) {
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
    KeyType key = 0;
    if (get_size_t("Enter key: ", &key) != E_OK) {
        return E_WRONGINPUT;
    }

    KeySpace *element = create_element(key, 0);

    if (element) {
        int release = choose_release(element);

        if (release != E_WRONGRELEASE) {
            remove_element(table, element, release);
        }
        else {
            parse_result(E_NOTFOUND);
        }
    }

    free_element(element);
    free(element);

    return E_OK;
}

int d_search(Table *table) {
    KeyType key = 0;
    if (get_size_t("Enter key: ", &key) != E_OK) {
        return E_WRONGINPUT;
    }

    KeySpace *element = create_element(key, 0);
    if (element) {
        int release = choose_release(element);
        IndexType last_idx = 0, idx = 0;

        if (release != E_WRONGRELEASE) {
            printf("Busy\tKey\tRelease\tInfo\n");
            idx = search(table, element, release, &last_idx);
            while (idx >= 0) {
                print_element(table->ks + idx);
                idx = search(table, element, release, &last_idx);
            }
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

