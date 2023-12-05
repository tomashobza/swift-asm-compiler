/**
 * @file symtable.c
 * @author Anastasia Butok (xbutok00@vutbr.cz)
 * @brief Implementation of the table of symbols functions.
 * @version 0.1
 * @date 2023-11-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef SYMTABLE_H
#define SYMTABLE_H

#define SYMTABLE_MAX_ITEMS 1024

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "stack.h"
#include "debug.h"
#include "error.h"

/**
 * @brief Enum for the types of the expression.
 */
typedef enum
{
    TYPE_EMPTY = -2,     // empty expression
    TYPE_INVALID = -1,   // invalid expression
    TYPE_INT = 0,        // int
    TYPE_DOUBLE = 1,     // double
    TYPE_STRING = 2,     // string
    TYPE_BOOL = 3,       // bool
    TYPE_NIL = 4,        // nil
    TYPE_INT_NIL = 5,    // int?
    TYPE_DOUBLE_NIL = 6, // double?
    TYPE_STRING_NIL = 7, // string?
    TYPE_BOOL_NIL = 8,   // bool?
} Expression_type;

typedef struct
{
    unsigned long gen_id_idx;
    Expression_type type;
    bool is_const;
    bool is_initialized;
    bool is_param;
} VariableData;

typedef struct
{
    char *name;
    char *id;
    Expression_type type;
} ParamData;

typedef struct
{
    Expression_type return_type;
    bool found_return;
    ParamData *params;
    int params_count;
    int capacity;
} FunctionData;

typedef struct symtable_item
{
    char *id;
    int scope;
    enum
    {
        VARIABLE = 0,
        FUNCTION = 1
    } type; // typ symbolu
    union
    {
        VariableData *var_data;
        FunctionData *func_data;
    } data;
    struct symtable_item *next;
} symtable_item;

typedef symtable_item **symtable;

typedef struct
{
    symtable_item *varItem;
    symtable_item *funcItem;
} sym_items;

DECLARE_STACK_FUNCTIONS(symtable);

extern symtable_stack *sym_st;

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
symtable_item *symtable_add(symtable_item *item, symtable table);

/**
 * @brief Finds a symbol in the table and returns it's pointer.
 *
 * @param name name of the symbol to find
 * @param table table to search in
 * @param is_func if the symbol is a function
 * @return symtable_item* - pointer to the symbol or NULL if not found
 */
symtable_item *symtable_find(char *name, symtable table, bool is_func);

/**
 * @brief Finds a symbol in the table and returns it's pointer.
 *
 * @param name name of the symbol to find
 * @param stack stack of symtables to search in
 * @param is_func if the symbol is a function
 * @return symtable_item* - pointer to the symbol or NULL if not found
 */
symtable_item *symtable_find_in_stack(char *name, symtable_stack *stack, bool is_func);

FunctionData *init_func_data();
VariableData *init_var_data();
ParamData *init_param_data(int count);

/**
 * @brief Adds a new parameter to the function.
 *
 * @param func function data to add the parameter to
 */
void add_param(FunctionData *func);

/**
 * @brief Creates a new symbol object.
 *
 * @param item return a pointer to the new symbol
 * @return symtable_item*
 */
symtable_item *init_symtable_item(bool is_func);

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