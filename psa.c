#include "psa.h"

#define TOKEN_EXPRSN (Token_type)100

char P_TABLE[9][9] = {
    {'-', '>', '>', '>', '>', '<', '-', '>', '>'},
    {'<', '-', '>', '>', '>', '<', '-', '>', '>'},
    {'<', '>', '-', '>', '>', '<', '-', '>', '>'},
    {'<', '<', '<', '-', '>', '<', '-', '>', '>'},
    {'<', '<', '<', '<', '-', '<', '-', '>', '>'},
    {'>', '>', '>', '>', '>', '>', '>', '>', '>'},
    {'<', '<', '<', '<', '<', '<', '<', '=', '>'},
    {'-', '-', '-', '-', '-', '<', '-', '>', '>'},
    {'<', '<', '<', '<', '<', '<', '<', '<', '-'},
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

bool readNextToken(Token **tkn)
{
    int ch = getchar();
    ungetc(ch, stdin);
    if (*tkn == NULL)
    {
        *tkn = malloc(sizeof(Token));
    }
    if (ch == EOF)
    {
        (*tkn)->type = TOKEN_EOF;
        return false;
    }

    generate_token(*tkn, "\0", true);
    return true;
}

void printStack(Stack *s)
{
    StackNode *tmp = s->top;
    printf("Stack: ");
    while (tmp != NULL)
    {
        printf("%d ", ((Token *)tmp->data)->type);
        tmp = tmp->next;
    }
    printf("\n");
}

/**
 * @brief Parses the expression using the precedent bottom-up parser. Reads tokens from the scanner.
 *
 * @return psa_return_type
 */
psa_return_type parse_expression()
{
    Stack *s = general_stack_init();
    general_stack_push(
        s,
        &(Token){
            .type = TOKEN_EOF,
        });

    int ch = getchar();
    ungetc(ch, stdin);
    Token *b = NULL;
    readNextToken(&b);

    Token *a = general_stack_top(s);

    printStack(s);
    printf("b: %d\n", b->type);

    while (b->type != TOKEN_EOF && (s->size >= 1))
    {
        printf("P_TABLE[%d][%d] = %c\n", getSymbolValue(a->type), getSymbolValue(b->type), P_TABLE[getSymbolValue(a->type)][getSymbolValue(b->type)]);
        switch (P_TABLE[getSymbolValue(a->type)][getSymbolValue(b->type)])
        {
        case '=':
            general_stack_push(s, b);
            readNextToken(&b);
            break;
        case '<':
            if (((Token *)general_stack_top(s))->type == TOKEN_EXPRSN)
            {
                Token *tmp = general_stack_pop(s);
                general_stack_push(s, &(Token){
                                          .type = (Token_type)TOKEN_SHIFT,
                                      });
                general_stack_push(s, tmp);
                general_stack_push(s, b);
                readNextToken(&b);
            }
        case '>':
        { // from the top of the stack, pop all tokens until the first < is found
            // put all the popped tokens->type into an array
            // getTheRule of the array
            // if the rule is not EOF, push the rule into the stack
            // else, return error
            Token_type *handle = malloc(sizeof(Token_type) * s->size);
            int i = 0;
            while (((Token *)general_stack_top(s))->type != TOKEN_EXPRSN)
            {
                handle[i] = ((Token *)general_stack_pop(s))->type;
                i++;
            }
            handle[i] = ((Token *)general_stack_pop(s))->type;
            Token_type rule = getRule(handle);
            if (rule != TOKEN_EOF)
            {
                general_stack_push(s, &(Token){
                                          .type = rule,
                                      });
            }
            else
            {
                return (psa_return_type){
                    .return_type = TOKEN_EOF,
                    .is_ok = false,
                };
            }
        }
        }

        printStack(s);

        a = general_stack_top(s);
    }

    free(b);

    return (psa_return_type){
        .return_type = TOKEN_EOF,
        .is_ok = true,
    };
}