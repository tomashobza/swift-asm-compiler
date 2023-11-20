#include <stdlib.h>
#include <stdio.h>
#include "general_stack.h"

// initalizes an empty stack. the stack will have elements that point to any type of data
Stack *general_stack_init()
{
    Stack *s = malloc(sizeof(Stack));
    if (s == NULL)
    {
        fprintf(stderr, "Malloc failed.\n");
        return NULL; // TODO: error code
    }
    s->top = NULL;
    s->size = 0;
    // s->ptr_size = data_size;

    return s;
}

void general_stack_push(Stack *s, void *data)
{
    StackNode *new_node = malloc(sizeof(StackNode));

    // new_node->data = malloc(s->ptr_size);
    // memcpy(new_node->data, data, s->ptr_size);

    new_node->data = data;
    new_node->next = s->top;

    s->top = new_node;
    s->size++;
}

void *general_stack_pop(Stack *s)
{
    if (s->top == NULL)
    {
        fprintf(stderr, "Stack underflow.\n");
        return NULL; // TODO: error code
    }

    StackNode *temp = s->top;
    void *popped = temp->data;
    s->top = temp->next;
    free(temp);
    s->size--;

    return popped;
}

void *general_stack_top(Stack *s)
{
    if (s->top == NULL)
    {
        fprintf(stderr, "Stack underflow.\n");
        return NULL; // TODO: error code
    }

    return s->top->data;
}