#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>

// #include "psa.h" // Include psa.h if it provides necessary types for stack.h

#define DECLARE_STACK_FUNCTIONS(type)                        \
    typedef struct type##_node                               \
    {                                                        \
        type data;                                           \
        struct type##_node *next;                            \
    } type##_node;                                           \
                                                             \
    typedef struct                                           \
    {                                                        \
        type##_node *top;                                    \
        unsigned int size;                                   \
    } type##_stack;                                          \
                                                             \
    type##_stack *type##_stack_init();                       \
    void type##_stack_push(type##_stack *stack, type value); \
    type type##_stack_pop(type##_stack *stack);              \
    type type##_stack_top(type##_stack *stack);              \
    void type##_stack_free(type##_stack *stack);

#define DEFINE_STACK_FUNCTIONS(type)                                        \
    type##_stack *type##_stack_init()                                       \
    {                                                                       \
        type##_stack *stack = (type##_stack *)malloc(sizeof(type##_stack)); \
        if (!stack)                                                         \
        {                                                                   \
            fprintf(stderr, "Memory allocation failed for stack\n");        \
            exit(EXIT_FAILURE);                                             \
        }                                                                   \
        stack->top = NULL;                                                  \
        return stack;                                                       \
    }                                                                       \
                                                                            \
    void type##_stack_push(type##_stack *stack, type value)                 \
    {                                                                       \
        type##_node *newNode = (type##_node *)malloc(sizeof(type##_node));  \
        if (!newNode)                                                       \
        {                                                                   \
            fprintf(stderr, "Memory allocation failed\n");                  \
            exit(EXIT_FAILURE);                                             \
        }                                                                   \
        newNode->data = value;                                              \
        newNode->next = stack->top;                                         \
        stack->top = newNode;                                               \
        stack->size++;                                                      \
    }                                                                       \
                                                                            \
    type type##_stack_pop(type##_stack *stack)                              \
    {                                                                       \
        if (stack->top == NULL)                                             \
        {                                                                   \
            fprintf(stderr, "Pop operation failed: stack is empty\n");      \
            exit(EXIT_FAILURE);                                             \
        }                                                                   \
        type##_node *topNode = stack->top;                                  \
        type value = topNode->data;                                         \
        stack->top = topNode->next;                                         \
        free(topNode);                                                      \
        stack->size--;                                                      \
        return value;                                                       \
    }                                                                       \
                                                                            \
    type type##_stack_top(type##_stack *stack)                              \
    {                                                                       \
        if (stack->top == NULL)                                             \
        {                                                                   \
            fprintf(stderr, "Top operation failed: stack is empty\n");      \
            exit(EXIT_FAILURE);                                             \
        }                                                                   \
        return stack->top->data;                                            \
    }                                                                       \
                                                                            \
    void type##_stack_free(type##_stack *stack)                             \
    {                                                                       \
        while (stack->top != NULL)                                          \
        {                                                                   \
            type##_node *temp = stack->top;                                 \
            stack->top = stack->top->next;                                  \
            free(temp);                                                     \
        }                                                                   \
        free(stack);                                                        \
    }                                                                       \
                                                                            \
    bool type##_stack_empty(type##_stack *stack)                            \
    {                                                                       \
        if (stack->top != NULL)                                             \
        {                                                                   \
            return false;                                                   \
        }                                                                   \
        return true;                                                        \
    }

DECLARE_STACK_FUNCTIONS(int)

#endif // STACK_H
