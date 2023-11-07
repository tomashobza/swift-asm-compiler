#ifndef GENERAL_STACK_H
#define GENERAL_STACK_H

typedef struct StackNode
{
    void *data;
    struct StackNode *next;
} StackNode;

typedef struct Stack
{
    StackNode *top;
    int size;
} Stack;

Stack *general_stack_init();

void general_stack_push(Stack *s, void *data);

void *general_stack_pop(Stack *s);

void *general_stack_top(Stack *s);

#endif