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
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "stack.h"

typedef struct symtable_item
{
    char *name;
    char *type;      //
    int size;        // size in bytes
    char *params;    // string of params
    int param_count; // number of params
    bool is_init;    //

    struct symtable_item *next;
} symtable_item;

typedef symtable_item **symtable;

DECLARE_STACK_FUNCTIONS(symtable);

/**
 * @brief Struct to hold the stack of symtables.
 *
 * @param stack stack of symtables
 */
void symtable_stack_free_all(symtable_stack *stack);

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
 * @return symtable* - pointer to the new symtable
 */
symtable symtable_init();

/**
 * @brief Adds a new symbol to the hash table of symbols. (Allows duplicates)
 *
 * @param item item to be added
 * @param table table to put the symbol into
 * @return symtable_item* - pointer to the new symbol
 */
symtable_item *symtable_add(symtable_item item, symtable table);

/**
 * @brief Finds a symbol in the table and returns it's pointer.
 *
 * @param name name of the symbol to find
 * @param table table to search in
 * @return symtable_item* - pointer to the symbol or NULL if not found
 */
symtable_item *symtable_find(char *name, symtable table);

/**
 * @brief Finds a symbol in the table and returns it's pointer.
 *
 * @param name name of the symbol to find
 * @param stack stack of symtables to search in
 * @return symtable_item* - pointer to the symbol or NULL if not found
 */
symtable_item *symtable_find_in_stack(char *name, symtable_stack *stack);

/**
 * @brief Creates a new symbol object.
 *
 * @param item return a pointer to the new symbol
 * @return symtable_item*
 */
symtable_item *init_symtable_item(symtable_item item);

/**
 * @brief Prints the content of the symtable.
 *
 * @param table table to print
 */
void symtable_print(symtable table);

/**
 * @brief Recursively free all the chained synonyms
 *
 * @param item synonym to start the freeing with
 */
void free_synonyms(symtable_item *item);

/**
 * @brief Free the memory used by the table and all it's items
 *
 * @param table table to free
 */
void symtable_free(symtable table);

#endif // SYMTABLE_H