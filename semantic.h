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
    LET_IN_IF,
    LOAD_IDENTIF,
    IDENTIF_EXP,
    FUNC_CALL_PSA,
    PUSH_SCOPE,
    POP_SCOPE,
    SEM_NONE
} Control_state;

// TODO: pridat komentare

void print_items(sym_items *items);
int push_token_get_next(Token *token, Token_stack *token_stack);
bool get_func_definition(Token *token, char *name, symtable_item *psa_item);
Expression_type get_expression_type(Token *token);
bool check_ret_values(Expression_type t_exp, Expression_type t_id);

void state_func_call_psa(Token *token, sym_items *items);
void state_identif_exp(Token *token, sym_items *items);
void state_load_identif(Token *token, sym_items *items);
void state_func_body_done(Token *token, sym_items *items);
void state_let_in_if(Token *token, sym_items *items);
void state_cond_exp(Token *token, sym_items *items);
void state_r_exp(Token *token, sym_items *items);
void state_pop_scope(Token *token, sym_items *items);
void state_push_scope(Token *token, sym_items *items);
void state_func_header_done(Token *token, sym_items *items);
void state_r_type(Token *token, sym_items *items);
void state_p_type(Token *token, sym_items *items);
void state_p_id(Token *token, sym_items *items);
void state_p_name(Token *token, sym_items *items);
void state_func_id(Token *token, sym_items *items);
void state_var_add(Token *token, sym_items *items);
void state_var_exp(Token *token, sym_items *items);
void state_var_type(Token *token, sym_items *items);
void state_var_id(Token *token, sym_items *items);
void state_var(Token *token, sym_items *items);
void state_let(Token *token, sym_items *items);

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