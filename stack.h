#ifndef STACK_H
#define STACK_H

#include <stdbool.h>  // for the boolean data type
#include "symtable.h" // include your symbol table header

// Stack structure definition
typedef struct Stack {
    symtable_t *top;  // Pointer to the top element (a symtable_t)
    int size;         // Number of elements in the stack
} Stack;

// Function prototypes

/**
    * @brief Creates a new stack.
    * @return Pointer to the stack.
    */
Stack* stack_init();

/**
    * @brief Checks if the stack is empty.
    * @param stack Pointer to the stack.
    * @return true if empty, false otherwise.
    */
bool stack_is_empty(const Stack *stack);

/**
    * @brief Pushes a new symbol table onto the stack.
    * @param s Pointer to the stack.
    * @param table Pointer to the symbol table to be pushed.
    */
void stack_push(Stack *s, symtable_t *table);

/**
    * @brief Pops the top symbol table from the stack.
    * @param s Pointer to the stack.
    * @return Pointer to the popped symbol table.
    */
symtable_t* stack_pop(Stack *s);

/**
    * @brief Frees the stack.
    * @param s Pointer to the stack.
    */
void stack_free(Stack *s);

#endif // STACK_H
