#include "utils.h"

void print_token_type(Token_type type)
{
    switch (type)
    {
    case TOKEN_IF:
        printf_cyan("TOKEN_IF");
        break;
    case TOKEN_ELSE:
        printf_cyan("TOKEN_ELSE");
        break;
    case TOKEN_WHILE:
        printf_cyan("TOKEN_WHILE");
        break;
    case TOKEN_RETURN:
        printf_cyan("TOKEN_RETURN");
        break;
    case TOKEN_VAR:
        printf_cyan("TOKEN_VAR");
        break;
    case TOKEN_LET:
        printf_cyan("TOKEN_LET");
        break;
    case TOKEN_BREAK:
        printf_cyan("TOKEN_BREAK");
        break;
    case TOKEN_CONTINUE:
        printf_cyan("TOKEN_CONTINUE");
        break;
    case TOKEN_TYPE_STRING:
        printf_cyan("TOKEN_TYPE_STRING");
        break;
    case TOKEN_TYPE_INT:
        printf_cyan("TOKEN_TYPE_INT");
        break;
    case TOKEN_TYPE_DOUBLE:
        printf_cyan("TOKEN_TYPE_DOUBLE");
        break;
    case TOKEN_TYPE_BOOL:
        printf_cyan("TOKEN_TYPE_BOOL");
        break;
    case TOKEN_FUNC:
        printf_cyan("TOKEN_FUNC");
        break;
    case TOKEN_TYPE_SUFFIX:
        printf_cyan("TOKEN_TYPE_SUFFIX");
        break;
    case TOKEN_IDENTIFICATOR:
        printf_cyan("TOKEN_IDENTIFICATOR");
        break;
    case TOKEN_EOF:
        printf_cyan("TOKEN_EOF");
        break;
    case TOKEN_INT:
        printf_cyan("TOKEN_INT");
        break;
    case TOKEN_DOUBLE:
        printf_cyan("TOKEN_DOUBLE");
        break;
    case TOKEN_BOOL:
        printf_cyan("TOKEN_BOOL");
        break;
    case TOKEN_EXP:
        printf_cyan("TOKEN_EXP");
        break;
    case TOKEN_STRING:
        printf_cyan("TOKEN_STRING");
        break;
    case TOKEN_EQ:
        printf_cyan("TOKEN_EQ");
        break;
    case TOKEN_NEQ:
        printf_cyan("TOKEN_NEQ");
        break;
    case TOKEN_LESS:
        printf_cyan("TOKEN_LESS");
        break;
    case TOKEN_MORE:
        printf_cyan("TOKEN_MORE");
        break;
    case TOKEN_MORE_EQ:
        printf_cyan("TOKEN_MORE_EQ");
        break;
    case TOKEN_LESS_EQ:
        printf_cyan("TOKEN_LESS_EQ");
        break;
    case TOKEN_PLUS:
        printf_cyan("TOKEN_PLUS");
        break;
    case TOKEN_MINUS:
        printf_cyan("TOKEN_MINUS");
        break;
    case TOKEN_MUL:
        printf_cyan("TOKEN_MUL");
        break;
    case TOKEN_DIV:
        printf_cyan("TOKEN_DIV");
        break;
    case TOKEN_BINARY_OPERATOR:
        printf_cyan("TOKEN_BINARY_OPERATOR");
        break;
    case TOKEN_ASSIGN:
        printf_cyan("TOKEN_ASSIGN");
        break;
    case TOKEN_L_BRACKET:
        printf_cyan("TOKEN_L_BRACKET");
        break;
    case TOKEN_R_BRACKET:
        printf_cyan("TOKEN_R_BRACKET");
        break;
    case TOKEN_R_CURLY:
        printf_cyan("TOKEN_R_CURLY");
        break;
    case TOKEN_L_CURLY:
        printf_cyan("TOKEN_L_CURLY");
        break;
    case TOKEN_COMMA:
        printf_cyan("TOKEN_COMMA");
        break;
    case TOKEN_ARROW:
        printf_cyan("TOKEN_ARROW");
        break;
    case TOKEN_NIL:
        printf_cyan("TOKEN_NIL");
        break;
    case TOKEN_DOUBLE_DOT:
        printf_cyan("TOKEN_DOUBLE_DOT");
        break;
    case TOKEN_NOT:
        printf_cyan("TOKEN_NOT");
        break;
    case TOKEN_AND:
        printf_cyan("TOKEN_AND");
        break;
    case TOKEN_OR:
        printf_cyan("TOKEN_OR");
        break;
    case TOKEN_UNDERSCORE:
        printf_cyan("TOKEN_UNDERSCORE");
        break;
    case TOKEN_READSTRING:
        printf_cyan("TOKEN_READSTRING");
        break;
    case TOKEN_READINT:
        printf_cyan("TOKEN_READINT");
        break;
    case TOKEN_READDOUBLE:
        printf_cyan("TOKEN_READDOUBLE");
        break;
    case TOKEN_WRITE:
        printf_cyan("TOKEN_WRITE");
        break;
    case TOKEN_INT2DOUBLE:
        printf_cyan("TOKEN_INT2DOUBLE");
        break;
    case TOKEN_DOUBLE2INT:
        printf_cyan("TOKEN_DOUBLE2INT");
        break;
    case TOKEN_LENGTH:
        printf_cyan("TOKEN_LENGTH");
        break;
    case TOKEN_SUBSTRING:
        printf_cyan("TOKEN_SUBSTRING");
        break;
    case TOKEN_ORD:
        printf_cyan("TOKEN_ORD");
        break;
    case TOKEN_CHR:
        printf_cyan("TOKEN_CHR");
        break;
    case TOKEN_FUNC_ID:
        printf_cyan("TOKEN_FUNC_ID");
        break;
    case TOKEN_EXPRSN:
        printf_yellow("TOKEN_EXPRSN");
        break;
    case TOKEN_SHIFT:
        printf_yellow("TOKEN_SHIFT");
        break;
    case TOKEN_UNSHIFT:
        printf_yellow("TOKEN_UNSHIFT");
        break;
    default:
        printf_red("[invalid token]");
        break;
    }
    printf_cyan("\n");
}

void print_expression_type(Expression_type type)
{
    switch (type)
    {
    case TYPE_INVALID:
        printf_cyan("TYPE_INVALID");
        break;
    case TYPE_INT:
        printf_cyan("TYPE_INT");
        break;
    case TYPE_DOUBLE:
        printf_cyan("TYPE_DOUBLE");
        break;
    case TYPE_STRING:
        printf_cyan("TYPE_STRING");
        break;
    case TYPE_BOOL:
        printf_cyan("TYPE_BOOL");
        break;
    case TYPE_NIL:
        printf_cyan("TYPE_NIL");
        break;
    case TYPE_EMPTY:
        printf_cyan("TYPE_EMPTY");
        break;
    default:
        printf_red("[invalid type]");
        break;
    }
    printf("\n");
}

Token convertPSATokenToToken(PSA_Token psa_tkn)
{
    return (Token){
        .type = psa_tkn.type,
        .token_value = psa_tkn.token_value,
        .preceded_by_nl = psa_tkn.preceded_by_nl,
    };
}

PSA_Token convertTokenToPSAToken(Token tkn)
{
    return (PSA_Token){
        .type = tkn.type,
        .token_value = tkn.token_value,
        .preceded_by_nl = tkn.preceded_by_nl,
        .expr_type = getTypeFromToken(tkn.type),
        .canBeNil = false,
    };
}