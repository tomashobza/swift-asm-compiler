#ifndef PSA_H
#define PSA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "psa_stack.h"
#include <math.h>

// STRUCTS, ENUMS & GLOBALS

/**
 * @brief Struct to hold the return type of the precedent bottom-up parser.
 *
 */
typedef struct
{
    Token_type return_type;
    bool is_ok;
} psa_return_type;

/**
 * @brief Precedence table for the precedent bottom-up parser.
 *
 */
extern char P_TABLE[10][10];

typedef enum
{
    TOKEN_EXPRSN = TOKEN_COUNT,
    TOKEN_SHIFT,
    TOKEN_UNSHIFT,
} PSA_Token_type;

// UTILITY FUNCTIONS

/**
 * @brief Checks if the given token array is a handle.
 *
 * @param arr1
 * @param arr2
 * @param len
 * @return true
 * @return false
 */
bool arrcmp(Token_type *arr1, unsigned int len1, Token_type *arr2, unsigned int len2);

uint32_t handleToUInt32(Token_type *handle, unsigned int len);

// PSA FUNCTIONS

/**
 * @brief Returns the rule for the given handle.
 *
 * @param handle - handle to be evaluated
 * @return Token_type - rule for the given handle
 */
Token_type getRule(Token_type *handle, unsigned int len);

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

#endif // PSA_H
