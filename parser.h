/**
 * @file parser.h
 * @author Jakub Všetečka <xvsete00@vutbr.cz>
 * @brief Parser header file.
 * @version 0.1
 * @date 2023-11-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "debug.h"
#include "scanner.h"
#include "error.h"
#include "stack.h"
#include "semantic.h"
#include "generator.h"
#include "symtable.h"

#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define BLUE "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN "\x1B[36m"
#define RESET "\x1B[0m"

void run_parser();
void get_token(Token *token);
bool cmp_type(Token *token, sym_items *items, Token_type type, Control_state sem_rule);
void add_builtin_functions(sym_items *items);
int run_control(Token *token, sym_items *items, Control_state sem_rule);

bool START(Token *token, sym_items *items);
bool STMT_LIST(Token *token, sym_items *items);
bool STMT(Token *token, sym_items *items);
bool VAR_LET(Token *token, sym_items *items);
bool VAR_SCOPE(Token *token, sym_items *items);
bool TYPE_AND_ASIGN(Token *token, sym_items *items);
bool D_TYPE(Token *token, sym_items *items, Control_state sem_rule);
bool R_FLEX(Token *token, sym_items *items);
bool DEF_FUNC(Token *token, sym_items *items);
bool P_LIST(Token *token, sym_items *items);
bool PARAM(Token *token, sym_items *items);
bool P_SEP(Token *token, sym_items *items);
bool RET_TYPE(Token *token, sym_items *items);
bool FUNC_STMT_LIST(Token *token, sym_items *items);
bool FUNC_STMT(Token *token, sym_items *items);
bool RET(Token *token, sym_items *items);
bool FUNC_WHILE(Token *token, sym_items *items);
bool FUNC_IF(Token *token, sym_items *items);
bool FUNC_ELSE_CLAUSE(Token *token, sym_items *items);
bool FUNC_AFTER_ELSE(Token *token, sym_items *items);
bool IF_STMT(Token *token, sym_items *items);
bool IF_COND(Token *token, sym_items *items);
bool ELSE_CLAUSE(Token *token, sym_items *items);
bool AFTER_ELSE(Token *token, sym_items *items);
bool WHILE_STMT(Token *token, sym_items *items);
bool LOAD_ID(Token *token, sym_items *items);
bool EXP(Token *token, sym_items *items, Control_state sem_rule);

int parser_main();

#endif // PARSER_H