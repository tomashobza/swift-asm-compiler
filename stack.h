/**
 * @file stack.h
 * @author Tomáš Hobza (xhobza03@vutbr.cz)
 * @brief Generic stack implementation using linked list and macros.
 * @version 0.1
 * @date 2023-11-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
    void type##_stack_free(type##_stack *stack);             \
    bool type##_stack_empty(type##_stack *stack);

#define DEFINE_STACK_FUNCTIONS(type)                                           \
    type##_stack *type##_stack_init()                                          \
    {                                                                          \
        type##_stack *stack = (type##_stack *)malloc(sizeof(type##_stack));    \
        if (!stack)                                                            \
        {                                                                      \
            fprintf(stderr, "Memory allocation failed for stack\n");           \
            throw_error(INTERNAL_ERR, -1, "Stack is allocation failed.")       \
        }                                                                      \
        stack->top = NULL;                                                     \
        stack->size = 0;                                                       \
        return stack;                                                          \
    }                                                                          \
                                                                               \
    void type##_stack_push(type##_stack *stack, type value)                    \
    {                                                                          \
        type##_node *newNode = (type##_node *)malloc(sizeof(type##_node));     \
        if (!newNode)                                                          \
        {                                                                      \
            fprintf(stderr, "Memory allocation failed\n");                     \
            throw_error(INTERNAL_ERR, -1, "Stack new node allocation failed.") \
        }                                                                      \
        newNode->data = value;                                                 \
        newNode->next = stack->top;                                            \
        stack->top = newNode;                                                  \
        stack->size++;                                                         \
    }                                                                          \
                                                                               \
    type type##_stack_pop(type##_stack *stack)                                 \
    {                                                                          \
        if (stack->top == NULL || type##_stack_empty(stack))                   \
        {                                                                      \
            fprintf(stderr, "Pop operation failed: stack is empty\n");         \
            throw_error(INTERNAL_ERR, -1, "Stack is empty when popping.")      \
        }                                                                      \
        type##_node *topNode = stack->top;                                     \
        type value = topNode->data;                                            \
        stack->top = topNode->next;                                            \
        free(topNode);                                                         \
        stack->size--;                                                         \
        return value;                                                          \
    }                                                                          \
                                                                               \
    type type##_stack_top(type##_stack *stack)                                 \
    {                                                                          \
        if (type##_stack_empty(stack))                                         \
        {                                                                      \
            fprintf(stderr, "Top operation failed: stack is empty\n");         \
            throw_error(INTERNAL_ERR, -1, "Stack is empty in stack top.")      \
        }                                                                      \
        return stack->top->data;                                               \
    }                                                                          \
                                                                               \
    void type##_stack_free(type##_stack *stack)                                \
    {                                                                          \
        while (stack->top != NULL)                                             \
        {                                                                      \
            type##_node *temp = stack->top;                                    \
            stack->top = stack->top->next;                                     \
            free(temp);                                                        \
        }                                                                      \
        free(stack);                                                           \
    }                                                                          \
                                                                               \
    bool type##_stack_empty(type##_stack *stack)                               \
    {                                                                          \
        if (stack != NULL && stack->top != NULL && stack->size > 0)            \
        {                                                                      \
            return false;                                                      \
        }                                                                      \
        return true;                                                           \
    }

DECLARE_STACK_FUNCTIONS(int)

#endif // STACK_H
