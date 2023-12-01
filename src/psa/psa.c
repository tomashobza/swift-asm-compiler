/**
 * @file psa.c
 * @author Tomáš Hobza (xhobza03@vutbr.cz), Anastasia Butok (xbutok00@vutbr.cz)
 * @brief Main function of the PSA module.
 * @version 0.1
 * @date 2023-11-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "psa.h"
DEFINE_STACK_FUNCTIONS(PSA_Token)

psa_return_type parse_expression_base(bool is_param)
{

    // TODO: add checking if variable is initialized before use
    // TODO: add functions for expressions in parameters
    int num_of_brackets = 0; // number of brackets in the expression

    PSA_Token_stack *s = PSA_Token_stack_init();
    if (s == NULL)
    {
        throw_error(INTERNAL_ERR, -1, "PSA stack initialization failed.");
    }
    PSA_Token_stack_push(s, PSA_TOKEN_EOF);

    /*
        a - token on the top of the stack
        b - token on the input
        s - stack
    */

    PSA_Token a = PSA_Token_stack_top(s);

    char next_token_error = 0;
    PSA_Token b = readNextToken(s, &next_token_error, &num_of_brackets);

    while (!(a.type == (Token_type)TOKEN_EXPRSN && s->size == 2 && (b.type == (Token_type)TOKEN_EOF)))
    {
        // if the stack top is of type (Token_type)TOKEN_EXPRSN, then we need to use the second top of the stack to determine the rule
        if (a.type == (Token_type)TOKEN_EXPRSN)
        {
            a = s->top->next->data;
        }

        // DEBUG_PSA_CODE(printf("Chyba: %d\n", next_token_error););

        // FOR PARAMETER EXPRESSIONS CHECK FOR END OF PARAMETER
        // if expression is a function parameter, the end of the expression is ) or ,
        if (is_param)
        {
            // printf("is in a function\n");

            switch (b.type)
            {
            case TOKEN_R_BRACKET:
                if (num_of_brackets >= 0)
                {
                    break;
                }

                return_token(convertPSATokenToToken(b));

                __attribute__((fallthrough));
            case TOKEN_COMMA:
                next_token_error = 0;
                b = PSA_TOKEN_EOF;
                break;
            default:
                break;
            }
        }

        // CHECK NEXT TOKEN FOR ERRORS
        if (next_token_error > 0 && b.type != TOKEN_EOF)
        {
            return_token(convertPSATokenToToken(b));
            if (b.preceded_by_nl || b.type == TOKEN_L_CURLY)
            {
                next_token_error = 0;
                b = PSA_TOKEN_EOF;
            }
            else
            {
                throw_error(SYNTACTIC_ERR, b.line_num, "Missing separator (EOL) after expression, before '%s'.", b.token_value);

                next_token_error = 0;
                b = PSA_TOKEN_EOF;
            }
        }

        // CHECK FOR FUNCTION CALL
        // if the next token is a function identificator, start parsing function call
        if (b.type == TOKEN_FUNC_ID)
        {
            DEBUG_PSA_CODE(
                printf_magenta("--------Je to funkce! --------\n");
                print_token_type(b.type););

            b = parseFunctionCall(s, b);
            PSA_Token_stack_push(s, b);
            a = PSA_Token_stack_top(s);

            b = readNextToken(s, &next_token_error, &num_of_brackets);

            DEBUG_PSA_CODE(printf_magenta("------------------------------\n"););

            if (is_param)
            {
                switch (b.type)
                {
                case TOKEN_R_BRACKET:
                    if (num_of_brackets >= 0)
                    {
                        break;
                    }

                    return_token(convertPSATokenToToken(b));

                    __attribute__((fallthrough));
                case TOKEN_COMMA:
                    next_token_error = 0;
                    b = PSA_TOKEN_EOF;
                    break;
                default:
                    break;
                }
            }
            // CHECK NEXT TOKEN FOR ERRORS
            if (next_token_error > 0 && b.type != TOKEN_EOF)
            {
                return_token(convertPSATokenToToken(b));
                if (b.preceded_by_nl)
                {
                    next_token_error = 0;
                    b = PSA_TOKEN_EOF;
                }
                else
                {
                    throw_error(SYNTACTIC_ERR, b.line_num, "Missing separator (EOL) after expression, before '%s'.", b.token_value);

                    next_token_error = 0;
                    b = PSA_TOKEN_EOF;
                }
            }

            continue;

            // read the next token
            // b = readNextToken(s, &next_token_error, num_of_brackets);
        }

        // check for an empty expression
        if (a.type == TOKEN_EOF && b.type == TOKEN_EOF)
        {
            PSA_Token_stack_free(s);
            // printf("Empty expression!\n");
            return (psa_return_type){
                .end_token = TOKEN_EXPRSN,
                .is_ok = true,
                .type = TYPE_EMPTY,
                .is_literal = false,
            };
        }

        DEBUG_PSA_CODE(printf_blue("Bracket count: %d\n", num_of_brackets););
        DEBUG_PSA_CODE(printf("na stacku: ");
                       printStack(s->top);
                       printf_yellow("na vstupu: {'%s', %d}\n", b.token_value, b.type);
                       printf_magenta("P_TABLE[{%d, '%s'}][{%d, '%s'}] = %c\n", getSymbolValue(a.type), a.token_value, (b.type), b.token_value, P_TABLE[getSymbolValue(a.type)][getSymbolValue(b.type)]););

        const unsigned int a_val = getSymbolValue(a.type);
        const unsigned int b_val = getSymbolValue(b.type);

        // choose further behaviour based on the value in the precedence table
        switch (P_TABLE[a_val][b_val])
        {
        case '=': // just push the token on the stack
            PSA_Token_stack_push(s, b);
            b = readNextToken(s, &next_token_error, &num_of_brackets);
            break;

        case '<': // opening of a handle
            if (PSA_Token_stack_top(s).type == (Token_type)TOKEN_EXPRSN)
            {
                PSA_Token tmp = PSA_Token_stack_pop(s);
                PSA_Token_stack_push(s, (PSA_Token){
                                            .type = (Token_type)TOKEN_SHIFT,
                                            .token_value = "<"});
                PSA_Token_stack_push(s, tmp);
                PSA_Token_stack_push(s, b);
                b = readNextToken(s, &next_token_error, &num_of_brackets);
            }
            else
            {
                PSA_Token_stack_push(s, (PSA_Token){
                                            .type = (Token_type)TOKEN_SHIFT,
                                            .token_value = "<"});
                PSA_Token_stack_push(s, b);
                b = readNextToken(s, &next_token_error, &num_of_brackets);
            }
            break;
        case '>':
        {
            int handle_len = 0;
            PSA_Token *handle = getHandleFromStack(s, &handle_len);
            int handle_line_num = handle_len > 0 ? handle[0].line_num : b.line_num;

            PSA_Token rule = getRule(handle, handle_len);
            rule.line_num = handle_line_num;
            DEBUG_PSA_CODE(printf_cyan("rule type: ");
                           print_expression_type(rule.expr_type);
                           printf("\n"););

            if (rule.type != TOKEN_EOF)
            {
                PSA_Token_stack_push(s, rule);
            }
            else
            {
                DEBUG_PSA_CODE(printTokenArray(handle, handle_len););

                throw_error(SYNTACTIC_ERR, b.line_num, "Unexpected token '%s' in expression.", b.token_value);

                PSA_Token_stack_free(s);
                free(handle);
                return (psa_return_type){
                    .end_token = TOKEN_EXPRSN,
                    .is_ok = false,
                    .type = TYPE_INVALID,
                    .is_literal = false,
                };
            }
            free(handle);

            break;
        }
        case '-':
        default:
            throw_error(SYNTACTIC_ERR, b.line_num, "Invalid combination of operands '%s' and '%s'.", a.token_value, b.token_value);

            PSA_Token_stack_free(s);
            return (psa_return_type){
                .end_token = TOKEN_EOF,
                .is_ok = false,
                .type = TYPE_INVALID,
                .is_literal = false,
            };
        }

        DEBUG_PSA_CODE(printStack(s->top);
                       printf("\n-----------\n\n"););

        a = PSA_Token_stack_top(s);
    }
    printf("\n");
    printf_green("PSA: ✅ | All good! \n");

    // TODO: checking bracket count might be redundant because of handles
    if (num_of_brackets != 0 && !(is_param && num_of_brackets == -1))
    {
        throw_error(SYNTACTIC_ERR, a.line_num, num_of_brackets > 0 ? "Missing closing bracket" : "Missing opening bracket");

        next_token_error = 0;
        b = PSA_TOKEN_EOF;
    }

    DEBUG_PSA_CODE(
        printf("Expression type: ");
        print_expression_type(a.expr_type);
        printf("Is expression a literal: ");
        printf_cyan("%s\n", a.is_literal ? "true" : "false"););

    PSA_Token_stack_free(s);
    return (psa_return_type){
        .is_ok = a.expr_type != TYPE_INVALID,
        .type = a.expr_type,
        .end_token = a.type,
        .is_literal = a.is_literal,
    };
}

// UTILITY WRAPPER FUNCTIONS

psa_return_type parse_expression()
{
    return parse_expression_base(false);
}

psa_return_type parse_expression_param()
{
    return parse_expression_base(true);
}
