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

/**
 * @brief Initializes needed structures and runs the parser.
 */
void run_parser();

/**
 * @brief Asks scanner for next token.
 *
 * @param token pointer to where the new token will be saved
 */
void get_token(Token *token);

/**
 * @brief Compares token type with expected type. Calls semantic rule if needed.
 *
 * @param token pointer to token
 * @param items pointer to sym_items for semantic analysis
 * @param type expected token type
 * @param sem_rule semantic rule
 */
bool cmp_type(Token *token, sym_items *items, Token_type type, Control_state sem_rule);

/**
 * @brief Adds built-in functions to symtable.
 *
 * @param items pointer to sym_items
 */
void add_builtin_functions(sym_items *items);

/**
 * @brief Executes semantic rule alongside with generator if needed.
 *
 * @param token pointer to token
 * @param items pointer to sym_items for context propagation
 * @param sem_rule semantic rule to apply
 */
int run_control(Token *token, sym_items *items, Control_state sem_rule);

// RECURSIVE DESCENT PARSER

/**
 * Parses the START nonterminal in the defined grammar.
 * START signifies the top-level rule of the grammar, structured as:
 * START -> STMT_LIST eof
 * This function manages the initial parsing step, ensuring that the entire input
 * conforms to the STMT_LIST followed by an end-of-file (eof) token.
 *
 * @param token A pointer to the current token, marking the beginning of the parsing process.
 * @param items A pointer to sym_items, used for contextual information and state management during parsing.
 * @return Returns true if the entire input sequence correctly matches the START rule, false otherwise.
 */
bool START(Token *token, sym_items *items);

/**
 * Parses STMT_LIST nonterminal in the defined grammar.
 * STMT_LIST -> LOCAL_STMT_LIST STMT_LIST | ε
 *
 * @param token A pointer to the current token in the parsing process.
 * @param items A pointer to sym_items, used for context propagation during parsing.
 * @return Returns true if parsing succeeds, false otherwise.
 */
bool STMT_LIST(Token *token, sym_items *items);

/**
 * Parses LOCAL_STMT_LIST nonterminal in the defined grammar.
 * LOCAL_STMT_LIST -> LOCAL_STMT LOCAL_STMT_LIST | ε
 *
 * @param token A pointer to the current token in the parsing process.
 * @param items A pointer to sym_items, used for context propagation during parsing.
 * @return Returns true if parsing succeeds, false otherwise.
 */
bool LOCAL_STMT_LIST(Token *token, sym_items *items);

/**
 * Parses STMT nonterminal in the defined grammar.
 * STMT -> VAR_LET | LOAD_ID | DEF_FUNC | FUNC_STMT | IF_STMT | WHILE_STMT | ε
 *
 * @param token A pointer to the current token in the parsing process.
 * @param items A pointer to sym_items, used for context propagation during parsing.
 * @return Returns true if parsing succeeds, false otherwise.
 */
bool STMT(Token *token, sym_items *items);

/**
 * Parses LOCAL_STMT nonterminal in the defined grammar.
 * LOCAL_STMT -> VAR_LET | LOAD_ID | FUNC_STMT | IF_STMT | WHILE_STMT | ε
 *
 * @param token A pointer to the current token in the parsing process.
 * @param items A pointer to sym_items, used for context propagation during parsing.
 * @return Returns true if parsing succeeds, false otherwise.
 */
bool LOCAL_STMT(Token *token, sym_items *items);

/**
 * Parses VAR_LET nonterminal in the defined grammar.
 * VAR_LET -> VAR_LET -> VAR_SCOPE id TYPE_AND_ASSIGN
 *
 * @param token A pointer to the current token in the parsing process.
 * @param items A pointer to sym_items, used for context propagation during parsing.
 * @return Returns true if parsing succeeds, false otherwise.
 */
bool VAR_LET(Token *token, sym_items *items);

/**
 * Parses VAR_SCOPE nonterminal in the defined grammar.
 * VAR_SCOPE -> let | var
 *
 * @param token A pointer to the current token in the parsing process.
 * @param items A pointer to sym_items, used for context propagation during parsing.
 * @return Returns true if parsing succeeds, false otherwise.
 */
bool VAR_SCOPE(Token *token, sym_items *items);

/**
 * Parses TYPE_AND_ASSIGN nonterminal in the defined grammar.
 * TYPE_AND_ASSIGN -> : D_TYPE R_FLEX | = EXP
 *
 * @param token A pointer to the current token in the parsing process.
 * @param items A pointer to sym_items, used for context propagation during parsing.
 * @param sem_rule Semantic rule to apply.
 * @return Returns true if parsing succeeds, false otherwise.
 */
bool TYPE_AND_ASIGN(Token *token, sym_items *items);

/**
 * Parses D_TYPE nonterminal in the defined grammar.
 * D_TYPE -> Int | Int? | Double | Double? | String | String? | Bool | Bool?
 *
 * @param token A pointer to the current token in the parsing process.
 * @param items A pointer to sym_items, used for context propagation during parsing.
 * @param sem_rule Semantic rule to apply.
 * @return Returns true if parsing succeeds, false otherwise.
 */
bool D_TYPE(Token *token, sym_items *items, Control_state sem_rule);

/**
 * Parses R_FLEX nonterminal in the defined grammar.
 * R_FLEX -> = EXP | ε
 *
 * @param token A pointer to the current token in the parsing process.
 * @param items A pointer to sym_items, used for context propagation during parsing.
 * @return Returns true if parsing succeeds, false otherwise.
 */
bool R_FLEX(Token *token, sym_items *items);

/**
 * Parses DEF_FUNC nonterminal in the defined grammar.
 * DEF_FUNC -> func id ( P_LIST ) RET_TYPE { FUNC_STMT_LIST }
 *
 * @param token A pointer to the current token in the parsing process.
 * @param items A pointer to sym_items, used for context propagation during parsing.
 * @return Returns true if parsing succeeds, false otherwise.
 */
bool DEF_FUNC(Token *token, sym_items *items);

/**
 * Parses P_LIST nonterminal in the defined grammar.
 * P_LIST -> PARAM | ε
 *
 * @param token A pointer to the current token in the parsing process.
 * @param items A pointer to sym_items, used for context propagation during parsing.
 * @return Returns true if parsing succeeds, false otherwise.
 */
bool P_LIST(Token *token, sym_items *items);

/**
 * Parses PARAM nonterminal in the defined grammar.
 * PARAM -> id id : D_TYPE SEP
 *
 * @param token A pointer to the current token in the parsing process.
 * @param items A pointer to sym_items, used for context propagation during parsing.
 * @return Returns true if parsing succeeds, false otherwise.
 */
bool PARAM(Token *token, sym_items *items);

/**
 * Parses SEP nonterminal in the defined grammar.
 * SEP -> , PARAM | ε
 *
 * @param token A pointer to the current token in the parsing process.
 * @param items A pointer to sym_items, used for context propagation during parsing.
 * @return Returns true if parsing succeeds, false otherwise.
 */
bool P_SEP(Token *token, sym_items *items);

/**
 * Parses RET_TYPE nonterminal in the defined grammar.
 * RET_TYPE -> -> D_TYPE | ε
 *
 * @param token A pointer to the current token in the parsing process.
 * @param items A pointer to sym_items, used for semantic analysis.
 * @return Returns true if parsing succeeds, false otherwise.
 */
bool RET_TYPE(Token *token, sym_items *items);

/**
 * Parses FUNC_STMT_LIST nonterminal in the defined grammar.
 * FUNC_STMT_LIST -> FUNC_STMT FUNC_STMT_LIST | ε
 *
 * @param token A pointer to the current token in the parsing process.
 * @param items A pointer to sym_items, used for semantic analysis.
 * @return Returns true if parsing succeeds, false otherwise.
 */
bool FUNC_STMT_LIST(Token *token, sym_items *items);

/**
 * Parses FUNC_STMT nonterminal in the defined grammar.
 * FUNC_STMT -> VAR_LET | LOAD_ID | FUNC_WHILE | FUNC_IF | RET | ε
 *
 * @param token A pointer to the current token in the parsing process.
 * @param items A pointer to sym_items, used for semantic analysis.
 * @return Returns true if parsing succeeds, false otherwise.
 */
bool FUNC_STMT(Token *token, sym_items *items);

/**
 * Parses RET nonterminal in the defined grammar.
 * RET -> return EXP
 *
 * @param token A pointer to the current token in the parsing process.
 * @param items A pointer to sym_items, used for semantic analysis.
 * @return Returns true if parsing succeeds, false otherwise.
 */
bool RET(Token *token, sym_items *items);

/**
 * Parses FUNC_WHILE nonterminal in the defined grammar.
 * FUNC_WHILE -> while EXP { FUNC_STMT_LIST }
 *
 * @param token A pointer to the current token in the parsing process.
 * @param items A pointer to sym_items, used for semantic analysis.
 * @return Returns true if parsing succeeds, false otherwise.
 */
bool FUNC_WHILE(Token *token, sym_items *items);

/**
 * Parses FUNC_IF nonterminal in the defined grammar.
 * FUNC_IF -> if IF_COND { FUNC_STMT_LIST } FUNC_ELSE_CLAUSE
 *
 * @param token A pointer to the current token in the parsing process.
 * @param items A pointer to sym_items, used for semantic analysis.
 * @return Returns true if parsing succeeds, false otherwise.
 */
bool FUNC_IF(Token *token, sym_items *items);

/**
 * Parses FUNC_ELSE_CLAUSE nonterminal in the defined grammar.
 * FUNC_ELSE_CLAUSE -> if IF_COND { FUNC_STMT_LIST } FUNC_ELSE_CLAUSE
 *
 * @param token A pointer to the current token in the parsing process.
 * @param items A pointer to sym_items, used for semantic analysis.
 * @return Returns true if parsing succeeds, false otherwise.
 */
bool FUNC_ELSE_IF(Token *token, sym_items *items);

/**
 * Parses FUNC_ELSE_CLAUSE nonterminal in the defined grammar.
 * FUNC_ELSE_CLAUSE -> else FUNC_AFTER_ELSE | ε
 *
 * @param token A pointer to the current token in the parsing process.
 * @param items A pointer to sym_items, used for semantic analysis.
 * @return Returns true if parsing succeeds, false otherwise.
 */
bool FUNC_ELSE_CLAUSE(Token *token, sym_items *items);

/**
 * Parses FUNC_AFTER_ELSE nonterminal in the defined grammar.
 * FUNC_AFTER_ELSE -> { FUNC_STMT_LIST } | FUNC_IF
 *
 * @param token A pointer to the current token in the parsing process.
 * @param items A pointer to sym_items, used for semantic analysis.
 * @return Returns true if parsing succeeds, false otherwise.
 */
bool FUNC_AFTER_ELSE(Token *token, sym_items *items);

/**
 * Parses IF_STMT nonterminal in the defined grammar.
 * IF_STMT -> if EXP { LOCAL_STMT_LIST } ELSE_CLAUSE
 *
 * @param token A pointer to the current token in the parsing process.
 * @param items A pointer to sym_items, used for semantic analysis.
 * @return Returns true if parsing succeeds, false otherwise.
 */
bool IF_STMT(Token *token, sym_items *items);

/**
 * Parses ELSE_IF_STMT nonterminal in the defined grammar.
 * ELSE_IF_STMT -> if EXP { LOCAL_STMT_LIST } ELSE_CLAUSE
 *
 * @param token A pointer to the current token in the parsing process.
 * @param items A pointer to sym_items, used for semantic analysis.
 * @return Returns true if parsing succeeds, false otherwise.
 */
bool ELSE_IF_STMT(Token *token, sym_items *items);

/**
 * Parses IF_COND nonterminal in the defined grammar.
 * IF_COND -> EXP | let id
 *
 * @param token A pointer to the current token in the parsing process.
 * @param items A pointer to sym_items, used for semantic analysis.
 * @return Returns true if parsing succeeds, false otherwise.
 */
bool IF_COND(Token *token, sym_items *items);

/**
 * Parses ELSE_CLAUSE nonterminal in the defined grammar.
 * ELSE_CLAUSE -> else AFTER_ELSE | ε
 *
 * @param token A pointer to the current token in the parsing process.
 * @param items A pointer to sym_items, used for semantic analysis.
 * @return Returns true if parsing succeeds, false otherwise.
 */
bool ELSE_CLAUSE(Token *token, sym_items *items);

/**
 * Parses AFTER_ELSE nonterminal in the defined grammar.
 * AFTER_ELSE -> { LOCAL_STMT_LIST } | IF_STMT
 *
 * @param token A pointer to the current token in the parsing process.
 * @param items A pointer to sym_items, used for semantic analysis.
 * @return Returns true if parsing succeeds, false otherwise.
 */
bool AFTER_ELSE(Token *token, sym_items *items);

/**
 * Parses WHILE_STMT nonterminal in the defined grammar.
 * WHILE_STMT -> while EXP { LOCAL_STMT_LIST }
 *
 * @param token A pointer to the current token in the parsing process.
 * @param items A pointer to sym_items, used for semantic analysis.
 * @return Returns true if parsing succeeds, false otherwise.
 */
bool WHILE_STMT(Token *token, sym_items *items);

/**
 * Parses LOAD_ID nonterminal in the defined grammar.
 * LOAD_ID -> id = EXP | func_id
 *
 * @param token A pointer to the current token in the parsing process.
 * @param items A pointer to sym_items, used for semantic analysis.
 * @return Returns true if parsing succeeds, false otherwise.
 */
bool LOAD_ID(Token *token, sym_items *items);

/**
 * Parses EXP nonterminal in the defined grammar.
 * PSA is used for parsing.
 *
 * @param token A pointer to the current token in the parsing process.
 * @param items A pointer to sym_items, used for semantic analysis.
 * @param sem_rule Semantic rule to apply.
 * @return Returns true if parsing succeeds, false otherwise.
 */
bool EXP(Token *token, sym_items *items, Control_state sem_rule);

#endif // PARSER_H