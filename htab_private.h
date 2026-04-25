// htab_private.h
// Řešení IJC-DU2 příklad 2), 25.04. 2026
// Autor: Josef Hruska, FSI
// Přeloženo: gcc 14.2.0
// Popis: Privátní hlavičkový soubor s definicí struktur pro knihovnu htab.

#ifndef HTAB_PRIVATE_H__
#define HTAB_PRIVATE_H__

#include "htab.h"

struct htab_item {
    struct htab_item *next;
    htab_pair_t pair;
};

struct htab{
	size_t size;
	size_t arr_size;
	struct htab_item *arr_ptrs[];
};

#endif
