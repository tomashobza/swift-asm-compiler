#ifndef STACK_H
#define STACK_H

#include <stdbool.h>  // for bool
#include "symtable.h"  // Assuming symtable_t is defined in this header

/// @struct Node
/// @brief Node structure for stack implementation
typedef struct Node {
    symtable_t *data;   ///< Pointer to the data
    struct Node* next;  ///< Pointer to the next node
} Node;

/// @struct Stack
/// @brief Stack structure using Node
typedef struct {
    Node* top;  ///< Pointer to the top node
    int size;   ///< Size of the stack
} Stack;

/// @brief Initialize a new stack and returns its pointer
/// @return Pointer to the new stack
Stack* stack_init();

/// @brief Check if the stack is empty
/// @param stack Pointer to the stack
/// @return True if stack is empty, false otherwise
bool stack_is_empty(const Stack *stack);

/// @brief Push a new element onto the stack
/// @param s Pointer to the stack
/// @param table Pointer to the symtable element to push
void stack_push(Stack *s, symtable_t *table);

/// @brief Pop an element off the stack
/// @param s Pointer to the stack
/// @return Pointer to the popped symtable element
symtable_t* stack_pop(Stack *s);

/// @brief Free the memory occupied by the stack
/// @param s Pointer to the stack
void stack_free(Stack *s);

#endif // STACK_H