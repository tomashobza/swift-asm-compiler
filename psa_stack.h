#ifndef PSA_STACK_H_
#define PSA_STACK_H_

#include "general_stack.h"
#include "scanner.h"

void psa_stack_push(Stack *s, Token data);

Token psa_stack_pop(Stack *s);

Token psa_stack_top(Stack *s);

#endif /* PSA_STACK_H_ */