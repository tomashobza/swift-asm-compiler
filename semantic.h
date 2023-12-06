/**
 * @file semantic.h
 * @author Jakub Všetečka <xvsete00@vutbr.cz>
 * @brief Header file for semantic analysis.
 * @version 0.1
 * @date 2023-11-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "psa.h"
#include "utils.h"
#include "error.h"
#include "stack.h"
#include "symtable.h"

extern symtable_stack *sym_st;

#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define BLUE "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN "\x1B[36m"
#define RESET "\x1B[0m"

typedef enum
{
    STARTING,
    LET,
    VAR,
    VAR_ID,
    VAR_TYPE,
    VAR_ASSIGN1,
    VAR_ASSIGN2,
    VAR_EXP,
    VAR_ADD,
    FUNC_ID,
    FUNC_HEADER_DONE,
    P_NAME,
    P_ID,
    P_TYPE,
    R_TYPE,
    R_EXP,
    FUNC_BODY_DONE,
    COND_EXP,
    IF_START,
    ELSE_IF_START,
    ELSE_IF_AFTER_COND,
    ELSE_START,
    IF_END,
    FUNC_IF_FOUND,
    FUNC_ELSE,
    WHILE_START,
    WHILE_COND,
    WHILE_END,
    LET_IN_IF,
    LOAD_IDENTIF,
    IDENTIF_EXP,
    FUNC_CALL_PSA,
    PUSH_SCOPE,
    POP_SCOPE,
    SEM_NONE
} Control_state;

/**
 * @brief Prints out sym_items attributes.
 *
 * @param items
 */
void print_items(sym_items *items);

/**
 * @brief Stores current tokne to the token_stack. Loads next token.
 *
 * @param token Pointer to the current token. New token will be stored here.
 * @param token_stack Pointer to the token stack.
 */
void push_token_get_next(Token *token, Token_stack *token_stack);

/**
 * @brief Finds the function definition in the program.
 *
 * @param token Pointer to the current token.
 * @param name Name of the function.
 * @param psa_item Pointer to the symtable_item where the function definition will be stored.
 *
 * @return true if the function was found.
 */
bool get_func_definition(Token *token, char *name, symtable_item *psa_item);

/**
 * @brief Converts token type to expression type.
 *
 * @param token Pointer to the token.
 * @return Expression_type
 */
Expression_type get_expression_type(Token *token);

/**
 * @brief Checks if the expression type is valid for the given ideticator type.
 *
 * @param t_exp Expression type.
 * @param t_id Identifier type.
 * @return true if the expression type is valid for the given ideticator type.
 */
bool check_ret_values(Expression_type t_exp, Expression_type t_id);

// SEMANTIC RULES

/**
 * @brief Prepares semantic analysis for an 'if' block in a function.
 * Resets the 'found_else' flag in the current scope.
 *
 * @param token Pointer to the current token.
 * @param items Pointer to the sym_items structure.
 */
void sem_func_if_start(Token *token, sym_items *items);

/**
 * @brief Marks the presence of an 'else' block in the current function scope.
 *
 * @param token Pointer to the current token.
 * @param items Pointer to the sym_items structure.
 */
void sem_func_else(Token *token, sym_items *items);

/**
 * @brief Initializes semantic analysis at the start of parsing.
 * Resets the function item in sym_items to null.
 *
 * @param token Pointer to the current token.
 * @param items Pointer to the sym_items structure.
 */
void sem_start(Token *token, sym_items *items);

/**
 * @brief Manages the semantic analysis of a function call.
 * Parses the call expression and validates its correctness.
 *
 * @param token Pointer to the current token.
 * @param items Pointer to the sym_items structure.
 */
void sem_func_call_psa(Token *token, sym_items *items);

/**
 * @brief Handles expressions involving identifiers.
 * Validates type compatibility and marks the variable as initialized.
 *
 * @param token Pointer to the current token.
 * @param items Pointer to the sym_items structure.
 */
void sem_identif_exp(Token *token, sym_items *items);

/**
 * @brief Loads a variable identifier for semantic analysis.
 * Validates the existence and mutability of the variable.
 *
 * @param token Pointer to the current token.
 * @param items Pointer to the sym_items structure.
 */
void sem_load_identif(Token *token, sym_items *items);

/**
 * @brief Finalizes the semantic analysis of a function body.
 * Validates the presence of a return statement and resets scope attributes.
 *
 * @param token Pointer to the current token.
 * @param items Pointer to the sym_items structure.
 */
void sem_func_body_done(Token *token, sym_items *items);

/**
 * @brief Handles let declarations within 'if' blocks.
 * Validates the let variable and sets up a new scope for the block.
 *
 * @param token Pointer to the current token.
 * @param items Pointer to the sym_items structure.
 */
void sem_let_in_if(Token *token, sym_items *items);

/**
 * @brief Validates the conditional expression in if/while statements.
 * Ensures the expression type is boolean and compatible with the condition context.
 *
 * @param token Pointer to the current token.
 * @param items Pointer to the sym_items structure.
 */
void sem_cond_exp(Token *token, sym_items *items);

/**
 * @brief Handles return expressions in functions.
 * Validates the expression type against the function's return type and marks the return as found.
 *
 * @param token Pointer to the current token.
 * @param items Pointer to the sym_items structure.
 */
void sem_r_exp(Token *token, sym_items *items);

/**
 * @brief Pops the current scope from the symbol table stack.
 * Handles return logic for functions, updating attributes in the parent scope.
 *
 * @param token Pointer to the current token.
 * @param items Pointer to the sym_items structure.
 */
void sem_pop_scope(Token *token, sym_items *items);

/**
 * @brief Creates and pushes a new scope onto the symbol table stack.
 *
 * @param token Pointer to the current token.
 * @param items Pointer to the sym_items structure.
 */
void sem_push_scope(Token *token, sym_items *items);

/**
 * @brief Finalizes the function header and adds the function to the symbol table.
 * Initializes a new scope and adds parameters as local variables to this scope.
 *
 * @param token Pointer to the current token.
 * @param items Pointer to the sym_items structure.
 */
void sem_func_header_done(Token *token, sym_items *items);

/**
 * @brief Sets the return type for a function based on the given token.
 *
 * @param token Pointer to the current token.
 * @param items Pointer to the sym_items structure.
 */
void sem_r_type(Token *token, sym_items *items);

/**
 * @brief Determines and sets the type for a function parameter.
 * Ensures the parameter's name and ID do not conflict and checks for duplicate parameters.
 *
 * @param token Pointer to the current token.
 * @param items Pointer to the sym_items structure.
 */
void sem_p_type(Token *token, sym_items *items);

/**
 * @brief Sets the identifier for a function parameter.
 * Validates and initializes a corresponding variable item in sym_items.
 *
 * @param token Pointer to the current token.
 * @param items Pointer to the sym_items structure.
 */
void sem_p_id(Token *token, sym_items *items);

/**
 * @brief Assigns a name to a function parameter.
 * Adds the parameter to the function's data and sets its name.
 *
 * @param token Pointer to the current token.
 * @param items Pointer to the sym_items structure.
 */
void sem_p_name(Token *token, sym_items *items);

/**
 * @brief Initializes a function item and sets its identifier.
 * Checks for function name conflicts before setting the ID in sym_items.
 *
 * @param token Pointer to the current token.
 * @param items Pointer to the sym_items structure.
 */
void sem_func_id(Token *token, sym_items *items);

/**
 * @brief Adds the processed variable to the symbol table.
 * Handles both new variables and parameters being converted to local variables.
 *
 * @param token Pointer to the current token.
 * @param items Pointer to the sym_items structure.
 */
void sem_var_add(Token *token, sym_items *items);

/**
 * @brief Handles the variable initialization expression.
 * Evaluates the expression type, ensuring compatibility with the variable's declared type.
 *
 * @param token Pointer to the current token.
 * @param items Pointer to the sym_items structure.
 */
void sem_var_exp(Token *token, sym_items *items);

/**
 * @brief Sets the data type for a variable based on the token's expression type.
 *
 * @param token Pointer to the current token.
 * @param items Pointer to the sym_items structure.
 */
void sem_var_type(Token *token, sym_items *items);

/**
 * @brief Validates and sets the identifier for a variable.
 * Checks for naming conflicts and sets the variable ID in sym_items.
 *
 * @param token Pointer to the current token.
 * @param items Pointer to the sym_items structure.
 */
void sem_var_id(Token *token, sym_items *items);

/**
 * @brief Manages the semantics of a non-constant variable declaration.
 * Initializes a new variable item as non-constant in sym_items.
 *
 * @param token Pointer to the current token.
 * @param items Pointer to the sym_items structure.
 */
void sem_var(Token *token, sym_items *items);

/**
 * @brief Handles the semantics of a constant variable declaration ('let').
 * Initializes a new variable item as constant in sym_items.
 *
 * @param token Pointer to the current token.
 * @param items Pointer to the sym_items structure.
 */
void sem_let(Token *token, sym_items *items);

/**
 * @brief Checks if the expression is convertable to the variable type. This behavior is defined in the documentation only for implicit type conversion of literals Int -> Double.
 *
 * @param variable_type type of the variable being assigned to
 * @param expression_type type of the expression being assigned
 * @param is_expression_literal true if the expression is a literal
 * @return true
 * @return false
 */
bool isTypeConvertable(Expression_type variable_type, Expression_type expression_type, bool is_expression_literal);

#endif // SEMANTIC_H