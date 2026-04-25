// htab_free.c
// Řešení IJC-DU2 příklad 2), 25.04. 2026
// Autor: Josef Hruska, FSI
// Přeloženo: gcc 14.2.0
// Popis: Destruktor tabulky.

#include <stdlib.h>
#include "htab_private.h"

void htab_free(htab_t * t) {
    if (t != NULL) {
        htab_clear(t); 
        free(t);
    }
}
