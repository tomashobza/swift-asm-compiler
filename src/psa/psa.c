#include "psa.h"
DEFINE_STACK_FUNCTIONS(PSA_Token)

psa_return_type parse_expression_base(bool is_param, symtable_stack *st_stack)
{
    int num_of_brackets = 0; // number of brackets in the expression

    PSA_Token_stack *s = PSA_Token_stack_init();
    PSA_Token_stack_push(s, (PSA_Token){
                                .type = (Token_type)TOKEN_EOF,
                                .token_value = "$"});

    /*
        a - token on the top of the stack
        b - token on the input
        s - stack
    */

    PSA_Token a = PSA_Token_stack_top(s);

    char next_token_error = 0;
    PSA_Token b = readNextToken(s, &next_token_error);

    // check for an empty expression
    if (a.type == TOKEN_EOF && b.type == TOKEN_EOF)
    {
        printf("Empty expression!\n");
        return (psa_return_type){
            .end_token = TOKEN_EXPRSN,
            .is_ok = true,
            .canBeNil = false,
            .type = TYPE_EMPTY,
        };
    }

    while (!(a.type == (Token_type)TOKEN_EXPRSN && s->size == 2 && b.type == (Token_type)TOKEN_EOF))
    {
        // if the stack top is of type (Token_type)TOKEN_EXPRSN, then we need to use the second top of the stack to determine the rule
        if (a.type == (Token_type)TOKEN_EXPRSN)
        {
            a = s->top->next->data;
        }

        DEBUG_CODE(printf("Chyba: %d\n", next_token_error););

        // update the bracket counter
        switch (b.type)
        {
        case TOKEN_L_BRACKET:
            num_of_brackets++;
            break;
        case TOKEN_R_BRACKET:
            num_of_brackets--;
            break;
        default:
            break;
        }

        // check for an invalid number of brackets (for parameters, the number of brackets can be -1 when the expression is empty and the last of the parameters)
        if (num_of_brackets < 0 && !(is_param && num_of_brackets == -1))
        {
            printf_red("PICO VOLE SPATNE ZAVORKY");
        }

        // if the next token is a function identificator, start parsing function call
        if (b.type == TOKEN_FUNC_ID)
        {
            printf_magenta("--------Je to funkce! --------\n");
            print_token_type(b.type);

            b = parseFunctionCall(s, b, st_stack);

            printf_magenta("------------------------------\n");

            // read the next token
            // b = readNextToken(s, &next_token_error);
        }
        else
        {
            if (b.preceded_by_nl && next_token_error > 0)
            {
                next_token_error = 0;
                b = (PSA_Token){
                    .type = (Token_type)TOKEN_EOF,
                    .token_value = "$",
                    .expr_type = TYPE_INVALID,
                    .canBeNil = false,
                    .preceded_by_nl = true,
                };
            }
        }

        // if expression is a function parameter, the end of the expression is ) or ,
        if (is_param)
        {
            printf("is in a function\n");

            switch (b.type)
            {
            case TOKEN_R_BRACKET:
                if (num_of_brackets < 0)
                {
                    return_token(convertPSATokenToToken(b));
                }
                __attribute__((fallthrough));
            case TOKEN_COMMA:
                printf("end of parameter\n");
                b = (PSA_Token){
                    .type = TOKEN_EOF,
                    .token_value = "$",
                    .expr_type = TYPE_INVALID,
                    .canBeNil = false,
                    .preceded_by_nl = true,
                };
                break;
            default:
                break;
            }
        }

        // // DETECT EMPTY EXPRESSION
        // if (a.type == (Token_type)TOKEN_EOF && !canTokenBeStartOfExpression(b.type))
        // {
        //     // empty expression followed by EOL is valid
        //     if (b.preceded_by_nl)
        //     {
        //         return (psa_return_type){
        //             .end_token = TOKEN_EXPRSN,
        //             .is_ok = true,
        //             .canBeNil = false,
        //             .type = TYPE_EMPTY,
        //         };
        //     }
        //     else
        //     { // empty expression followed by something else is invalid
        //         printf_red("❌ | Error: invalid token! Token '%s' cannot be a part of an expression, add a newline as a separator. \n", b.token_value);

        //         return (psa_return_type){
        //             .end_token = TOKEN_EOF,
        //             .is_ok = false,
        //             .canBeNil = false,
        //             .type = TYPE_INVALID,
        //         };
        //     }
        // }

        DEBUG_CODE(printf("na stacku: ");
                   printStack(s->top);
                   printf_yellow("na vstupu: {'%s', %d}\n", b.token_value, b.type);
                   printf_magenta("P_TABLE[{%d, '%s'}][{%d, '%s'}] = %c\n", getSymbolValue(a.type), a.token_value, (b.type), b.token_value, P_TABLE[getSymbolValue(a.type)][getSymbolValue(b.type)]););

        const unsigned int a_val = getSymbolValue(a.type);
        const unsigned int b_val = getSymbolValue(b.type);

        if (a_val == 99 || b_val == 99)
        {
            printf_red("❌ | Error: invalid token! Unexpected token '%s'. \n", b.token_value);

            return (psa_return_type){
                .end_token = TOKEN_EXPRSN,
                .is_ok = false,
                .canBeNil = false,
                .type = TYPE_INVALID,
            };
        }

        switch (P_TABLE[a_val][b_val])
        {
        case '=':
            PSA_Token_stack_push(s, b);
            b = readNextToken(s, &next_token_error);
            break;
        case '<':

            if (PSA_Token_stack_top(s).type == (Token_type)TOKEN_EXPRSN)
            {
                PSA_Token tmp = PSA_Token_stack_pop(s);
                PSA_Token_stack_push(s, (PSA_Token){
                                            .type = (Token_type)TOKEN_SHIFT,
                                            .token_value = "<"});
                PSA_Token_stack_push(s, tmp);
                PSA_Token_stack_push(s, b);
                b = readNextToken(s, &next_token_error);
            }
            else
            {
                PSA_Token_stack_push(s, (PSA_Token){
                                            .type = (Token_type)TOKEN_SHIFT,
                                            .token_value = "<"});
                PSA_Token_stack_push(s, b);
                b = readNextToken(s, &next_token_error);
            }
            break;
        case '>':
        { // from the top of the stack, pop all tokens until the first < is found
            // put all the popped tokens->type into an array
            // getTheRule of the array
            // if the rule is not EOF, push the rule into the stack
            // else, return error
            PSA_Token *handle = malloc(sizeof(PSA_Token) * s->size);

            int i = 0;
            while (PSA_Token_stack_top(s).type != TOKEN_SHIFT)
            {
                handle[i] = ((PSA_Token)PSA_Token_stack_pop(s));
                i++;
            }
            (void)PSA_Token_stack_pop(s); // pop the <

            // reverse the array
            for (int j = 0; j < i / 2; j++)
            {
                PSA_Token tmp = handle[j];
                handle[j] = handle[i - j - 1];
                handle[i - j - 1] = tmp;
            }

            PSA_Token rule = getRule(handle, i);

            if (rule.type != TOKEN_EOF)
            {
                PSA_Token_stack_push(s, rule);
            }
            else
            {
                printTokenArray(handle, i);
                printf_red("❌ | Error: invalid expression! Unexpected token '%s' in expression. \n", b.token_value);

                return (psa_return_type){
                    .end_token = TOKEN_EXPRSN,
                    .is_ok = false,
                    .canBeNil = false,
                    .type = TYPE_INVALID,
                };
            }

            break;
        }
        case '-':
        default:
            printf_red("❌ | Error: invalid combination of operands! '%s' and '%s' cannot be together, because it wasn't meant to be. \n", a.token_value, b.token_value);

            return (psa_return_type){
                .end_token = TOKEN_EOF,
                .is_ok = false,
                .canBeNil = false,
                .type = TYPE_INVALID,
            };
        }

        printStack(s->top);
        DEBUG_CODE(printf("\n-----------\n\n"););

        a = PSA_Token_stack_top(s);
    }
    printf("\n");
    printf_green("✅ | All good! \n");

    return (psa_return_type){
        .is_ok = a.expr_type != TYPE_INVALID,
        .type = a.expr_type,
        .end_token = a.type,
        .canBeNil = a.canBeNil,
    };
}

psa_return_type parse_expression(symtable_stack *st_stack)
{
    return parse_expression_base(false, st_stack);
}

psa_return_type parse_expression_param(symtable_stack *st_stack)
{
    return parse_expression_base(true, st_stack);
}
