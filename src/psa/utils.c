/**
 * @file utils.c
 * @author Anastasia Butok (xbutok00@vutbr.cz)
 * @brief Utility functions for PSA.
 * @version 0.1
 * @date 2023-11-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "psa.h"

uint32_t handleToUInt32(PSA_Token *handle, unsigned int len)
{
    uint32_t result = 0;
    for (unsigned int i = 0; i < len; i++)
    {
        result = result << 8 | (char)(handle[i].type);
    }
    return result;
}

uint32_t reverseHandleTypesToUInt32(Expression_type *types, unsigned int len)
{
    uint32_t result = 0;
    for (int i = len - 1; i >= 0; i--)
    {
        result = result << 8 | (char)types[i];
    }
    return result;
}

Expression_type getTypeFromToken(Token_type token)
{
    switch (token)
    {
    case TOKEN_INT:
        return (Expression_type)TYPE_INT;
    case TOKEN_DOUBLE:
    case TOKEN_EXP:
        return (Expression_type)TYPE_DOUBLE;

    case TOKEN_BOOL:
        return (Expression_type)TYPE_BOOL;

    case TOKEN_STRING:
        return (Expression_type)TYPE_STRING;

    case TOKEN_NIL:
        return (Expression_type)TYPE_NIL;

    default:
        return (Expression_type)TYPE_INVALID;
    }
}

bool isTokenLiteral(Token_type token)
{
    switch (token)
    {
    case TOKEN_INT:
    case TOKEN_DOUBLE:
    case TOKEN_EXP:
    case TOKEN_BOOL:
    case TOKEN_STRING:
    case TOKEN_NIL:
        return true;
    default:
        return false;
    }
}

bool isTokenOperand(Token_type token)
{
    switch (token)
    {
    case TOKEN_INT:
    case TOKEN_DOUBLE:
    case TOKEN_EXP:
    case TOKEN_BOOL:
    case TOKEN_STRING:
    case TOKEN_EXPRSN:
    case TOKEN_IDENTIFICATOR:
        return true;
    default:
        return false;
    }
}

bool isTokenBinaryOperator(Token_type token)
{
    switch (token)
    {
    case TOKEN_MUL:
    case TOKEN_DIV:
    case TOKEN_PLUS:
    case TOKEN_MINUS:
    case TOKEN_EQ:
    case TOKEN_NEQ:
    case TOKEN_LESS:
    case TOKEN_MORE:
    case TOKEN_LESS_EQ:
    case TOKEN_MORE_EQ:
    case TOKEN_AND:
    case TOKEN_OR:
    case TOKEN_BINARY_OPERATOR:
        return true;
    default:
        return false;
    }
}

bool isTokenBracket(Token_type token)
{
    switch (token)
    {
    case TOKEN_L_BRACKET:
    case TOKEN_R_BRACKET:
        return true;
    default:
        return false;
    }
}

bool canTokenBeStartOfExpression(Token_type token)
{
    switch (token)
    {
    case TOKEN_INT:
    case TOKEN_DOUBLE:
    case TOKEN_EXP:
    case TOKEN_BOOL:
    case TOKEN_STRING:
    case TOKEN_NOT:
    case TOKEN_L_BRACKET:
    case TOKEN_IDENTIFICATOR:
        return true;
    default:
        return false;
    }
}

bool canTokenBeEndOfExpression(Token_type token)
{
    switch (token)
    {
    case TOKEN_INT:
    case TOKEN_DOUBLE:
    case TOKEN_EXP:
    case TOKEN_BOOL:
    case TOKEN_STRING:
    case TOKEN_R_BRACKET:
    case TOKEN_IDENTIFICATOR:
        return true;
    default:
        return false;
    }
}

bool canTypeBeNil(Expression_type type)
{
    switch (type)
    {
    case TYPE_INT_NIL:
    case TYPE_DOUBLE_NIL:
    case TYPE_BOOL_NIL:
    case TYPE_STRING_NIL:
    case TYPE_NIL:
        return true;
    default:
        return false;
    }
}

char getOperationChar(Token_type token)
{
    switch (token)
    {
    case TOKEN_MUL:
        return '*';
    case TOKEN_DIV:
        return '/';
    case TOKEN_PLUS:
        return '+';
    case TOKEN_MINUS:
        return '-';
    case TOKEN_EQ:
        return '=';
    case TOKEN_NEQ:
        return '!';
    case TOKEN_LESS:
        return '<';
    case TOKEN_MORE:
        return '>';
    case TOKEN_LESS_EQ:
        return 'L';
    case TOKEN_MORE_EQ:
        return 'M';
    case TOKEN_AND:
        return '&';
    case TOKEN_OR:
        return '|';
    default:
        return ' ';
    }
}

Expression_type removeTypeNil(Expression_type expr_type)
{
    switch (expr_type)
    {
    case TYPE_INT_NIL:
        return TYPE_INT;
    case TYPE_DOUBLE_NIL:
        return TYPE_DOUBLE;
    case TYPE_BOOL_NIL:
        return TYPE_BOOL;
    case TYPE_STRING_NIL:
        return TYPE_STRING;
    default:
        return TYPE_INVALID;
    }
}