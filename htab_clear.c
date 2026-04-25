// htab_clear.c
// Řešení IJC-DU2 příklad 2), 25.04. 2026
// Autor: Josef Hruska, FSI
// Přeloženo: gcc 14.2.0
// Popis: Odstraní a uvolní všechny prvky v tabulce.

#include <stdlib.h>
#include "htab_private.h"

void htab_clear(htab_t * t) {
    if (t == NULL) return;

    for (size_t i = 0; i < t->arr_size; i++) {
        struct htab_item *current = t->arr_ptrs[i];
        
        while (current != NULL) {
            struct htab_item *helper = current;
            current = current->next;
            
            free((char *)helper->pair.key); 
            
            free(helper);
        }
        t->arr_ptrs[i] = NULL;
    }
    
    t->size = 0;
}
