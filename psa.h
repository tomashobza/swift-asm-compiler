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

/**
 * @brief Rules for the precedent bottom-up parser.
    1. E -> i
    2. E -> (E)
    3. E -> !E
    4. E -> +E
    5. E -> -E
    6. E -> E*E
    7. E -> E/E
    8. E -> E+E
    9. E -> E-E
    10. E -> E==E
    11. E -> E!=E
    12. E -> E<E
    13. E -> E>E
    14. E -> E<=E
    15. E -> E>=E
    16. E -> E&&E
    17. E -> E||E
    18. E -> E??E
*/
typedef enum
{
    RULE_1a = TOKEN_IDENTIFICATOR,                                                             // E -> i
    RULE_1b = TOKEN_INT,                                                                       // E -> i
    RULE_1c = TOKEN_DOUBLE,                                                                    // E -> i
    RULE_1d = TOKEN_EXP,                                                                       // E -> i
    RULE_1e = TOKEN_STRING,                                                                    // E -> i
    RULE_2 = (char)TOKEN_L_BRACKET << 16 | (char)TOKEN_EXPRSN << 8 | (char)TOKEN_R_BRACKET,    // E -> (E)
    RULE_3 = (char)TOKEN_NOT << 8 | (char)TOKEN_EXPRSN,                                        // E -> !E
    RULE_4 = (char)TOKEN_PLUS << 8 | (char)TOKEN_EXPRSN,                                       // E -> +E
    RULE_5 = (char)TOKEN_MINUS << 8 | (char)TOKEN_EXPRSN,                                      // E -> -E
    RULE_6 = (char)TOKEN_EXPRSN << 16 | (char)TOKEN_MUL << 8 | (char)TOKEN_EXPRSN,             // E -> E*E
    RULE_7 = (char)TOKEN_EXPRSN << 16 | (char)TOKEN_DIV << 8 | (char)TOKEN_EXPRSN,             // E -> E/E
    RULE_8 = (char)TOKEN_EXPRSN << 16 | (char)TOKEN_PLUS << 8 | (char)TOKEN_EXPRSN,            // E -> E+E
    RULE_9 = (char)TOKEN_EXPRSN << 16 | (char)TOKEN_MINUS << 8 | (char)TOKEN_EXPRSN,           // E -> E-E
    RULE_10 = (char)TOKEN_EXPRSN << 16 | (char)TOKEN_EQ << 8 | (char)TOKEN_EXPRSN,             // E -> E==E
    RULE_11 = (char)TOKEN_EXPRSN << 16 | (char)TOKEN_NEQ << 8 | (char)TOKEN_EXPRSN,            // E -> E!=E
    RULE_12 = (char)TOKEN_EXPRSN << 16 | (char)TOKEN_LESS << 8 | (char)TOKEN_EXPRSN,           // E -> E<E
    RULE_13 = (char)TOKEN_EXPRSN << 16 | (char)TOKEN_MORE << 8 | (char)TOKEN_EXPRSN,           // E -> E>E
    RULE_14 = (char)TOKEN_EXPRSN << 16 | (char)TOKEN_LESS_EQ << 8 | (char)TOKEN_EXPRSN,        // E -> E<=E
    RULE_15 = (char)TOKEN_EXPRSN << 16 | (char)TOKEN_MORE_EQ << 8 | (char)TOKEN_EXPRSN,        // E -> E>=E
    RULE_16 = (char)TOKEN_EXPRSN << 16 | (char)TOKEN_AND << 8 | (char)TOKEN_EXPRSN,            // E -> E&&E
    RULE_17 = (char)TOKEN_EXPRSN << 16 | (char)TOKEN_OR << 8 | (char)TOKEN_EXPRSN,             // E -> E||E
    RULE_18 = (char)TOKEN_EXPRSN << 16 | (char)TOKEN_BINARY_OPERATOR << 8 | (char)TOKEN_EXPRSN // E -> E??E
} PSA_Rules;

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
