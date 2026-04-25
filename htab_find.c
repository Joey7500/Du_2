// htab_find.c
// Řešení IJC-DU2 příklad 2), 25.04. 2026
// Autor: Josef Hruska, FSI
// Přeloženo: gcc 14.2.0
// Popis: Vyhledá záznam v hashovací tabulce podle klíče a vrátí na něj ukazatel.

#include <string.h>
#include "htab_private.h"

htab_pair_t *htab_find(const htab_t * t, htab_key_t key) {
    size_t index = (htab_hash_function(key) % t->arr_size);

    struct htab_item *current = t->arr_ptrs[index];

    while (current != NULL) {
        if (strcmp(current->pair.key, key) == 0) {
            return &(current->pair);
        }
        current = current->next;
    }

    return NULL;
}
