#include "psa.h"

#define DEBUG 0

#if DEBUG
#define DEBUG_CODE(code) \
    do                   \
    {                    \
        code             \
    } while (0)
#else
#define DEBUG_CODE(code) \
    do                   \
    {                    \
    } while (0)
#endif

char P_TABLE[10][10] = {
    //!	   */   +-  LOG   ??   i    (    )	  $
    {'-', '>', '>', '>', '>', '<', '<', '>', '>'}, // !
    {'<', '>', '<', '>', '>', '<', '<', '>', '>'}, // */
    {'<', '<', '>', '>', '>', '<', '<', '>', '>'}, // +-
    {'<', '<', '<', '>', '>', '<', '<', '>', '>'}, // LOG
    {'<', '<', '<', '<', '>', '<', '<', '>', '>'}, // ??
    {'>', '>', '>', '>', '>', '>', '>', '>', '>'}, // i
    {'<', '<', '<', '<', '-', '<', '<', '=', '>'}, // (
    {'>', '>', '>', '>', '-', '<', '-', '>', '>'}, // )
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
        return 8;
    default: // error
        return 99;
    }
}

// TODO: delete this function
bool arrcmp(Token_type *arr1, unsigned int len1, Token_type *arr2, unsigned int len2)
{
    // //printf("len1: %d, len2: %d\n", len1, len2);

    if (len1 != len2)
    {
        return false;
    }

    for (int i = 0; i < len1; i++)
    {
        if (arr1[i] != arr2[i])
        {
            // printf("%d != %d\n", arr1[i], arr2[i]);
            return false;
        }
    }
    return true;
}

// TODO: delete this
uint32_t handleToUInt32(Token_type *handle, unsigned int len)
{
    uint32_t result = 0;

    for (int i = 0; i < len; i++)
    {
        result = result << 8 | (char)handle[i];
        printf("%d, ", handle[i]);
    }

    return result;
}

uint32_t reverseHandleToUInt32(Token_type *handle, unsigned int len)
{
    uint32_t result = 0;
    printf("\n{");
    for (int i = len - 1; i >= 0; i--)
    {
        result = result << 8 | (char)handle[i];
        printf("%d, ", handle[i]);
    }
    printf("}\n");
    printf("handleToUInt32: %d\n", result);
    return result;
}

Token_type getRule(uint32_t handle_val)
{
    /*
        E -> i
        E -> (E)
        E -> !E
        E -> +E
        E -> -E
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
    */
    switch (handle_val)
    {
    case TOKEN_IDENTIFICATOR:
    case TOKEN_INT:
    case TOKEN_DOUBLE:
    case TOKEN_EXP:
    case TOKEN_STRING:
        DEBUG_CODE(printf_cyan("rule: E -> i\n"););
        return (Token_type)TOKEN_EXPRSN;
    case (char)TOKEN_L_BRACKET << 16 | (char)TOKEN_EXPRSN << 8 | (char)TOKEN_R_BRACKET:
        DEBUG_CODE(printf_cyan("rule: E -> (E)\n"););
        return (Token_type)TOKEN_EXPRSN;
    case (char)TOKEN_NOT << 8 | (char)TOKEN_EXPRSN:
        DEBUG_CODE(printf_cyan("rule: E -> !E\n"););
        return (Token_type)TOKEN_EXPRSN;
    case (char)TOKEN_PLUS << 8 | (char)TOKEN_EXPRSN:
        DEBUG_CODE(printf_cyan("rule: E -> +E\n"););
    case (char)TOKEN_MINUS << 8 | (char)TOKEN_EXPRSN:
        DEBUG_CODE(printf_cyan("rule: E -> -E\n"););
        return (Token_type)TOKEN_EXPRSN;
    case (char)TOKEN_EXPRSN << 16 | (char)TOKEN_MUL << 8 | (char)TOKEN_EXPRSN:
        DEBUG_CODE(printf_cyan("rule: E -> E*E\n"););
        return (Token_type)TOKEN_EXPRSN;
    case (char)TOKEN_EXPRSN << 16 | (char)TOKEN_DIV << 8 | (char)TOKEN_EXPRSN:
        DEBUG_CODE(printf_cyan("rule: E -> E/E\n"););
        return (Token_type)TOKEN_EXPRSN;
    case (char)TOKEN_EXPRSN << 16 | (char)TOKEN_PLUS << 8 | (char)TOKEN_EXPRSN:
        DEBUG_CODE(printf_cyan("rule: E -> E+E\n"););
        return (Token_type)TOKEN_EXPRSN;
    case (char)TOKEN_EXPRSN << 16 | (char)TOKEN_MINUS << 8 | (char)TOKEN_EXPRSN:
        DEBUG_CODE(printf_cyan("rule: E -> E-E\n"););
        return (Token_type)TOKEN_EXPRSN;
    case (char)TOKEN_EXPRSN << 16 | (char)TOKEN_EQ << 8 | (char)TOKEN_EXPRSN:
        DEBUG_CODE(printf_cyan("rule: E -> E==E\n"););
        return (Token_type)TOKEN_EXPRSN;
    case (char)TOKEN_EXPRSN << 16 | (char)TOKEN_NEQ << 8 | (char)TOKEN_EXPRSN:
        DEBUG_CODE(printf_cyan("rule: E -> E!=E\n"););
        return (Token_type)TOKEN_EXPRSN;
    case (char)TOKEN_EXPRSN << 16 | (char)TOKEN_LESS << 8 | (char)TOKEN_EXPRSN:
        DEBUG_CODE(printf_cyan("rule: E -> E<E\n"););
        return (Token_type)TOKEN_EXPRSN;
    case (char)TOKEN_EXPRSN << 16 | (char)TOKEN_MORE << 8 | (char)TOKEN_EXPRSN:
        DEBUG_CODE(printf_cyan("rule: E -> E>E\n"););
        return (Token_type)TOKEN_EXPRSN;
    case (char)TOKEN_EXPRSN << 16 | (char)TOKEN_LESS_EQ << 8 | (char)TOKEN_EXPRSN:
        DEBUG_CODE(printf_cyan("rule: E -> E<=E\n"););
        return (Token_type)TOKEN_EXPRSN;
    case (char)TOKEN_EXPRSN << 16 | (char)TOKEN_MORE_EQ << 8 | (char)TOKEN_EXPRSN:
        DEBUG_CODE(printf_cyan("rule: E -> E>=E\n"););
        return (Token_type)TOKEN_EXPRSN;
    case (char)TOKEN_EXPRSN << 16 | (char)TOKEN_BINARY_OPERATOR << 8 | (char)TOKEN_EXPRSN:
        DEBUG_CODE(printf_cyan("rule: E -> E??E\n"););
        return (Token_type)TOKEN_EXPRSN;

    default:
        DEBUG_CODE(printf_red("rule: EOF\n"););
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

    // printf("b: {'%s', %d}\n", b.token_value, b.type);
    return b;
}

void printStackRec(StackNode *top)
{
    if (top == NULL)
    {
        return;
    }
    printStackRec(top->next);
    printf("%s", ((Token *)top->data)->token_value);
    // printf("{'%s', %d} ", ((Token *)top->data)->token_value, ((Token *)top->data)->type);
}

// recursively prints the stack
void printStack(StackNode *top)
{
    printStackRec(top);
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
    psa_stack_push(s, (Token){
                          .type = (Token_type)TOKEN_EOF,
                          .token_value = "$"});

    /*
        a - token on the top of the stack
        b - token on the input
        s - stack
    */

    Token *a = general_stack_top(s);
    Token b = readNextToken();

    while (!(a->type == (Token_type)TOKEN_EXPRSN && s->size == 2 && b.type == (Token_type)TOKEN_EOF))
    {
        // if the stack top is of type (Token_type)TOKEN_EXPRSN, then we need to use the second top of the stack to determine the rule
        if (a->type == (Token_type)TOKEN_EXPRSN)
        {
            a = (Token *)(s->top->next->data);
        }

        DEBUG_CODE(printf("na stacku: ");
                   printStack(s->top);
                   printf_yellow("na vstupu: {'%s', %d}\n", b.token_value, b.type);
                   printf_magenta("P_TABLE[{%d, '%s'}][{%d, '%s'}] = %c\n", getSymbolValue(a->type), a->token_value, (b.type), b.token_value, P_TABLE[getSymbolValue(a->type)][getSymbolValue(b.type)]););

        const unsigned int a_val = getSymbolValue(a->type);
        const unsigned int b_val = getSymbolValue(b.type);

        if (a_val == 99 || b_val == 99)
        {
            printf_red("❌ | Error: invalid token! Unexpected token '%s'. \n", b.token_value);

            return (psa_return_type){
                .return_type = TOKEN_EOF,
                .is_ok = false,
            };
        }

        switch (P_TABLE[a_val][b_val])
        {
        case '=':
            psa_stack_push(s, b);
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
            }
            else
            {
                psa_stack_push(s, (Token){
                                      .type = (Token_type)TOKEN_SHIFT,
                                      .token_value = "<"});
                psa_stack_push(s, b);
                b = readNextToken();
            }
            break;
        case '>':
        { // from the top of the stack, pop all tokens until the first < is found
            // put all the popped tokens->type into an array
            // getTheRule of the array
            // if the rule is not EOF, push the rule into the stack
            // else, return error
            Token_type *handle = malloc(sizeof(Token_type) * s->size);

            int i = 0;
            while (psa_stack_top(s).type != TOKEN_SHIFT)
            {
                handle[i] = ((Token)psa_stack_pop(s)).type;
                i++;
            }
            (void)psa_stack_pop(s); // pop the <

            // reverse handle
            uint32_t handle_val = reverseHandleToUInt32(handle, i);

            Token_type rule = getRule(handle_val);

            if (rule != TOKEN_EOF)
            {
                psa_stack_push(s, (Token){
                                      .type = rule,
                                      .token_value = "E"});
            }
            else
            {
                printf_red("❌ | Error: invalid expression! Unexpected token '%s' in expression. \n", b.token_value);

                return (psa_return_type){
                    .return_type = TOKEN_EOF,
                    .is_ok = false,
                };
            }

            break;
        }
        case '-':
        default:
            printf_red("❌ | Error: invalid combination of operands! '%s' and '%s' cannot be together, because it wasn't meant to be. \n", a->token_value, b.token_value);

            return (psa_return_type){
                .return_type = TOKEN_EOF,
                .is_ok = false,
            };
        }

        printStack(s->top);
        DEBUG_CODE(printf("\n-----------\n\n"););

        a = general_stack_top(s);
    }
    printf("\n");
    printf_green("✅ | All good! \n");

    return (psa_return_type){
        .return_type = TOKEN_EOF,
        .is_ok = true,
    };
}