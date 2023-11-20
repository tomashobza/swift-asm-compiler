#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>

// #include "psa.h" // Include psa.h if it provides necessary types for stack.h

#define DECLARE_STACK_FUNCTIONS(type)                       \
    typedef struct type##Node                               \
    {                                                       \
        type data;                                          \
        struct type##Node *next;                            \
    } type##Node;                                           \
                                                            \
    typedef struct                                          \
    {                                                       \
        type##Node *top;                                    \
        unsigned int size;                                  \
    } type##Stack;                                          \
                                                            \
    type##Stack *type##_stack_init();                       \
    void type##_stack_push(type##Stack *stack, type value); \
    type type##_stack_pop(type##Stack *stack);              \
    type type##_stack_top(type##Stack *stack);              \
    void type##_stack_free(type##Stack *stack);

#define DEFINE_STACK_FUNCTIONS(type)                                     \
    type##Stack *type##_stack_init()                                     \
    {                                                                    \
        type##Stack *stack = (type##Stack *)malloc(sizeof(type##Stack)); \
        if (!stack)                                                      \
        {                                                                \
            fprintf(stderr, "Memory allocation failed for stack\n");     \
            exit(EXIT_FAILURE);                                          \
        }                                                                \
        stack->top = NULL;                                               \
        return stack;                                                    \
    }                                                                    \
                                                                         \
    void type##_stack_push(type##Stack *stack, type value)               \
    {                                                                    \
        type##Node *newNode = (type##Node *)malloc(sizeof(type##Node));  \
        if (!newNode)                                                    \
        {                                                                \
            fprintf(stderr, "Memory allocation failed\n");               \
            exit(EXIT_FAILURE);                                          \
        }                                                                \
        newNode->data = value;                                           \
        newNode->next = stack->top;                                      \
        stack->top = newNode;                                            \
        stack->size++;                                                   \
    }                                                                    \
                                                                         \
    type type##_stack_pop(type##Stack *stack)                            \
    {                                                                    \
        if (stack->top == NULL)                                          \
        {                                                                \
            fprintf(stderr, "Pop operation failed: stack is empty\n");   \
            exit(EXIT_FAILURE);                                          \
        }                                                                \
        type##Node *topNode = stack->top;                                \
        type value = topNode->data;                                      \
        stack->top = topNode->next;                                      \
        free(topNode);                                                   \
        stack->size--;                                                   \
        return value;                                                    \
    }                                                                    \
                                                                         \
    type type##_stack_top(type##Stack *stack)                            \
    {                                                                    \
        if (stack->top == NULL)                                          \
        {                                                                \
            fprintf(stderr, "Top operation failed: stack is empty\n");   \
            exit(EXIT_FAILURE);                                          \
        }                                                                \
        return stack->top->data;                                         \
    }                                                                    \
                                                                         \
    void type##_stack_free(type##Stack *stack)                           \
    {                                                                    \
        while (stack->top != NULL)                                       \
        {                                                                \
            type##Node *temp = stack->top;                               \
            stack->top = stack->top->next;                               \
            free(temp);                                                  \
        }                                                                \
        free(stack);                                                     \
    }

DECLARE_STACK_FUNCTIONS(int)

#endif // STACK_H
