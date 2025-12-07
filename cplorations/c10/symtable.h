/****************************************
 * C-ploration 9 for CS 271
 *
 * [NAME] Brent Grossman
 * [TERM] FALL 2025
 *
 ****************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef __SYMTABLE_H__
#define __SYMTABLE_H__

#define SYMBOL_TABLE_SIZE 1000

typedef int16_t hack_addr;

typedef struct Symbol {
    char *name;
    hack_addr addr;
} Symbol;

int hash(const char *str);
struct Symbol *symtable_find(const char *key);
void symtable_insert(const char *key, hack_addr addr);
void symtable_display_table();
void symtable_print_labels();

#endif
