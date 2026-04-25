// htab_init.c
// Řešení IJC-DU2 příklad 2), 25.04. 2026
// Autor: Josef Hruska, FSI
// Přeloženo: gcc 14.2.0
// Popis: Konstruktor tabulky - alokuje paměť pro strukturu tabulky a pole ukazatelů.

#include <stdlib.h>
#include "htab_private.h"

htab_t *htab_init(size_t n) {
    htab_t *t = malloc(sizeof(struct htab) + n * sizeof(struct htab_item *));
    
    if (t == NULL) {
        return NULL;
    }

    t->size = 0;
    t->arr_size = n;

    for (size_t i = 0; i < n; i++) {
        t->arr_ptrs[i] = NULL;
    }

    return t;
}
