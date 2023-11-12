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

uint32_t reverseHandleTypesToUInt32(Expression_type *types, unsigned int len)
{
    uint32_t result = 0;
    printf("\n{");
    for (int i = len - 1; i >= 0; i--)
    {
        result = result << 8 | (char)types[i];
        printf("%d, ", types[i]);
    }
    printf("}\n");
    printf("handleTypesToUInt32: %d\n", result);
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

    case TOKEN_STRING:
        return (Expression_type)TYPE_STRING;
    default:
        return (Expression_type)TYPE_INVALID;
    }
}

PSA_Token getRule(uint32_t handle_val)
{
    // TODO: predelat tyto odporne switch-case na neco lepsiho (pomoci enumu)
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
        return (PSA_Token){
            .type = (Token_type)TOKEN_EXPRSN,
            .token_value = "E",
            .expr_type = getTypeFromToken((Token_type)handle_val),
        };
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

PSA_Token readNextToken()
{
    int ch = getchar();
    ungetc(ch, stdin);
    if (ch == EOF)
    {
        return (PSA_Token){
            .type = (Token_type)TOKEN_EOF,
            .token_value = "$",
        };
    }

    Token *tkn = malloc(sizeof(Token));
    generate_token(tkn, "\0", false);
    PSA_Token b = {
        .type = tkn->type,
        .token_value = tkn->token_value,
        .expr_type = getTypeFromToken(tkn->type),
    };
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
    printf("%s", ((PSA_Token *)top->data)->token_value);
    // printf("{'%s', %d} ", ((PSA_Token *)top->data)->token_value, ((PSA_Token *)top->data)->type);
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
    psa_stack_push(s, (PSA_Token){
                          .type = (Token_type)TOKEN_EOF,
                          .token_value = "$"});

    /*
        a - token on the top of the stack
        b - token on the input
        s - stack
    */

    PSA_Token *a = general_stack_top(s);
    PSA_Token b = readNextToken();

    while (!(a->type == (Token_type)TOKEN_EXPRSN && s->size == 2 && b.type == (Token_type)TOKEN_EOF))
    {
        // if the stack top is of type (Token_type)TOKEN_EXPRSN, then we need to use the second top of the stack to determine the rule
        if (a->type == (Token_type)TOKEN_EXPRSN)
        {
            a = (PSA_Token *)(s->top->next->data);
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
                PSA_Token tmp = psa_stack_pop(s);
                psa_stack_push(s, (PSA_Token){
                                      .type = (Token_type)TOKEN_SHIFT,
                                      .token_value = "<"});
                psa_stack_push(s, tmp);
                psa_stack_push(s, b);
                b = readNextToken();
            }
            else
            {
                psa_stack_push(s, (PSA_Token){
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
                handle[i] = ((PSA_Token)psa_stack_pop(s)).type;
                i++;
            }
            (void)psa_stack_pop(s); // pop the <

            // reverse handle
            uint32_t handle_val = reverseHandleToUInt32(handle, i);

            Token_type rule = getRule(handle_val);

            if (rule != TOKEN_EOF)
            {
                psa_stack_push(s, (PSA_Token){
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

// PSA STACK

void psa_stack_push(Stack *s, PSA_Token data)
{
    PSA_Token *new_node = malloc(sizeof(PSA_Token));
    *new_node = data;

    general_stack_push(s, new_node);
}

PSA_Token psa_stack_pop(Stack *s)
{
    PSA_Token *popped = general_stack_pop(s);
    PSA_Token ret = *popped;
    free(popped);
    return ret;
}

PSA_Token psa_stack_top(Stack *s)
{
    PSA_Token *top = general_stack_top(s);
    return *top;
}