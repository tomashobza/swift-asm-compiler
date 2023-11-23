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
    FUNC_ID,
    FUNC_HEADER_DONE,
    P_NAME,
    P_ID,
    P_TYPE,
    R_TYPE,
    R_EXP,
    FUNC_BODY_DONE,
    COND_EXP,
    LOAD_IDENTIF,
    IDENTIF_EXP,
    FUNC_CALL_PSA,
    PUSH_SCOPE,
    POP_SCOPE,
    SEM_NONE
} Sem_rule;

int semantic_init();
void add_param(FunctionData *func, ParamData new_param);
void reset_param();
void reset_func();
void reset_var();
void add_builtin_functions();
void semantic_destroy();
int push_token_get_next(Token *token, Token_stack *token_stack);
bool get_func_definition(Token *token, char *name, symtable_item *psa_item);
int check_semantic(Token *token, Sem_rule sem_rule);

#endif // SEMANTIC_H