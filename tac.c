// tac.c
// Řešení IJC-DU2 příklad 1), 25.04. 2026
// Autor: Josef Hruska, FSI
// Přeloženo: gcc 14.2.0
// Popis: Program čte vstupní soubor (nebo stdin) po řádcích a tiskne je v obráceném pořadí s využitím vlastního dynamicky alokovaného seznamu.

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct node {
    char *line;
    struct node *next;
};

void list_init(struct node **ptr) {
    *ptr = NULL;
}

bool list_ins_first(struct node **ptr, char *line) {
    struct node *new_node = malloc(sizeof(struct node));
    if (new_node == NULL) return false;
    new_node->line = line;
    new_node->next = *ptr;
    *ptr = new_node;
    return true;
}

void list_print(struct node **ptr) {
    struct node *current = *ptr;
    while (current != NULL) {
        printf("%s\n", current->line);
        current = current->next;
    }
}

int list_size(struct node **ptr) {
    struct node *current = *ptr;
    int count = 0;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

void list_free(struct node **ptr) {
    struct node *current = *ptr;
    while (current != NULL) {
        struct node *helper = current;
        current = current->next;
        free(helper->line);
        free(helper);
    }
    *ptr = NULL;
}

int main(int argc, char *argv[]) {
    struct node *l;
    list_init(&l);

    FILE *in = stdin;
    unsigned long limit = 0;
    bool has_limit = false;

    // Zpracování argumentů (např. -l 10 nebo nazev_souboru)
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-l") == 0) {
            if (i + 1 < argc) {
                limit = strtoul(argv[i + 1], NULL, 10);
                has_limit = true;
                i++; 
            } else {
                fprintf(stderr, "Chyba: Ocekavano cislo po prepinaci -l\n");
                return 1;
            }
        } else {
            in = fopen(argv[i], "r");
            if (in == NULL) {
                fprintf(stderr, "Chyba: Nelze otevrit soubor %s\n", argv[i]);
                return 1;
            }
        }
    }

    char buffer[1024];
    bool length_error_printed = false;
    unsigned long lines_read = 0;

    // Samotné čtení řádků
    while (fgets(buffer, sizeof(buffer), in) != NULL) {
        if (has_limit && lines_read >= limit) {
            break; 
        }

        size_t len = strlen(buffer);
        
        // Překročení limitu délky řádku
        if (len == sizeof(buffer) - 1 && buffer[len - 1] != '\n' && !feof(in)) {
            if (!length_error_printed) {
                fprintf(stderr, "Chyba: Prekrocen limit delky radku.\n");
                length_error_printed = true;
            }
            int c;
            while ((c = fgetc(in)) != '\n' && c != EOF);
        }

        buffer[strcspn(buffer, "\n")] = '\0';

        char *line_copy = strdup(buffer);
        if (line_copy == NULL || !list_ins_first(&l, line_copy)) {
            fprintf(stderr, "Chyba: Selhala alokace pameti\n");
            if (line_copy != NULL) free(line_copy);
            list_free(&l);
            if (in != stdin) fclose(in);
            return 1;
        }
        lines_read++;
    }

    // Výpis a úklid
    list_print(&l);
    list_free(&l);

    if (in != stdin) {
        fclose(in);
    }

    return 0;
}
