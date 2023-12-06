/**
 * @file ptable.c
 * @author Anastasia Butok (xbutok00@vutbr.cz)
 * @brief Precedence table for PSA along with getSymbolValue LUT.
 * @version 0.1
 * @date 2023-11-24
 *
 * @copyright Copyright (c) 2023
 * Project: IFJ compiler
 *
 */

#include "psa.h"

char P_TABLE[10][10] = {
    // INPUT >
    //!	   */   +-  REL  LOG  ??   i    (    )	  $
    {'-', '>', '>', '>', '>', '>', '<', '<', '>', '>'}, // !
    {'<', '>', '>', '>', '>', '>', '<', '<', '>', '>'}, // */
    {'<', '<', '>', '>', '>', '>', '<', '<', '>', '>'}, // +-
    {'<', '<', '<', '>', '>', '>', '<', '<', '>', '>'}, // REL
    {'<', '<', '<', '<', '>', '>', '<', '<', '>', '>'}, // LOG
    {'<', '<', '<', '<', '<', '>', '<', '<', '>', '>'}, // ??
    {'>', '>', '>', '>', '>', '>', '-', '-', '>', '>'}, // i
    {'<', '<', '<', '<', '<', '<', '<', '<', '=', '-'}, // (
    {'>', '>', '>', '>', '>', '>', '-', '-', '>', '>'}, // )
    {'<', '<', '<', '<', '<', '<', '<', '<', '-', '-'}, // $
                                                        // ^ TOP OF STACK

    // REL = RELATIONAL OPERATOR (==, !=, <, >, <=, >=)
    // LOG = LOGICAL OPERATOR (&&, ||)
};

unsigned int getSymbolValue(Token_type token)
{
    // symbols: !,* /,+ -,== != < > <= >=,??,i,(,),$

    switch (token)
    {
    // !
    case TOKEN_NOT:
        return 0;
    // */
    case TOKEN_MUL:
    case TOKEN_DIV:
        return 1;
    // +-
    case TOKEN_PLUS:
    case TOKEN_MINUS:
        return 2;
    // REL: == != < > <= >=
    case TOKEN_EQ:
    case TOKEN_NEQ:
    case TOKEN_LESS:
    case TOKEN_MORE:
    case TOKEN_LESS_EQ:
    case TOKEN_MORE_EQ:
        return 3;
    // LOG: && ||
    case TOKEN_AND:
    case TOKEN_OR:
        return 4;
    // ??
    case TOKEN_BINARY_OPERATOR:
        return 5;
    // i
    case TOKEN_IDENTIFICATOR:
    case TOKEN_FUNC_ID:
    case TOKEN_INT:
    case TOKEN_DOUBLE:
    case TOKEN_EXP:
    case TOKEN_BOOL:
    case TOKEN_STRING:
    case TOKEN_NIL:
        return 6;
    // (
    case TOKEN_L_BRACKET:
        return 7;
    // )
    case TOKEN_R_BRACKET:
        return 8;
    // $
    case TOKEN_EOF:
        return 9;
    // error
    default:
        return 99;
    }
}
