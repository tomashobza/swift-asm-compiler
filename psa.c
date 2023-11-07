#include "psa.h"

char P_TABLE[10][10] = {
    {'-', '>', '>', '>', '>', '<', '<', '>', '>', '<'},
    {'<', '-', '>', '>', '>', '<', '<', '>', '>', '<'},
    {'<', '<', '-', '>', '>', '<', '<', '>', '>', '<'},
    {'<', '<', '<', '-', '>', '<', '-', '-', '>', '<'},
    {'<', '<', '<', '<', '-', '<', '-', '-', '>', '<'},
    {'>', '>', '>', '>', '>', '>', '>', '>', '>', '>'},
    {'<', '<', '<', '-', '-', '<', '-', '=', '>', '<'},
    {'>', '>', '>', '-', '-', '<', '-', '-', '>', '<'},
    {'<', '<', '<', '<', '<', '<', '<', '<', '-', '<'},
    {'>', '>', '>', '>', '>', '>', '>', '>', '>', '='},
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
    case TOKEN_EOL:
        return 9;
    case TOKEN_EOF:
    default:
        return 8;
    }
}

bool arrcmp(Token_type *arr1, Token_type *arr2, int len1, int len2)
{
    printf("tmp1: %d, tmp2: %d, len: %i\n", tmp1, tmp2, len);

    for (int i = 0; i < len; i++)
    {
        if (arr1[i] != arr2[i])
        {
            printf("%d != %d\n", arr1[i], arr2[i]);
            return false;
        }
    }
    return true;
}

Token_type getRule(Token_type *handle)
{
    /*
        E -> E*E
        E -> E/E
        E -> E+E
        E -> E-E
        E -> E==E
        E -> E!=E
        E -> E<E
        E -> E>E
        E -> E<=E
        E -> E>=E
        E -> E??E
        E -> i
        E -> (E)
        E -> !E
    */

    if (arrcmp(handle, (Token_type[]){(Token_type)TOKEN_EXPRSN, TOKEN_MUL, (Token_type)TOKEN_EXPRSN}, 3))
    {
        printf("rule: E -> E*E\n");
        return (Token_type)TOKEN_EXPRSN;
    }
    else if (arrcmp(handle, (Token_type[]){(Token_type)TOKEN_EXPRSN, TOKEN_DIV, (Token_type)TOKEN_EXPRSN}, 3))
    {
        printf("rule: E -> E/E\n");
        return (Token_type)TOKEN_EXPRSN;
    }
    else if (arrcmp(handle, (Token_type[]){(Token_type)TOKEN_EXPRSN, TOKEN_PLUS, (Token_type)TOKEN_EXPRSN}, 3))
    {
        printf("rule: E -> E+E\n");
        return (Token_type)TOKEN_EXPRSN;
    }
    else if (arrcmp(handle, (Token_type[]){(Token_type)TOKEN_EXPRSN, TOKEN_MINUS, (Token_type)TOKEN_EXPRSN}, 3))
    {
        printf("rule: E -> E-E\n");
        return (Token_type)TOKEN_EXPRSN;
    }
    else if (arrcmp(handle, (Token_type[]){(Token_type)TOKEN_EXPRSN, TOKEN_EQ, (Token_type)TOKEN_EXPRSN}, 3))
    {
        printf("rule: E -> E==E\n");
        return (Token_type)TOKEN_EXPRSN;
    }
    else if (arrcmp(handle, (Token_type[]){(Token_type)TOKEN_EXPRSN, TOKEN_NEQ, (Token_type)TOKEN_EXPRSN}, 3))
    {
        printf("rule: E -> E!=E\n");
        return (Token_type)TOKEN_EXPRSN;
    }
    else if (arrcmp(handle, (Token_type[]){(Token_type)TOKEN_EXPRSN, TOKEN_LESS, (Token_type)TOKEN_EXPRSN}, 3))
    {
        printf("rule: E -> E<E\n");
        return (Token_type)TOKEN_EXPRSN;
    }
    else if (arrcmp(handle, (Token_type[]){(Token_type)TOKEN_EXPRSN, TOKEN_MORE, (Token_type)TOKEN_EXPRSN}, 3))
    {
        printf("rule: E -> E>E\n");
        return (Token_type)TOKEN_EXPRSN;
    }
    else if (arrcmp(handle, (Token_type[]){(Token_type)TOKEN_EXPRSN, TOKEN_LESS_EQ, (Token_type)TOKEN_EXPRSN}, 3))
    {
        printf("rule: E -> E<=E\n");
        return (Token_type)TOKEN_EXPRSN;
    }
    else if (arrcmp(handle, (Token_type[]){(Token_type)TOKEN_EXPRSN, TOKEN_MORE_EQ, (Token_type)TOKEN_EXPRSN}, 3))
    {
        printf("rule: E -> E>=E\n");
        return (Token_type)TOKEN_EXPRSN;
    }
    else if (arrcmp(handle, (Token_type[]){(Token_type)TOKEN_EXPRSN, TOKEN_BINARY_OPERATOR, (Token_type)TOKEN_EXPRSN}, 3))
    {
        printf("rule: E -> E??E\n");
        return (Token_type)TOKEN_EXPRSN;
    }
    /*
        case TOKEN_IDENTIFICATOR:
        case TOKEN_INT:
        case TOKEN_DOUBLE:
        case TOKEN_EXP:
        case TOKEN_STRING:
    */
    else if (arrcmp(handle, (Token_type[]){TOKEN_IDENTIFICATOR}, 1))
    {
        printf("rule: E -> i\n");
        return (Token_type)TOKEN_EXPRSN;
    }
    else if (arrcmp(handle, (Token_type[]){TOKEN_INT}, 1))
    {
        printf("rule: E -> i\n");
        return (Token_type)TOKEN_EXPRSN;
    }
    else if (arrcmp(handle, (Token_type[]){TOKEN_DOUBLE}, 1))
    {
        printf("rule: E -> i\n");
        return (Token_type)TOKEN_EXPRSN;
    }
    else if (arrcmp(handle, (Token_type[]){TOKEN_EXP}, 1))
    {
        printf("rule: E -> i\n");
        return (Token_type)TOKEN_EXPRSN;
    }
    else if (arrcmp(handle, (Token_type[]){TOKEN_STRING}, 1))
    {
        printf("rule: E -> i\n");
        return (Token_type)TOKEN_EXPRSN;
    }
    else if (arrcmp(handle, (Token_type[]){TOKEN_L_BRACKET, (Token_type)TOKEN_EXPRSN, TOKEN_R_BRACKET}, 3))
    {
        printf("rule: E -> (E)\n");
        return (Token_type)TOKEN_EXPRSN;
    }
    else if (arrcmp(handle, (Token_type[]){TOKEN_NOT, (Token_type)TOKEN_EXPRSN}, 2))
    {
        printf("rule: E -> !E\n");
        return (Token_type)TOKEN_EXPRSN;
    }
    else
    {
        printf("rule: EOF\n");
        return TOKEN_EOF;
    }

    return TOKEN_EOF;
}

Token readNextToken()
{
    int ch = getchar();
    ungetc(ch, stdin);
    if (ch == EOF)
    {
        return (Token){
            .type = (Token_type)TOKEN_EOF,
            .token_value = "$",
        };
    }

    Token *tkn = malloc(sizeof(Token));
    generate_token(tkn, "\0", false);
    Token b = *tkn;
    free(tkn);

    printf("b: {'%s', %d}\n", b.token_value, b.type);
    return b;
}

// recursively prints the stack
void printStack(StackNode *top)
{
    if (top == NULL)
    {
        return;
    }
    printStack(top->next);
    printf("%s", ((Token *)top->data)->token_value);
    // printf("{'%s', %d} ", ((Token *)top->data)->token_value, ((Token *)top->data)->type);
}

/**
 * @brief Parses the expression using the precedent bottom-up parser. Reads tokens from the scanner.
 *
 * @return psa_return_type
 */
psa_return_type parse_expression()
{
    Stack *s = general_stack_init();
    psa_stack_push(s, (Token){
                          .type = (Token_type)TOKEN_EOF,
                          .token_value = "$"});
    printStack(s->top);
    printf("\n");

    int ch = getchar();
    ungetc(ch, stdin);
    Token b = readNextToken();

    Token *a = general_stack_top(s);

    printStack(s->top);
    printf("\n");
    // printf("b: %s\n", b.token_value);

    while (!(a->type == (Token_type)TOKEN_EXPRSN && s->size == 2 && b.type == (Token_type)TOKEN_EOF))
    {
        // if the stack top is of type (Token_type)TOKEN_EXPRSN, then we need to use the second top of the stack to determine the rule
        printf("a: {'%s': %d}\n", a->token_value, a->type);
        if (a->type == (Token_type)TOKEN_EXPRSN)
        {
            a = (Token *)(s->top->next->data);
        }
        printf("a: {'%s': %d}\n", a->token_value, a->type);

        printf("na stacku: ");
        printStack(s->top);
        printf("\n");
        printf("na vstupu: ");
        printf("{'%s', %d}\n", b.token_value, b.type);
        printf("P_TABLE[{%d, '%s'}][{%d, '%s'}] = %c\n", getSymbolValue(a->type), a->token_value, (b.type), b.token_value, P_TABLE[getSymbolValue(a->type)][getSymbolValue(b.type)]);

        switch (P_TABLE[getSymbolValue(a->type)][getSymbolValue(b.type)])
        {
        case '=':
            psa_stack_push(s, b);
            printStack(s->top);
            printf("\n");
            b = readNextToken();
            break;
        case '<':
            if (psa_stack_top(s).type == (Token_type)TOKEN_EXPRSN)
            {
                Token tmp = psa_stack_pop(s);
                psa_stack_push(s, (Token){
                                      .type = (Token_type)TOKEN_SHIFT,
                                      .token_value = "<"});
                psa_stack_push(s, tmp);
                psa_stack_push(s, b);
                b = readNextToken();
                printStack(s->top);
                printf("\n");
            }
            else
            {
                psa_stack_push(s, (Token){
                                      .type = (Token_type)TOKEN_SHIFT,
                                      .token_value = "<"});
                psa_stack_push(s, b);
                b = readNextToken();
                printStack(s->top);
                printf("\n");
            }
            break;
        case '>':
        { // from the top of the stack, pop all tokens until the first < is found
            // put all the popped tokens->type into an array
            // getTheRule of the array
            // if the rule is not EOF, push the rule into the stack
            // else, return error
            Token_type *handle = malloc(sizeof(Token_type) * s->size);
            printf("handle: ");
            int i = 0;
            while (psa_stack_top(s).type != TOKEN_SHIFT)
            {

                handle[i] = ((Token)psa_stack_pop(s)).type;
                printf("%s", ((Token *)s->top->data)->token_value);
                i++;
            }
            printf("\n");
            handle[i] = ((Token)psa_stack_pop(s)).type;
            // reverse handle
            Token_type tmp;
            for (int j = 0; j < i / 2; j++)
            {
                tmp = handle[j];
                handle[j] = handle[i - j - 1];
                handle[i - j - 1] = tmp;
            }

            Token_type rule = getRule(handle);

            if (rule != TOKEN_EOF)
            {
                psa_stack_push(s, (Token){
                                      .type = rule,
                                      .token_value = "E"});
            }
            else
            {
                printf("domrdalo se to!\n");
                return (psa_return_type){
                    .return_type = TOKEN_EOF,
                    .is_ok = false,
                };
            }
            printStack(s->top);
            printf("\n");
            break;
        }
        case '-':
        default:
            // printf("input {'%s', %d}\n", b.token_value, b.type);
            // printf("stack {'%s', %d}\n", a->token_value, a->type);
            return (psa_return_type){
                .return_type = TOKEN_EOF,
                .is_ok = false,
            };
        }

        printStack(s->top);
        printf("\n");

        printf("\n\n");

        a = general_stack_top(s);
    }

    printf("All good!\n");

    return (psa_return_type){
        .return_type = TOKEN_EOF,
        .is_ok = true,
    };
}