#include <stdio.h>
#include <stdlib.h>
#include "stack.h"  // Assuming the Stack and Node definitions are in stack.h

Stack* stack_init() {
    Stack *s = malloc(sizeof(Stack));
    if (s == NULL) {
        fprintf(stderr, "Malloc failed.\n");
        return NULL; // TODO: error code
    }
    s->top = NULL;
    s->size = 0;
    return s;
}

bool stack_is_empty(const Stack *stack) {
    return stack->top == NULL;
}

void stack_push(Stack *s, symtable_t *table) {
    Node *new_node = malloc(sizeof(Node));
    new_node->data = table;
    new_node->next = s->top;
    s->top = new_node;
    s->size++;
}

symtable_t* stack_pop(Stack *s) {
    if (stack_is_empty(s)) {
        fprintf(stderr, "Stack underflow.\n");
        return NULL; //TODO: error code
    }
    
    Node *temp = s->top;
    symtable_t *popped = temp->data;
    s->top = temp->next;
    free(temp);
    s->size--;

    return popped;
}

void stack_free(Stack *s) {
    while (!stack_is_empty(s)) {
        stack_pop(s);
    }
    free(s);
}