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
    case TOKEN_IDENTIFICATOR: // TODO: delete and for id use symbol table
        return (Expression_type)TYPE_DOUBLE;

    case TOKEN_STRING:
        return (Expression_type)TYPE_STRING;
    default:
        return (Expression_type)TYPE_INVALID;
    }
}

bool isTokenOperand(Token_type token)
{
    switch (token)
    {
    case TOKEN_INT:
    case TOKEN_DOUBLE:
    case TOKEN_EXP:
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

bool canTokenBeStartOfExpression(Token_type token)
{
    switch (token)
    {
    case TOKEN_INT:
    case TOKEN_DOUBLE:
    case TOKEN_EXP:
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
    case TOKEN_STRING:
    case TOKEN_R_BRACKET:
    case TOKEN_IDENTIFICATOR:
        return true;
    default:
        return false;
    }
}