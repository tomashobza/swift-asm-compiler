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
    case TOKEN_EOL:
        return 9;
    case TOKEN_EOF:
        return 8;
    default: // error
        return 99;
    }
}

uint32_t handleToUInt32(PSA_Token *handle, unsigned int len)
{
    uint32_t result = 0;
    printf("\n{");
    for (int i = 0; i < len; i++)
    {
        result = result << 8 | (char)(handle[i].type);
        printf("%d, ", (handle[i].type));
    }
    printf("}\n");
    printf("handleToUInt32: %d\n", result);
    return result;
}

uint32_t reverseHandleToUInt32(PSA_Token *handle, unsigned int len)
{
    uint32_t result = 0;
    printf("\n{");
    for (int i = len - 1; i >= 0; i--)
    {
        result = result << 8 | (char)(handle[i].type);
        printf("%d, ", (handle[i].type));
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
    case TOKEN_IDENTIFICATOR: // TODO: delete and for id use symbol table
        return (Expression_type)TYPE_DOUBLE;

    case TOKEN_STRING:
        return (Expression_type)TYPE_STRING;
    default:
        return (Expression_type)TYPE_INVALID;
    }
}

/**
 * @brief Returns the type of the expression based on the types of the operands.

    Supports: (int, int), (int, double), (double, int), (double, double)
 *
 * @param l_operand
 * @param r_operand
 * @return Expression_type
 */
Expression_type getTypeCombination(PSA_Token l_operand, PSA_Token r_operand)
{
    switch (((char)l_operand.expr_type << 8) | r_operand.expr_type)
    {
    case ((char)TYPE_INT << 8) | TYPE_INT:
        return TYPE_INT;
    case ((char)TYPE_INT << 8) | TYPE_DOUBLE:
        if (l_operand.type != TOKEN_IDENTIFICATOR)
        {
            DEBUG_CODE(printf("implicite Int2Double for left operand '%s'\n", l_operand.token_value););
            return TYPE_DOUBLE;
        }
        else
        {
            return TYPE_INVALID;
        }
    case ((char)TYPE_DOUBLE << 8) | TYPE_INT:
        if (r_operand.type != TOKEN_IDENTIFICATOR)
        {
            DEBUG_CODE(printf("impicite Int2Double for right operand '%s'\n", r_operand.token_value););
            return TYPE_DOUBLE;
        }
        else
        {
            return TYPE_INVALID;
        }
    case ((char)TYPE_DOUBLE << 8) | TYPE_DOUBLE:
        return TYPE_DOUBLE;
    default:
        return TYPE_INVALID;
    }
}

/**
 * @brief Returns the type of the expression based on the types of the operands and the operation.
 *
 * @param l_operand
 * @param operation
 * @param r_operand
 * @return Expression_type
 */
PSA_Token getHandleType(PSA_Token l_operand, Token_type operation, PSA_Token r_operand)
{
    if (l_operand.expr_type == TYPE_INVALID || r_operand.expr_type == TYPE_INVALID)
    {
        return (PSA_Token){
            .type = (Token_type)TOKEN_EXPRSN,
            .token_value = "E",
            .expr_type = TYPE_INVALID,
            .canBeNil = false,
        };
    }

    switch (operation)
    {
    // for: +, -, *, /
    case TOKEN_PLUS:
        // can be (string, string), ...
        if (l_operand.expr_type == TYPE_STRING && r_operand.expr_type == TYPE_STRING && !(l_operand.canBeNil || r_operand.canBeNil))
        {
            return (PSA_Token){
                .type = (Token_type)TOKEN_EXPRSN,
                .token_value = "E",
                .expr_type = TYPE_STRING,
                .canBeNil = true,
            };
        }
    case TOKEN_MINUS:
    case TOKEN_MUL:
    case TOKEN_DIV:
        if (l_operand.canBeNil || r_operand.canBeNil)
        {
            return (PSA_Token){
                .type = (Token_type)TOKEN_EXPRSN,
                .token_value = "E",
                .expr_type = TYPE_INVALID,
                .canBeNil = true,
            };
        }

        // can be (int, int), (int, double), (double, int), (double, double)
        Expression_type type = getTypeCombination(l_operand, r_operand);

        if (type != TYPE_INVALID)
        {
            return (PSA_Token){
                .type = (Token_type)TOKEN_EXPRSN,
                .token_value = "E",
                .expr_type = type,
                .canBeNil = true,
            };
        }

        break;

    // for: ==, !=
    case TOKEN_EQ:
    case TOKEN_NEQ:

        // can be (int, int), (int, double), (double, int), (double, double), (string, string), (bool, bool)
        if (l_operand.expr_type == r_operand.expr_type || getTypeCombination(l_operand, r_operand) != TYPE_INVALID)
        {
            return (PSA_Token){
                .type = (Token_type)TOKEN_EXPRSN,
                .token_value = "E",
                .expr_type = TYPE_BOOL,
                .canBeNil = false,
            };
        }
        break;
    // for: <, >, <=, >=
    case TOKEN_LESS:
    case TOKEN_MORE:
    case TOKEN_LESS_EQ:
    case TOKEN_MORE_EQ:
        // can be (int, int), (int, double), (double, int), (double, double), (string, string)
        if (getTypeCombination(l_operand, r_operand) != TYPE_INVALID || l_operand.expr_type == TYPE_STRING && r_operand.expr_type == TYPE_STRING)
        {
            return (PSA_Token){
                .type = (Token_type)TOKEN_EXPRSN,
                .token_value = "E",
                .expr_type = TYPE_BOOL,
                .canBeNil = false,
            };
        }
        break;
    // for: ??, &&, ||
    case TOKEN_AND:
    case TOKEN_OR:
        if (l_operand.expr_type == TYPE_BOOL && r_operand.expr_type == TYPE_BOOL)
        {
            return (PSA_Token){
                .type = (Token_type)TOKEN_EXPRSN,
                .token_value = "E",
                .expr_type = TYPE_BOOL,
                .canBeNil = false,
            };
        }
        break;
    // for: !
    case TOKEN_BINARY_OPERATOR:
        if (l_operand.canBeNil && !r_operand.canBeNil)
        {
            return (PSA_Token){
                .type = (Token_type)TOKEN_EXPRSN,
                .token_value = "E",
                .expr_type = getTypeCombination(l_operand, r_operand),
                .canBeNil = false,
            };
        }
        break;
    default:
        break;
    }

    return (PSA_Token){
        .type = (Token_type)TOKEN_EXPRSN,
        .token_value = "E",
        .expr_type = TYPE_INVALID,
        .canBeNil = false,
    };
}

PSA_Token getRule(PSA_Token *handle, unsigned int len)
{
    uint32_t handle_val = handleToUInt32(handle, len);
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
    case RULE_1a:
    case RULE_1b:
    case RULE_1c:
    case RULE_1d:
    case RULE_1e:
        DEBUG_CODE(printf_cyan("rule: E -> i\n"););
        return (PSA_Token){
            .type = (Token_type)TOKEN_EXPRSN,
            .token_value = "E",
            .expr_type = getTypeFromToken(handle[0].type),
        };
    case RULE_2:
        DEBUG_CODE(printf_cyan("rule: E -> (E)\n"););
        return (PSA_Token){
            .type = (Token_type)TOKEN_EXPRSN,
            .token_value = "E",
            .expr_type = getTypeFromToken(handle[1].type),
        };
    case RULE_3:
        DEBUG_CODE(printf_cyan("rule: E -> !E\n"););
        return (PSA_Token){
            .type = (Token_type)TOKEN_EXPRSN,
            .token_value = "E",
            .expr_type = getTypeFromToken(handle[2].type),
        };
    case RULE_4:
        DEBUG_CODE(printf_cyan("rule: E -> +E\n"););
        return (PSA_Token){
            .type = (Token_type)TOKEN_EXPRSN,
            .token_value = "E",

        };
    case RULE_5:
        DEBUG_CODE(printf_cyan("rule: E -> -E\n"););
        return (PSA_Token){
            .type = (Token_type)TOKEN_EXPRSN,
            .token_value = "E",

        };
    case RULE_6:
        DEBUG_CODE(printf_cyan("rule: E -> E*E\n"););
        return (PSA_Token){
            .type = (Token_type)TOKEN_EXPRSN,
            .token_value = "E",
        };
    case RULE_7:
        DEBUG_CODE(printf_cyan("rule: E -> E/E\n"););
        return (PSA_Token){
            .type = (Token_type)TOKEN_EXPRSN,
            .token_value = "E",
        };
    case RULE_8:
        DEBUG_CODE(printf_cyan("rule: E -> E+E\n"););
        return (PSA_Token){
            .type = (Token_type)TOKEN_EXPRSN,
            .token_value = "E",
        };
    case RULE_9:
        DEBUG_CODE(printf_cyan("rule: E -> E-E\n"););
        return (PSA_Token){
            .type = (Token_type)TOKEN_EXPRSN,
            .token_value = "E",
        };
    case RULE_10:
        DEBUG_CODE(printf_cyan("rule: E -> E==E\n"););
        return (PSA_Token){
            .type = (Token_type)TOKEN_EXPRSN,
            .token_value = "E",
        };
    case RULE_11:
        DEBUG_CODE(printf_cyan("rule: E -> E!=E\n"););
        return (PSA_Token){
            .type = (Token_type)TOKEN_EXPRSN,
            .token_value = "E",
        };
    case RULE_12:
        DEBUG_CODE(printf_cyan("rule: E -> E<E\n"););
        return (PSA_Token){
            .type = (Token_type)TOKEN_EXPRSN,
            .token_value = "E",
        };
    case RULE_13:
        DEBUG_CODE(printf_cyan("rule: E -> E>E\n"););
        return (PSA_Token){
            .type = (Token_type)TOKEN_EXPRSN,
            .token_value = "E",
        };
    case RULE_14:
        DEBUG_CODE(printf_cyan("rule: E -> E<=E\n"););
        return (PSA_Token){
            .type = (Token_type)TOKEN_EXPRSN,
            .token_value = "E",
        };
    case RULE_15:
        DEBUG_CODE(printf_cyan("rule: E -> E>=E\n"););
        return (PSA_Token){
            .type = (Token_type)TOKEN_EXPRSN,
            .token_value = "E",
        };
    case RULE_16:
        DEBUG_CODE(printf_cyan("rule: E -> E&&E\n"););
        return (PSA_Token){
            .type = (Token_type)TOKEN_EXPRSN,
            .token_value = "E",
        };
    case RULE_17:
        DEBUG_CODE(printf_cyan("rule: E -> E||E\n"););
        return (PSA_Token){
            .type = (Token_type)TOKEN_EXPRSN,
            .token_value = "E",
        };
    case RULE_18:
        DEBUG_CODE(printf_cyan("rule: E -> E??E\n"););
        return (PSA_Token){
            .type = (Token_type)TOKEN_EXPRSN,
            .token_value = "E",
        };
    default:
        DEBUG_CODE(printf_red("rule: EOF\n"););
        return (PSA_Token){
            .type = (Token_type)TOKEN_EOF,
            .token_value = "$",
        };
    }

    return (PSA_Token){
        .type = (Token_type)TOKEN_EOF,
        .token_value = "$",
    };
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
            PSA_Token *handle = malloc(sizeof(Token_type) * s->size);

            int i = 0;
            while (psa_stack_top(s).type != TOKEN_SHIFT)
            {
                handle[i] = ((PSA_Token)psa_stack_pop(s));
                i++;
            }
            (void)psa_stack_pop(s); // pop the <

            PSA_Token rule = getRule(handle, i);

            if (rule.type != TOKEN_EOF)
            {
                psa_stack_push(s, rule);
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