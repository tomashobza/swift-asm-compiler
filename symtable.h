#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define SYMTABLE_MAX_ITEMS 256

typedef struct symtable_item
{
    char *name;
    char *type;   //
    int size;     // size in bytes
    bool is_init; //
    struct symtable_item *next;
} symtable_item_t;

typedef struct symtable
{
    symtable_item_t **items;
    struct symtable *next;
} symtable_t;

// Function Prototypes
uint32_t hash(char *input);
symtable_t *init_symtable();
symtable_item_t *symtable_add(symtable_item_t item, symtable_t *table);
symtable_item_t *init_symtable_item(symtable_item_t item);
void symtable_print(symtable_t *table);
void free_synonyms(symtable_item_t *item);
void symtable_free(symtable_t *table);

#endif // SYMTABLE_H
