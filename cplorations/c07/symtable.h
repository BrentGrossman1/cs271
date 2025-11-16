/****************************************
 * C-ploration 7 for CS 271
 *
 * [NAME] Brent Grossman
 * [TERM] FALL 2025
 *
 ****************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef SYMTABLE_H_INCLUDED
#define SYMTABLE_H_INCLUDED

#define SYMBOL_TABLE_SIZE 1000

#endif // SYMTABLE_H_INCLUDED


typedef int16_t hack_addr;

typedef struct Symbol {
    char *name;
    hack_addr addr;
} Symbol;

int hash(char *str);
struct Symbol *symtable_find(char *key);
void symtable_insert(char *key, hack_addr addr);
void symtable_display_table();
void symtable_print_labels();

