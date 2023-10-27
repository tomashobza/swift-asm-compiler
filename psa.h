#ifndef PSA_H
#define PSA_H

#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>
#include "general_stack.h"

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
 * @brief Parses the expression using the precedent bottom-up parser. Reads tokens from the scanner.
 *
 * @return psa_return_type
 */
psa_return_type parse_expression();

/**
 * @brief Checks if the given token array is a handle.
 *
 * @param arr1
 * @param arr2
 * @param len
 * @return true
 * @return false
 */
bool arrcmp(Token_type *arr1, Token_type *arr2, int len);

/**
 * @brief Returns the rule for the given handle.
 *
 * @param handle - handle to be evaluated
 * @return Token_type - rule for the given handle
 */
Token_type getRule(Token_type *handle);

/**
 * @brief LUT that returns the value of the token for the precedence table.
 *
 * @param token - token to be evaluated
 * @return unsigned int - value of the token
 */
unsigned int getValue(Token_type token)
{
    // symbols: !,* /,+ -,== != < > <= >=,??,i,(,),$

    switch (token)
    {
    case TOKEN_NOT:
        return 0;
    case TOKEN_MUL:
    case TOKEN_DIV:
        return 1;
    case TOKEN_PLUS:
    case TOKEN_MINUS:
        return 2;
    case TOKEN_EQ:
    case TOKEN_NEQ:
    case TOKEN_LESS:
    case TOKEN_MORE:
    case TOKEN_LESS_EQ:
    case TOKEN_MORE_EQ:
        return 3;
    case TOKEN_BINARY_OPERATOR:
        return 4;
    case TOKEN_IDENTIFICATOR:
    case TOKEN_INT:
    case TOKEN_DOUBLE:
    case TOKEN_EXP:
    case TOKEN_STRING:
        return 5;
    case TOKEN_L_BRACKET:
        return 6;
    case TOKEN_R_BRACKET:
        return 7;
    case TOKEN_EOF:
    default:
        return 8;
    }
}

/**
 * @brief Precedence table for the precedent bottom-up parser.
 *
 */
char P_TABLE[9][9] = {
    {'-', '<', '<', '<', '<', '>', '<', '>', '>'},
    {'>', '>', '<', '<', '<', '>', '<', '>', '>'},
    {'>', '>', '>', '<', '<', '>', '<', '>', '>'},
    {'>', '>', '>', '-', '<', '>', '<', '>', '>'},
    {'>', '>', '>', '>', '<', '>', '<', '>', '>'},
    {'<', '<', '<', '<', '<', '<', '<', '<', '<'},
    {'<', '<', '<', '<', '<', '>', '<', '=', '>'},
    {'>', '>', '>', '>', '>', '>', '<', '>', '>'},
    {'<', '<', '<', '<', '<', '>', '<', '<', '<'},
};

#endif // PSA_H
