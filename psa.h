#ifndef PSA_H
#define PSA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include "colorful_printf.h"
#include "general_stack.h"
#include "scanner.h"

// STRUCTS, ENUMS & GLOBALS

typedef enum
{
    TYPE_INVALID = -1,
    TYPE_INT = 0,
    TYPE_DOUBLE = 1,
    TYPE_STRING = 2,
    TYPE_BOOL = 3,
    TYPE_NIL = 4,
} Expression_type;

/**
 * @brief Struct to hold the return type of the precedent bottom-up parser.
 *
 */
typedef struct
{
    Token_type return_type;
    bool is_ok;
} psa_return_type;

typedef struct
{
    Token_type type;
    char *token_value;
    // uint32_t handle_val;
    Expression_type expr_type;
} PSA_Token;

/**
 * @brief Precedence table for the precedent bottom-up parser.
 *
 */
extern char P_TABLE[10][10];

typedef enum
{
    TOKEN_EXPRSN = TOKEN_COUNT, // E - 58
    TOKEN_SHIFT,                // < - 59
    TOKEN_UNSHIFT,              // > - 60
} PSA_Token_type;

// UTILITY FUNCTIONS

// PSA FUNCTIONS

/**
 * @brief Returns the rule for the given handle.
 *
 * @param handle - handle to be evaluated
 * @return Token_type - rule for the given handle
 */
Token_type getRule(uint32_t handle_val);

/**
 * @brief LUT that returns the value of the token for the precedence table.
 *
 * @param token - token to be evaluated
 * @return unsigned int - value of the token
 */
unsigned int getSymbolValue(Token_type token);

// PSA MAIN FUNCTION

/**
 * @brief Parses the expression using the precedent bottom-up parser. Reads tokens from the scanner.
 *
 * @return psa_return_type
 */
psa_return_type parse_expression();

// PSA STACK

void psa_stack_push(Stack *s, PSA_Token data);

PSA_Token psa_stack_pop(Stack *s);

PSA_Token psa_stack_top(Stack *s);

#endif // PSA_H
