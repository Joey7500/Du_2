// htab_size.c
// Řešení IJC-DU2 příklad 2), 25.04. 2026
// Autor: Josef Hruska, FSI
// Přeloženo: gcc 14.2.0
// Popis: Funkce vracející aktuální počet záznamů v tabulce.

#include "htab_private.h"

size_t htab_size(const htab_t * t){
	return t->size;
}
