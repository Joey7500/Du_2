// htab_lookup_add.c
// Řešení IJC-DU2 příklad 2), 25.04. 2026
// Autor: Josef Hruska, FSI
// Přeloženo: gcc 14.2.0
// Popis: Vyhledá záznam podle klíče. Pokud neexistuje, vytvoří a přidá nový.

#include <stdlib.h>
#include <string.h>
#include "htab_private.h"

htab_pair_t *htab_lookup_add(htab_t * t, htab_key_t key) {
    htab_pair_t *found = htab_find(t, key);
    if (found != NULL) {
        return found;
    }

    char *new_key = malloc(strlen(key) + 1);
    if (new_key == NULL) return NULL;
    strcpy(new_key, key);

    struct htab_item *new_item = malloc(sizeof(struct htab_item));
    if (new_item == NULL) {
        free(new_key);
        return NULL;
    }

    new_item->pair.key = new_key;
    new_item->pair.value = 0;

    size_t index = (htab_hash_function(key) % t->arr_size);
    new_item->next = t->arr_ptrs[index];
    t->arr_ptrs[index] = new_item;

    t->size++;

    return &(new_item->pair);
}
