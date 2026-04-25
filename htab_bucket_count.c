// htab_bucket_count.c
// Řešení IJC-DU2 příklad 2), 25.04. 2026
// Autor: Josef Hruska, FSI
// Přeloženo: gcc 14.2.0
// Popis: Funkce vracející celkovou velikost pole ukazatelů.

#include "htab_private.h"

size_t htab_bucket_count(const htab_t * t){
	return t->arr_size;
}
