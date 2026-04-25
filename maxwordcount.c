// maxwordcount.c
// Řešení IJC-DU2 příklad 2), 25.04. 2026
// Autor: Josef Hruska, FSI
// Přeloženo: gcc 14.2.0
// Popis: Program počítá četnost slov ve vstupním textu a tiskne seznam nejčetnějších slov s využitím vlastní implementace hashovací tabulky.

/*
 * ZAMYŠLENÍ NAD EFEKTIVITOU IMPLEMENTACE
 
 * Měření proběhlo na rozsáhlém textu (kniha.txt z Projektu Gutenberg).
 *
 * 1. Referenční C++ verze (std::unordered_map, g++ -O2):
 * real: 0m0.009s  |  user: 0m0.004s  |  sys: 0m0.005s
 *
 * 2. Moje C implementace (htab, gcc -O2, bez sanitizeru):
 * real: 0m0.005s  |  user: 0m0.000s  |  sys: 0m0.005s
 *
 * 3. Moje C implementace (htab, gcc -O0, s -fsanitize=address):
 * real: 0m0.024s  |  user: 0m0.010s  |  sys: 0m0.008s
 *
 * ZÁVĚR:
 * Při použití optimalizace -O2 je moje implementace v jazyce C téměř 2x 
 * rychlejší než C++ varianta. Je to dáno především tím,
 * že moje implementace využívá dostatečně velkou fixní velikost pole (39989)
 * a neztrácí tak čas dynamickým přehašováním a realokací paměti během behu.
 *
 * Měření s AddressSanitizerem jasně ukazuje značnou režii při zapnutých 
 * kontrolách. Běh se zpomalil zhruba pětinásobně, protože nástroj hlídá 
 * každý paměťový přístup. Paměťové testy ale proběhly čistě a bez úniků.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "htab.h"

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
    // Zvolena velikost pole 39989 (prvočíslo), která zajišťuje rovnoměrné 
    // rozptýlení klíčů a minimalizuje délku spojových seznamů v přihrádkách 
    // i při zpracování velmi rozsáhlých textů (např. celých knih).
    htab_t *table = htab_init(39989);
    if (table == NULL) {
        fprintf(stderr, "Chyba: Nelze alokovat pamet pro tabulku.\n");
        return 1;
    }

    char word[MAX_WORD_LEN];
    bool warning_printed = false;
    int len;

    while ((len = read_word(MAX_WORD_LEN, word, stdin)) != EOF) {
        if (len == MAX_WORD_LEN - 1 && !warning_printed) {
            fprintf(stderr, "Varovani: Prekrocen limit delky slova (%d znaku). Delsi slova budou zkracena.\n", MAX_WORD_LEN - 1);
            warning_printed = true; // Tiskneme max 1 varování
        }
        
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
