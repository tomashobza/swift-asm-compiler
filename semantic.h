#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "error.h"
#include "stack.h"

typedef enum {
    LET,
    VAR,
    VAR_TYPE,
    FUNC,
    P_NAME,
    P_ID,
    P_TYPE,
    RETURN,
    SEM_NONE
} Sem_type;

int semantic_init();
void semantic_destroy();
int check_semantic(Token **token, Sem_type sem_type);

#endif // SEMANTIC_H