// io.c
// Řešení IJC-DU2 příklad 2), 25.04. 2026
// Autor: Josef Hruska, FSI
// Přeloženo: gcc 14.2.0
// Popis: Modul pro bezpečné čtení slov ze souboru s omezením maximální délky.

/* 
 * POZNÁMKA K LOKALIZACI A KÓDOVÁNÍ UTF-8
 
 * Při zapnuté lokalizaci a použití kódování UTF-8 by s tímto kódem mohl 
 * nastat problém, jelikož UTF-8 znaky mají proměnlivou délku (1 až 4 bajty).
 * Standardní C funkce jako fgetc() nebo isspace() pracují po jednotlivých 
 * bajtech (char), nikoliv po celých UTF-8 znacích.
 * * To by mohlo vést k tomu, že limit délky slova omezí počet bajtů, a nikoliv
 * skutečných znaků, čímž by mohlo dojít k "rozseknutí" vícebajtového znaku.
 */

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

int read_word(unsigned max, char s[max], FILE *f) {
    if (f == NULL || max <= 1) {
        return EOF;
    }

    int c;
    while ((c = fgetc(f)) != EOF && isspace(c)) {
    }

    if (c == EOF) {
        return EOF;
    }

    unsigned i = 0;
    static bool warning_printed = false;
    bool is_too_long = false;

    while (c != EOF && !isspace(c)) {
        if (i < max - 1) {
            s[i] = c;
            i++;
        } else {
            is_too_long = true; // Znak se nevejde, zahazujeme
        }
        c = fgetc(f);
    }

    s[i] = '\0';

    if (is_too_long && !warning_printed) {
        fprintf(stderr, "Varovani: Prekrocen limit delky slova. Delsi slova budou zkracena.\n");
        warning_printed = true;
    }

    return i;
}
