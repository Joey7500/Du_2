// htab_for_each.c
// Řešení IJC-DU2 příklad 2), 25.04. 2026
// Autor: Josef Hruska, FSI
// Přeloženo: gcc 14.2.0
// Popis: Projde postupně všechny položky v tabulce a aplikuje na ně zadanou funkci.

#include "htab_private.h"

void htab_for_each(const htab_t * t, void (*f)(htab_pair_t *data)) {
    if (t == NULL || f == NULL) {
        return;
    }

    for (size_t i = 0; i < t->arr_size; i++) {
        struct htab_item *current = t->arr_ptrs[i];
        
        while (current != NULL) {
            f(&(current->pair));
            current = current->next;
        }
    }
}

