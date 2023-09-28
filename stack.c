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
    * @brief Creates a new stack with initial capacity.
    * @param initial_capacity Initial capacity of the stack.
    * @return Pointer to the stack.
    */ 
Stack* createStack(int initial_capacity) {
    Stack *s = malloc(sizeof(Stack));
    s->arr = malloc(initial_capacity * sizeof(int));
    s->size = 0;
    s->capacity = initial_capacity;
    s->top = NULL;
    return s;
}

void push(Stack *s, symtable_t table) {
    if (s->size == s->capacity) {
        s->capacity *= 2;
        s->arr = realloc(s->arr, s->capacity * sizeof(symtable_t));
    }
    s->arr[s->size] = table;
    s->top = table; // Update the top element
    s->size++;
}

symtable_t pop(Stack *s) {
    if (s->size == 0) {
        fprintf(stderr, "Stack underflow.\n");
        exit(1);
    }

    s->size--;
    symtable_t popped = s->arr[s->size];

    // Capacity downsizing
    if (s->size <= s->capacity / 4) {
        s->capacity /= 2;
        s->arr = realloc(s->arr, s->capacity * sizeof(symtable_t));
        if (s->arr == NULL) {
            fprintf(stderr, "Memory reallocation failed.\n");
            exit(1);
        }
    }

    return popped;
}

/**
    * @brief Frees the stack.
    * @param s Pointer to the stack.
    */ 
void freeStack(Stack *s) {
    free(s->arr);
    free(s);
}