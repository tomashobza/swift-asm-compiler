/**
 * @file symtable.h
 * @author Tomáš Hobza (xhobza03@vutbr.cz)
 * @brief
 */

#ifndef SYMTABLE_H
#define SYMTABLE_H

#define SYMTABLE_MAX_ITEMS 13
// #define SYMTABLE_MAX_ITEMS 313

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct symtable_item
{
    char *name;
    char *type;   //
    int size;     // size in bytes
    bool is_init; //

    struct symtable_item *next;
} symtable_item_t;

typedef symtable_item_t **symtable_t;

/**
 * @brief Calculates the hashed value of a string. Source: https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
 *
 * @param input value to be hashed
 * @return int
 */
uint32_t hash(char *input);

/**
 * @brief Allocates a new symtable and initializes the items to NULL, then returns the address of the new symtable.
 *
 * @return symtable_t* - pointer to the new symtable
 */
symtable_t init_symtable();

/**
 * @brief Adds a new symbol to the hash table of symbols.
 *
 * @param item item to be added
 * @param table table to put the symbol into
 * @return symtable_item_t*
 */
symtable_item_t *symtable_add(symtable_item_t item, symtable_t table);

/**
 * @brief Creates a new symbol object.
 *
 * @param item return a pointer to the new symbol
 * @return symtable_item_t*
 */
symtable_item_t *init_symtable_item(symtable_item_t item);

/**
 * @brief Prints the content of the symtable.
 *
 * @param table table to print
 */
void symtable_print(symtable_t table);

#endif // SYMTABLE_H
