/**
 * @file symtable.h
 * @author Tomáš Hobza (xhobza03@vutbr.cz)
 * @brief
 */

#ifndef SYMTABLE_H
#define SYMTABLE_H

#define MAX_ITEMS 313

#include <stdint.h>
#include <stdbool.h>

typedef struct symtable_item
{
    char *name;
    char *type;   //
    int size;     // size in bytes
    bool is_init; //

    struct symtable_item *next;
} symtable_item_t;

typedef symtable_item_t *symtable_t[MAX_ITEMS];

/**
 * @brief Calculates the hashed value of a string. Source: https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
 *
 * @param input value to be hashed
 * @return int
 */
uint32_t hash(char *input);

#endif // SYMTABLE_H
