#include "psa.h"

char P_TABLE[10][10] = {
    //!	   */   +-  LOG   ??   i    (    )	  $
    {'-', '>', '>', '>', '>', '<', '<', '>', '>'}, // !
    {'<', '>', '>', '>', '>', '<', '<', '>', '>'}, // */
    {'<', '<', '>', '>', '>', '<', '<', '>', '>'}, // +-
    {'<', '<', '<', '>', '>', '<', '<', '>', '>'}, // LOG
    {'<', '<', '<', '<', '>', '<', '<', '>', '>'}, // ??
    {'>', '>', '>', '>', '>', '-', '-', '>', '>'}, // i
    {'<', '<', '<', '<', '<', '<', '<', '=', '-'}, // (
    {'>', '>', '>', '>', '>', '-', '-', '>', '>'}, // )
    {'<', '<', '<', '<', '<', '<', '<', '-', '-'}, // $

    // LOG = LOGICAL OPERATOR (==, !=, <, >, <=, >=)
};

unsigned int getSymbolValue(Token_type token)
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
    case TOKEN_AND:
    case TOKEN_OR:
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
        return 8;
    default: // error
        return 99;
    }
}