#include <stdlib.h>
#include "psa_stack.h"

void psa_stack_push(Stack *s, Token data)
{
    Token *new_node = malloc(sizeof(Token));
    *new_node = data;

    general_stack_push(s, new_node);
}

Token psa_stack_pop(Stack *s)
{
    Token *popped = general_stack_pop(s);
    Token ret = *popped;
    free(popped);
    return ret;
}

Token psa_stack_top(Stack *s)
{
    Token *top = general_stack_top(s);
    return *top;
}