// maxwordcount.c
// Řešení IJC-DU2 příklad 2), 25.04. 2026
// Autor: Josef Hruska, FSI
// Přeloženo: gcc 14.2.0
// Popis: Program počítá četnost slov ve vstupním textu a tiskne seznam nejčetnějších slov s využitím vlastní implementace hashovací tabulky.

/*
 * Měření proběhlo na dvou textech: kratší kniha.txt a velký big_input.txt (2M slov).
 *
 * 1. Referenční C++ verze (std::unordered_map, g++ -O2):
 * [big_input] real: 0m0.627s  |  user: 0m0.610s  |  sys: 0m0.017s
 * [kniha.txt] real: 0m0.015s  |  user: 0m0.015s  |  sys: 0m0.001s
 *
 * 2. Moje C implementace - STATICKÁ (libhtab.a, gcc -O2):
 * [big_input] real: 0m0.315s  |  user: 0m0.295s  |  sys: 0m0.021s
 * [kniha.txt] real: 0m0.009s  |  user: 0m0.003s  |  sys: 0m0.004s
 *
 * 3. Moje C implementace - DYNAMICKÁ (libhtab.so, gcc -O2):
 * [big_input] real: 0m0.368s  |  user: 0m0.359s  |  sys: 0m0.009s
 * [kniha.txt] real: 0m0.009s  |  user: 0m0.004s  |  sys: 0m0.005s
 *
 * 
 * C vs C++: 
 * Moje implementace v C je zhruba 2x rychlejší než referenční
 * C++ verze. C++ využívá std::string (časté alokace) a std::unordered_map
 * musí při rostoucím počtu prvků dynamicky zvětšovat pole a re-hashovat. 
 * Moje řešení načítá do fixního bufferu a využívá velkou tabulku s pevnou
 * velikostí (11007).
 *
 * Statické vs Dynamické: 
 * Dle očekávání je staticky sestavený program 
 * (0.315s) mírně rychlejší než dynamický (0.368s). Dynamické linkování (.so)
 * přidává drobnou režii při volání knihovních funkcí přes překladové tabulky.
 *
 * Sanitizer: 
 * Předchozí měření s -fsanitize=address ukázala zhruba
 * 4x-5x zpomalení celého běhu, což dokazuje značnou režii nástroje při
 * hlídání přístupů do paměti. Správa paměti v kódu je ale bez chyb (0 leaks).
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "htab.h"

// Podmíněný překlad vlastní hashovací funkce (djb2).
// Použití: gcc -DMY_HASH_FUNCTION ...
// Tím se přebije htab_hash_function() z knihovny (platí pro statické sestavení).
#ifdef MY_HASH_FUNCTION
size_t htab_hash_function(htab_key_t str) {
    size_t h = 5381;
    const unsigned char *p;
    for (p = (const unsigned char*)str; *p != '\0'; p++)
        h = ((h << 5) + h) + *p; // h * 33 + c (djb2)
    return h;
}
#endif

#define MAX_WORD_LEN 128 

int read_word(unsigned max, char s[max], FILE * f);

unsigned global_max = 0;

void find_max(htab_pair_t *data) {
    if (data->value > global_max) {
        global_max = data->value;
    }
}

void print_max(htab_pair_t *data) {
    if (data->value == global_max) {
        printf("%s\t%u\n", data->key, data->value);
    }
}

int main(void) {
    // Zvolena velikost pole 11007 (prvočíslo), která zajišťuje rovnoměrné 
    // rozptýlení klíčů a minimalizuje délku spojových seznamů v přihrádkách 
    // i při zpracování velmi rozsáhlých textů (např. celých knih).
    htab_t *table = htab_init(11007);
    if (table == NULL) {
        fprintf(stderr, "Chyba: Nelze alokovat pamet pro tabulku.\n");
        return 1;
    }

    char word[MAX_WORD_LEN];
    int len;

    while ((len = read_word(MAX_WORD_LEN, word, stdin)) != EOF) {
        if (len > 0) { // Ignorujeme prázdná čtení
            htab_pair_t *pair = htab_lookup_add(table, word);
            if (pair == NULL) {
                fprintf(stderr, "Chyba: Selhala alokace pri vkladani slova.\n");
                htab_free(table);
                return 1;
            }
            pair->value++;
        }
    }

    htab_for_each(table, find_max);

    htab_for_each(table, print_max);

    htab_free(table);

    return 0;
}
