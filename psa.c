#include "psa.h"

#define TOKEN_EXPRSN (Token_type)100

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

bool arrcmp(Token_type *arr1, Token_type *arr2, int len)
{
    for (int i = 0; i < len; i++)
    {
        if (arr1[i] != arr2[i])
        {
            return false;
        }
    }
    return true;
}

Token_type getRule(Token_type *handle)
{
    /*
        O -> *
        O -> /
        O -> +
        O -> -
        O -> ==
        O -> !=
        O -> <
        O -> >
        O -> <=
        O -> >=
        O -> ??
        O -> \nO
        O -> O\n
        E -> i
        E -> (E)
        E -> !E
        E -> EOE
    */

    if (arrcmp(handle, (Token_type[]){TOKEN_MUL}, 1))
    {
        return TOKEN_EXPRSN;
    }
    else if (arrcmp(handle, (Token_type[]){TOKEN_DIV}, 1))
    {
        return TOKEN_EXPRSN;
    }
    else if (arrcmp(handle, (Token_type[]){TOKEN_PLUS}, 1))
    {
        return TOKEN_EXPRSN;
    }
    else if (arrcmp(handle, (Token_type[]){TOKEN_MINUS}, 1))
    {
        return TOKEN_EXPRSN;
    }
    else if (arrcmp(handle, (Token_type[]){TOKEN_EQ}, 1))
    {
        return TOKEN_EXPRSN;
    }
    else if (arrcmp(handle, (Token_type[]){TOKEN_NEQ}, 1))
    {
        return TOKEN_EXPRSN;
    }
    else if (arrcmp(handle, (Token_type[]){TOKEN_LESS}, 1))
    {
        return TOKEN_EXPRSN;
    }
    else if (arrcmp(handle, (Token_type[]){TOKEN_MORE}, 1))
    {
        return TOKEN_EXPRSN;
    }
    else if (arrcmp(handle, (Token_type[]){TOKEN_LESS_EQ}, 1))
    {
        return TOKEN_EXPRSN;
    }
    else if (arrcmp(handle, (Token_type[]){TOKEN_MORE_EQ}, 1))
    {
        return TOKEN_EXPRSN;
    }
    else if (arrcmp(handle, (Token_type[]){TOKEN_BINARY_OPERATOR}, 1))
    {
        return TOKEN_EXPRSN;
    }

    return TOKEN_EOF;
}

/**
 * @brief Parses the expression using the precedent bottom-up parser. Reads tokens from the scanner.
 *
 * @return psa_return_type
 */
psa_return_type parse_expression()
{
    Token *token = malloc(sizeof(Token));
    Stack *s = general_stack_init();

    int ch = getchar();
    while (ch != EOF)
    {
        ungetc(ch, stdin);
        generate_token(token, "\0", true);
        // main_scanner(token);
        printf("'%s', ", token->token_value);
        ch = getchar();
    }

    free(token);

    return (psa_return_type){
        .return_type = TOKEN_EOF,
        .is_ok = true,
    };
}