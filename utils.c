/**
 * @file utils.c
 * @author Tomáš Hobza (xhobza03@vutbr.cz)
 * @brief Utility functions for the whole project.
 * @version 0.1
 * @date 2023-11-24
 *
 * @copyright Copyright (c) 2023
 * Project: IFJ compiler
 *
 */

#include "utils.h"

void fprint_token_type(Token_type type, FILE *out)
{
    switch (type)
    {
    case TOKEN_IF:
        fprintf_cyan(out, "TOKEN_IF");
        break;
    case TOKEN_ELSE:
        fprintf_cyan(out, "TOKEN_ELSE");
        break;
    case TOKEN_WHILE:
        fprintf_cyan(out, "TOKEN_WHILE");
        break;
    case TOKEN_RETURN:
        fprintf_cyan(out, "TOKEN_RETURN");
        break;
    case TOKEN_VAR:
        fprintf_cyan(out, "TOKEN_VAR");
        break;
    case TOKEN_LET:
        fprintf_cyan(out, "TOKEN_LET");
        break;
    case TOKEN_BREAK:
        fprintf_cyan(out, "TOKEN_BREAK");
        break;
    case TOKEN_CONTINUE:
        fprintf_cyan(out, "TOKEN_CONTINUE");
        break;
    case TOKEN_TYPE_STRING:
        fprintf_cyan(out, "TOKEN_TYPE_STRING");
        break;
    case TOKEN_TYPE_INT:
        fprintf_cyan(out, "TOKEN_TYPE_INT");
        break;
    case TOKEN_TYPE_DOUBLE:
        fprintf_cyan(out, "TOKEN_TYPE_DOUBLE");
        break;
    case TOKEN_TYPE_BOOL:
        fprintf_cyan(out, "TOKEN_TYPE_BOOL");
        break;
    case TOKEN_TYPE_STRING_NIL:
        fprintf_cyan(out, "TOKEN_TYPE_STRING_NIL");
        break;
    case TOKEN_TYPE_INT_NIL:
        fprintf_cyan(out, "TOKEN_TYPE_INT_NIL");
        break;
    case TOKEN_TYPE_DOUBLE_NIL:
        fprintf_cyan(out, "TOKEN_TYPE_DOUBLE_NIL");
        break;
    case TOKEN_TYPE_BOOL_NIL:
        fprintf_cyan(out, "TOKEN_TYPE_BOOL_NIL");
        break;
    case TOKEN_FUNC:
        fprintf_cyan(out, "TOKEN_FUNC");
        break;
    case TOKEN_IDENTIFICATOR:
        fprintf_cyan(out, "TOKEN_IDENTIFICATOR");
        break;
    case TOKEN_EOF:
        fprintf_cyan(out, "TOKEN_EOF");
        break;
    case TOKEN_INT:
        fprintf_cyan(out, "TOKEN_INT");
        break;
    case TOKEN_DOUBLE:
        fprintf_cyan(out, "TOKEN_DOUBLE");
        break;
    case TOKEN_BOOL:
        fprintf_cyan(out, "TOKEN_BOOL");
        break;
    case TOKEN_EXP:
        fprintf_cyan(out, "TOKEN_EXP");
        break;
    case TOKEN_STRING:
        fprintf_cyan(out, "TOKEN_STRING");
        break;
    case TOKEN_EQ:
        fprintf_cyan(out, "TOKEN_EQ");
        break;
    case TOKEN_NEQ:
        fprintf_cyan(out, "TOKEN_NEQ");
        break;
    case TOKEN_LESS:
        fprintf_cyan(out, "TOKEN_LESS");
        break;
    case TOKEN_MORE:
        fprintf_cyan(out, "TOKEN_MORE");
        break;
    case TOKEN_MORE_EQ:
        fprintf_cyan(out, "TOKEN_MORE_EQ");
        break;
    case TOKEN_LESS_EQ:
        fprintf_cyan(out, "TOKEN_LESS_EQ");
        break;
    case TOKEN_PLUS:
        fprintf_cyan(out, "TOKEN_PLUS");
        break;
    case TOKEN_MINUS:
        fprintf_cyan(out, "TOKEN_MINUS");
        break;
    case TOKEN_MUL:
        fprintf_cyan(out, "TOKEN_MUL");
        break;
    case TOKEN_DIV:
        fprintf_cyan(out, "TOKEN_DIV");
        break;
    case TOKEN_BINARY_OPERATOR:
        fprintf_cyan(out, "TOKEN_BINARY_OPERATOR");
        break;
    case TOKEN_ASSIGN:
        fprintf_cyan(out, "TOKEN_ASSIGN");
        break;
    case TOKEN_L_BRACKET:
        fprintf_cyan(out, "TOKEN_L_BRACKET");
        break;
    case TOKEN_R_BRACKET:
        fprintf_cyan(out, "TOKEN_R_BRACKET");
        break;
    case TOKEN_R_CURLY:
        fprintf_cyan(out, "TOKEN_R_CURLY");
        break;
    case TOKEN_L_CURLY:
        fprintf_cyan(out, "TOKEN_L_CURLY");
        break;
    case TOKEN_COMMA:
        fprintf_cyan(out, "TOKEN_COMMA");
        break;
    case TOKEN_ARROW:
        fprintf_cyan(out, "TOKEN_ARROW");
        break;
    case TOKEN_NIL:
        fprintf_cyan(out, "TOKEN_NIL");
        break;
    case TOKEN_DOUBLE_DOT:
        fprintf_cyan(out, "TOKEN_DOUBLE_DOT");
        break;
    case TOKEN_NOT:
        fprintf_cyan(out, "TOKEN_NOT");
        break;
    case TOKEN_AND:
        fprintf_cyan(out, "TOKEN_AND");
        break;
    case TOKEN_OR:
        fprintf_cyan(out, "TOKEN_OR");
        break;
    case TOKEN_UNDERSCORE:
        fprintf_cyan(out, "TOKEN_UNDERSCORE");
        break;
    case TOKEN_FUNC_ID:
        fprintf_cyan(out, "TOKEN_FUNC_ID");
        break;
    case TOKEN_EXPRSN:
        fprintf_yellow(out, "TOKEN_EXPRSN");
        break;
    case TOKEN_SHIFT:
        fprintf_yellow(out, "TOKEN_SHIFT");
        break;
    case TOKEN_UNSHIFT:
        fprintf_yellow(out, "TOKEN_UNSHIFT");
        break;
    default:
        fprintf_red(out, "[invalid token]");
        break;
    }
    fprintf_cyan(out, "\n");
}

void print_token_type(Token_type type)
{
    fprint_token_type(type, stdout);
}

void fprint_expression_type(Expression_type type, FILE *out)
{
    switch (type)
    {
    case TYPE_INVALID:
        fprintf_red(out, "TYPE_INVALID");
        break;
    case TYPE_INT:
        fprintf_cyan(out, "TYPE_INT");
        break;
    case TYPE_DOUBLE:
        fprintf_cyan(out, "TYPE_DOUBLE");
        break;
    case TYPE_STRING:
        fprintf_cyan(out, "TYPE_STRING");
        break;
    case TYPE_BOOL:
        fprintf_cyan(out, "TYPE_BOOL");
        break;
    case TYPE_NIL:
        fprintf_cyan(out, "TYPE_NIL");
        break;
    case TYPE_EMPTY:
        fprintf_cyan(out, "TYPE_EMPTY");
        break;
    case TYPE_INT_NIL:
        fprintf_cyan(out, "TYPE_INT_NIL");
        break;
    case TYPE_DOUBLE_NIL:
        fprintf_cyan(out, "TYPE_DOUBLE_NIL");
        break;
    case TYPE_STRING_NIL:
        fprintf_cyan(out, "TYPE_STRING_NIL");
        break;
    case TYPE_BOOL_NIL:
        fprintf_cyan(out, "TYPE_BOOL_NIL");
        break;
    default:
        fprintf_red(out, "[invalid type]");
        break;
    }
    fprintf(out, "\n");
}

void print_expression_type(Expression_type type)
{
    fprint_expression_type(type, stdout);
}

Token convertPSATokenToToken(PSA_Token psa_tkn)
{
    return (Token){
        .type = psa_tkn.type,
        .token_value = psa_tkn.token_value,
        .preceded_by_nl = psa_tkn.preceded_by_nl,
        .line_num = psa_tkn.line_num,
    };
}

PSA_Token convertTokenToPSAToken(Token tkn)
{
    return (PSA_Token){
        .type = tkn.type,
        .token_value = tkn.token_value,
        .preceded_by_nl = tkn.preceded_by_nl,
        .expr_type = getTypeFromToken(tkn.type),
        .is_literal = isTokenLiteral(tkn.type),
        .line_num = tkn.line_num,
    };
}