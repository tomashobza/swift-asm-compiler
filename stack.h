#ifndef STACK_H
#define STACK_H

#include "symtable.h"

// Stack structure definition
typedef struct {
    symtable_t *arr;  // Change from symtable_t to symtable_t*
    symtable_t top; // Pointer to the top element
    int size;
    int capacity;
} Stack;
// Function prototypes

// Creates a new stack with initial capacity and returns its pointer
Stack* createStack(int initial_capacity);

// Pushes a new element onto the stack, resizing if necessary
void push(Stack *s, symtable_t table);

// Pops an element off the stack and returns it
symtable_t pop(Stack *s);

// Frees the memory occupied by the stack
void freeStack(Stack *s);

#endif // STACK_H
