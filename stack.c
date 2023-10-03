/**
 * @file stack.c
 * @brief data structure - stack
 * @author Jakub Všetečka
 *
 * Project: IFJ compiler
 */

#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

/**
    * @brief Creates a new stack.
    * @return Pointer to the stack.
    */ 
Stack* stack_init() {
    Stack *s = malloc(sizeof(Stack));
    if (s == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }
    s->top = NULL;  // Initially, the stack is empty.
    s->size = 0;
    return s;
}

/**
    * @brief Checks if the stack is empty.
    * @param stack Pointer to the stack.
    * @return true if empty, false otherwise.
    */
bool stack_is_empty(const Stack *stack) {
    return stack->top == NULL;
}

/**
    * @brief Pushes a new symbol table onto the stack.
    * @param s Pointer to the stack.
    * @param table Pointer to the symbol table to be pushed.
    */
void stack_push(Stack *s, symtable_t *table) {
    table->next = s->top;
    s->top = table;
    s->size++;
}

/**
    * @brief Pops the top symbol table from the stack.
    * @param s Pointer to the stack.
    * @return Pointer to the popped symbol table.
    */
symtable_t* stack_pop(Stack *s) {
    if (stack_is_empty(s)) {
        fprintf(stderr, "Stack underflow.\n");
        exit(1);
    }
    symtable_t *popped = s->top;
    s->top = popped->next;
    s->size--;
    return popped;
}

/**
    * @brief Frees the stack.
    * @param s Pointer to the stack.
    */
void stack_free(Stack *s) {
    while (!stack_is_empty(s)) {
        symtable_t *temp = stack_pop(s);
        symtable_free(temp);
    }
    free(s);
}
