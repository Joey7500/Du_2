// htab_erase.c
// Řešení IJC-DU2 příklad 2), 25.04. 2026
// Autor: Josef Hruska, FSI
// Přeloženo: gcc 14.2.0
// Popis: Vyhledá a bezpečně odstraní zadaný záznam z tabulky, uvolni pamet.

#include <stdlib.h>
#include <string.h>
#include "htab_private.h"

bool htab_erase(htab_t * t, htab_key_t key) {
    if (t == NULL || key == NULL) {
        return false;
    }

    size_t index = (htab_hash_function(key) % t->arr_size);
    
    struct htab_item *current = t->arr_ptrs[index];
    struct htab_item *prev = NULL;

    while (current != NULL) {
        if (strcmp(current->pair.key, key) == 0) {

            if (prev == NULL) {
                t->arr_ptrs[index] = current->next;
            } else {
                prev->next = current->next;
            }

            free((char *)current->pair.key);
            free(current);

            t->size--;

            return true;
        }
        
        prev = current;
        current = current->next;
    }

    return false;
}

