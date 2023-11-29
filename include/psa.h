/**
 * @file psa.h
 * @author Tomáš Hobza (xhobza03@vutbr.cz), Anastasia Butok (xbutok00@vutbr.cz)
 * @brief Header file for the precedent bottom-up parser module.
 * @version 0.1
 * @date 2023-11-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef PSA_H
#define PSA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#include "debug.h"
#include "colorful_printf.h"
#include "scanner.h"
#include "stack.h"
#include "symtable.h"

extern symtable_stack *sym_st;

#include "error.h"

// STRUCTS, ENUMS & GLOBALS

/**
 * @brief Struct to hold the return type of the precedent bottom-up parser.
 */
typedef struct
{
    Token_type end_token; // last token left
    bool is_ok;           // is the expression valid?
    Expression_type type; // type of the expression
} psa_return_type;

/**
 * @brief Struct to hold the token for the precedent syntactic analyzer.
 */
typedef struct
{
    Token_type type;
    char *token_value;
    Expression_type expr_type;
    bool preceded_by_nl;
    bool is_literal;
} PSA_Token;

/**
 * @brief PSA_Token that represents the end of the file. It is used as a bottom of the stack and for error states.
 */
#define PSA_TOKEN_EOF \
    (PSA_Token) { .type = TOKEN_EOF, .token_value = "$", .expr_type = TYPE_INVALID, .preceded_by_nl = true, .is_literal = false }

// STACK FUNCTIONS
DECLARE_STACK_FUNCTIONS(PSA_Token);

/**
 * @brief Precedence table for the precedent bottom-up parser.
 */
extern char P_TABLE[10][10];

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
    RULE_0 = TOKEN_FUNC_ID,                                                                    // E -> f
    RULE_1a = TOKEN_IDENTIFICATOR,                                                             // E -> i
    RULE_1b = TOKEN_INT,                                                                       // E -> i
    RULE_1c = TOKEN_DOUBLE,                                                                    // E -> i
    RULE_1d = TOKEN_EXP,                                                                       // E -> i
    RULE_1e = TOKEN_STRING,                                                                    // E -> i
    RULE_1f = TOKEN_BOOL,                                                                      // E -> i
    RULE_2 = (char)TOKEN_L_BRACKET << 16 | (char)TOKEN_EXPRSN << 8 | (char)TOKEN_R_BRACKET,    // E -> (E)
    RULE_3 = (char)TOKEN_NOT << 8 | (char)TOKEN_EXPRSN,                                        // E -> !E
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

/**
 * @brief Converts the handle to uint32_t (for the handle LUT).
 *
 * @param handle array of tokens (handle)
 * @param len length of the handle
 * @return uint32_t - handle converted to uint32_t
 */
uint32_t handleToUInt32(PSA_Token *handle, unsigned int len);

/**
 * @brief  Converts the handle to uint32_t (for the handle LUT) in reversed order, because the handle is read from the stack in reversed order.
 *
 * @param types array of types (handle) in reversed order
 * @param len length of the handle
 * @return uint32_t - handle converted to uint32_t
 */
uint32_t reverseHandleTypesToUInt32(Expression_type *types, unsigned int len);

/**
 * @brief Get the expression type from the token.
 *
 * @param token token to be evaluated
 * @return Expression_type - expression type of the token
 */
Expression_type getTypeFromToken(Token_type token);

/**
 * @brief Check if the token is a literal.
 *
 * @param token token to check
 * @return true
 * @return false
 */
bool isTokenLiteral(Token_type token);

/**
 * @brief Checks if the token is an operand.
 *
 * @param token token to check
 * @return true
 * @return false
 */
bool isTokenOperand(Token_type token);

/**
 * @brief Checks if the token is a binary operator.
 *
 * @param token token to check
 * @return true
 * @return false
 */
bool isTokenBinaryOperator(Token_type token);

/**
 * @brief Checks if the token is a bracket.
 *
 * @param token token to check
 * @return true
 * @return false
 */
bool isTokenBracket(Token_type token);

/**
 * @brief Checks if the token can be at the beginning of an expression.
 *
 * @param token token to check
 * @return true
 * @return false
 */
bool canTokenBeStartOfExpression(Token_type token);

/**
 * @brief Checks if the token can be at the end of an expression.
 *
 * @param token token to check
 * @return true
 * @return false
 */
bool canTokenBeEndOfExpression(Token_type token);

/**
 * @brief Get the ASCII character of the operation.
 *
 * @param token operation token
 * @return char
 */
char getOperationChar(Token_type token);

// PSA FUNCTIONS

/**
 * @brief Get the token derived from the hadle based on the rules.
 *
 * @param handle array of tokens (handle)
 * @param len length of the handle
 * @return PSA_Token - token derived from the handle
 */
PSA_Token getRule(PSA_Token *handle, unsigned int len);

/**
 * @brief Get all the tokens forming the handle from the stack.
 *
 * @param s stack of tokens
 * @return PSA_Token* - array of tokens (handle)
 */
PSA_Token *getHandleFromStack(PSA_Token_stack *s, int *i);

/**
 * @brief LUT that returns the value of the token for the precedence table.
 *
 * @param token - token to be evaluated
 * @return unsigned int - value of the token
 */
unsigned int getSymbolValue(Token_type token);

/**
 * @brief Returns the type of the expression based on the types of the operands.
    Supports: (int, int), (int, double), (double, int), (double, double)
 *
 * @param l_operand left operand
 * @param r_operand right operand
 * @return Expression_type
 */
Expression_type getTypeCombination(PSA_Token l_operand, PSA_Token r_operand);

/**
 * @brief Get the expression type based on the operands and the operation (for binary operations).
 *
 * @param l_operand left operand
 * @param operation operation
 * @param r_operand right operand
 * @return PSA_Token - token with the type of the expression
 */
PSA_Token getHandleType(PSA_Token l_operand, Token_type operation, PSA_Token r_operand);

/**
 * @brief Returns the expression type of an identifier from the symbol table.
 *
 * @param id token of the identifier
 * @return Expression_type
 */
Expression_type getIdType(PSA_Token id);

/**
 * @brief Checks if the type can be nil.
 *
 * @param type type to be checked
 * @return true
 * @return false
 */
bool canTypeBeNil(Expression_type type);

// PSA MAIN FUNCTION

/**
 * @brief Parses the expression using the precedent bottom-up parser. Reads tokens from the scanner.
 *
 * @return psa_return_type
 */
psa_return_type parse_expression();

/**
 * @brief Parses the expression using the precedent bottom-up parser. Reads tokens from the scanner.
 *
 * @param is_param Is the expression a function parameter? (, will be the end of the expression)
 * @return psa_return_type
 */
psa_return_type parse_expression_base(bool is_param);

/**
 * @brief Parses the expression that is a function parameter using the precedent bottom-up parser. Reads tokens from the scanner. Separating commas (,) are consumed, but closing bracket (]) is not.
 *
 * @return psa_return_type
 */
psa_return_type parse_expression_param();

// INPUT/OUTPUT FUNCTIONS

/**
 * @brief Reads the next token from the scanner and returns it. If the token is invalid, returns TOKEN_EOF. It also checks for errors based on the token on the top of the stack and the token that is being read.
 * `next_token_error = 0b000 -> no error`
 * `next_token_error = 0b001 -> missing operator`
 * `next_token_error = 0b010 -> illegal token`
 *
 * @param s stack of tokens
 * @param next_token_error error code
 * @param num_of_brackets number of brackets in the expression (NULL if not needed)
 * @return PSA_Token - next token
 */
PSA_Token readNextToken(PSA_Token_stack *s, char *next_token_error, int *num_of_brackets);

/**
 * @brief Prints the stack of tokens recursively.
 *
 * @param top top of the stack
 */
void printStackRec(PSA_Token_node *top);

/**
 * @brief Prints the stack of tokens.
 *
 * @param top top of the stack
 */
void printStack(PSA_Token_node *top);

/**
 * @brief Prints the token array.
 *
 * @param handle token array (handle)
 * @param len length of the handle
 */
void printTokenArray(PSA_Token *handle, unsigned int len);

// FUNCTION CALLING HANDLERS

/**
 * @brief Parses the calling of a function both syntactically and semantically.
 *
 * @param id PSA_Token contaning the id of the function
 * @return PSA_Token derivation of the function call
 */
PSA_Token parseFunctionCall(PSA_Token_stack *main_s, PSA_Token id);

/**
 * @brief Checks the validity of the parameters of the function call.
 *
 * @param main_s main PSA token stack
 * @param param_index index of the parameter
 * @param found_func symbol of the function from the symbol table
 * @param parsed_param pointer to where the parsed parameter (return struct of the PSA) will be saved
 * @param unknown_params are the parameters unknown?
 * @return true - the parameter is both syntactically and semantically valid
 * @return false - the parameter is not syntactically or semantically valid
 */
bool checkParameter(PSA_Token_stack *main_s, unsigned int param_index, symtable_item *found_func, psa_return_type *parsed_param, bool unknown_params);

/**
 * @brief Checks if the parameter name matches the name of the parameter in the function definition (if there should be a name).
 *
 * @param main_s main expression token stack
 * @param param_index index of the parameter
 * @return true
 * @return false
 */
bool checkParamName(PSA_Token_stack *main_s, unsigned int param_index, symtable_item *found_func, bool unknown_params);

#include "utils.h"
#include "semantic.h"

#endif // PSA_H
