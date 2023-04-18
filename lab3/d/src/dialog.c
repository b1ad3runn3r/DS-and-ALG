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

int d_insert(Table *table) {
    KeyType key = 0;
    if (get_size_t("Enter key: ", &key) != E_OK) {
        return E_WRONGINPUT;
    }

    InfoType data = 0;
    if (get_size_t("Enter data: ", &data) != E_OK) {
        return E_WRONGINPUT;
    }

    int status = f_insert(table, key, data);
    parse_result(status);

    return E_OK;
}

int d_remove(Table *table) {
    KeyType key = 0;
    if (get_size_t("Enter key: ", &key) != E_OK) {
        return E_WRONGINPUT;
    }

    int status = f_remove_element(table, key);

    parse_result(status);

    return E_OK;
}

int d_search(Table *table) {
    KeyType key = 0;
    if (get_size_t("Enter key: ", &key) != E_OK) {
        return E_WRONGINPUT;
    }

    int found = f_search(table, key);
    if (found >= E_OK) {
        printf("Found: %d\n", found);

        KeySpace cur_element;
        Item cur_item;

        long offset = 2 * sizeof(int) + found * (sizeof(KeySpace) + sizeof(Item));
        fseek(table->fp, offset, SEEK_SET);
        fread(&cur_element, sizeof(KeySpace), 1, table->fp);
        fread(&cur_item, sizeof(Item), 1, table->fp);
        f_print_element(table->fp, &cur_element, &cur_item);
    }
    else {
        parse_result(E_NOTFOUND);
    }

    return E_OK;
}

int d_print(Table *table) {
    f_print_table(table);
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
