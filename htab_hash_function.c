// htab_hash_function.c
// Řešení IJC-DU2 příklad 2), 25.04. 2026
// Autor: Josef Hruska, FSI
// Přeloženo: gcc 14.2.0
// Popis: Implementace hashovaci funkce pro výpočet indexu z klíče.

#include <stdint.h>
#include "htab.h"

size_t htab_hash_function(htab_key_t str) {
    uint32_t h = 0;
    const unsigned char *p;
    for (p = (const unsigned char*)str; *p != '\0'; p++) {
        h = 65599 * h + *p;
    }
    return h;
}
