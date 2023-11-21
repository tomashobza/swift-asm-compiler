#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "scanner.h"
#include "error.h"
#include "stack.h"
#include "semantic.h"

#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define BLUE "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN "\x1B[36m"
#define RESET "\x1B[0m"

#define DEBUG 1
#if DEBUG
#define DEBUG_CODE(code) \
    do                   \
    {                    \
        code             \
    } while (0)
#else
#define DEBUG_CODE(code) \
    do                   \
    {                    \
    } while (0)
#endif

void get_token(Token **token);
bool cmp_type(Token **token, Token_type type, Sem_rule sem_rule);

bool START(Token **token);
bool STMT_LIST(Token **token);
bool STMT(Token **token);
bool VAR_LET(Token **token);
bool VAR_SCOPE(Token **token);
bool TYPE_AND_ASIGN(Token **token);
bool D_TYPE(Token **token, Sem_rule sem_rule);
bool R_FLEX(Token **token);
bool DEF_FUNC(Token **token);
bool P_LIST(Token **token);
bool PARAM(Token **token);
bool P_SEP(Token **token);
bool RET_TYPE(Token **token);
bool FUNC_STMT_LIST(Token **token);
bool FUNC_STMT(Token **token);
bool RET(Token **token);
bool FUNC_WHILE(Token **token);
bool FUNC_IF(Token **token);
bool FUNC_ELSE_CLAUSE(Token **token);
bool FUNC_AFTER_ELSE(Token **token);
bool IF_STMT(Token **token);
bool ELSE_CLAUSE(Token **token);
bool AFTER_ELSE(Token **token);
bool WHILE_STMT(Token **token);
bool LOAD_ID(Token **token);
bool ALL_AFTER_ID(Token **token);
bool FUNC_CALL(Token **token);
bool EXP(Token **token);

int parser_main();

#endif // PARSER_H