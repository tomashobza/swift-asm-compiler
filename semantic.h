#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "error.h"
#include "stack.h"

#define RED   "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define BLUE  "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN  "\x1B[36m"
#define RESET "\x1B[0m"

typedef enum {
    LET,
    VAR,
    VAR_ID,
    VAR_TYPE,
    FUNC_ID,
    FUNC_HEADER_DONE,
    P_NAME,
    P_ID,
    P_TYPE,
    R_TYPE,
    SEM_NONE
} Sem_rule;

int semantic_init();
void add_param(FunctionData *func, ParamData new_param);
void reset_param();
void semantic_destroy();
int check_semantic(Token **token, Sem_rule sem_rule);

#endif // SEMANTIC_H