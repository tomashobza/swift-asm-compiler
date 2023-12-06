/**
 * @file psa.c
 * @author Tomáš Hobza (xhobza03@vutbr.cz), Anastasia Butok (xbutok00@vutbr.cz)
 * @brief Main function of the PSA module.
 * @version 0.1
 * @date 2023-11-24
 *
 * @copyright Copyright (c) 2023
 * Project: IFJ compiler
 *
 */

#include "psa.h"

DEFINE_STACK_FUNCTIONS(PSA_Token)

psa_return_type parse_expression_base(bool is_param)
{

    int num_of_brackets = 0; // number of brackets in the expression

    PSA_Token_stack *s = PSA_Token_stack_init();
    if (s == NULL)
    {
        throw_error(INTERNAL_ERR, -1, "PSA stack initialization failed.");
    }
    PSA_Token_stack_push(s, PSA_TOKEN_EOF); // initialize the stack with the EOF token

    /*
        a - token on the top of the stack
        b - token on the input
        s - stack
    */

    PSA_Token a = PSA_Token_stack_top(s);

    char next_token_error = 0;
    PSA_Token b = readNextToken(s, &next_token_error, &num_of_brackets, false);

    // the PSA main loop - while the stack top is not EOF and the input is not EOF
    while (!(a.type == (Token_type)TOKEN_EXPRSN && s->size == 2 && (b.type == (Token_type)TOKEN_EOF)))
    {
        /*
            NOTE: To end the expression, the EOF token is pushed to the stack. This is done multiple times throughout the code to ensure the anonymity of the PSA.
        */

        // if the stack top is of type (Token_type)TOKEN_EXPRSN, then we need to use the second top of the stack to determine the rule
        if (a.type == (Token_type)TOKEN_EXPRSN)
        {
            a = s->top->next->data;
        }

        // FOR PARAMETER EXPRESSIONS CHECK FOR END OF PARAMETER
        // if expression is a function parameter, the end of the expression is ) or ,
        if (is_param)
        {
            switch (b.type)
            {
            case TOKEN_R_BRACKET: // )
                if (num_of_brackets >= 0)
                {
                    break;
                }

                return_token(convertPSATokenToToken(b)); // mark the intentional case fallthrough

                __attribute__((fallthrough));
            case TOKEN_COMMA: // ,
                next_token_error = 0;
                b = PSA_TOKEN_EOF;
                break;
            default:
                break;
            }
        }

        // CHECK NEXT TOKEN FOR ERRORS
        // NOTE: EOF next token errors shall be ignored as they are handled by the parser
        if (next_token_error > 0 && b.type != TOKEN_EOF)
        {
            return_token(convertPSATokenToToken(b)); // an invalid token was read, return it to the scanner for the parser to handle it

            // check for cases where the next token just needs to be ignored and the expression ended
            if (b.preceded_by_nl || b.type == TOKEN_L_CURLY || b.type == TOKEN_R_CURLY)
            {
                next_token_error = 0;
                b = PSA_TOKEN_EOF;
            }
            // in other cases, throw an error
            else
            {
                throw_error(SYNTACTIC_ERR, b.line_num, "Missing separator (EOL) after expression, before '%s'.", b.token_value);

                next_token_error = 0;
                b = PSA_TOKEN_EOF;
            }
        }

        // check for an empty expression
        if (a.type == TOKEN_EOF && b.type == TOKEN_EOF)
        {
            // empty expression may be valid in some cases, so we need to note it and return it to the parser
            PSA_Token_stack_free(s);
            return (psa_return_type){
                .end_token = TOKEN_EXPRSN,
                .is_ok = true,
                .type = TYPE_EMPTY, // empty expression
                .is_literal = false,
            };
        }

        DEBUG_PSA_CODE(printf_blue("Bracket count: %d\n", num_of_brackets););
        DEBUG_PSA_CODE(printf("na stacku: ");
                       printStack(s->top);
                       printf_yellow("na vstupu: {'%s', %d}\n", b.token_value, b.type);
                       printf_magenta("P_TABLE[{%d, '%s'}][{%d, '%s'}] = %c\n", getSymbolValue(a.type), a.token_value, (b.type), b.token_value, P_TABLE[getSymbolValue(a.type)][getSymbolValue(b.type)]););

        // get the value of the tokens from the precedence table
        const unsigned int a_val = getSymbolValue(a.type);
        const unsigned int b_val = getSymbolValue(b.type);

        // choosing further behaviour based on the value in the precedence table is necessary for the '!' token due to the duality of it's meaning (not and forced unwrapping)
        char ptable_val = P_TABLE[a_val][b_val];
        if (s->size >= 2 && s->top->next->data.type == TOKEN_EXPRSN && a.type == TOKEN_NOT)
        {
            // if the top of the stack is an expression and the token on the top of the stack is '!', then the '!' token is forced unwrapping
            ptable_val = '>';
        }

        switch (ptable_val)
        {
        case '=': // just push the token on the stack
            PSA_Token_stack_push(s, b);
            b = readNextToken(s, &next_token_error, &num_of_brackets, false);
            break;

        case '<': // opening of a handle

            // the '<' token shall fall through if an EXPRESSION token is on the top of the stack
            if (PSA_Token_stack_top(s).type == (Token_type)TOKEN_EXPRSN)
            {
                PSA_Token tmp = PSA_Token_stack_pop(s);
                PSA_Token_stack_push(s, (PSA_Token){
                                            .type = (Token_type)TOKEN_SHIFT,
                                            .token_value = "<"});
                PSA_Token_stack_push(s, tmp);
                PSA_Token_stack_push(s, b);
                b = readNextToken(s, &next_token_error, &num_of_brackets, false);
            }
            else
            {
                PSA_Token_stack_push(s, (PSA_Token){
                                            .type = (Token_type)TOKEN_SHIFT,
                                            .token_value = "<"});
                PSA_Token_stack_push(s, b);
                b = readNextToken(s, &next_token_error, &num_of_brackets, false);
            }
            break;
        case '>': // closing of a handle
        {
            PSA_Token top = a; // the top of the stack - saving it for later (generator)

            // get the handle (array of tokens) from the stack
            int handle_len = 0;
            PSA_Token *handle = getHandleFromStack(s, &handle_len);
            int handle_line_num = handle_len > 0 ? handle[0].line_num : b.line_num;

            // get the derivated token from the handle
            PSA_Token rule = getRule(handle, handle_len);
            rule.line_num = handle_line_num;
            DEBUG_PSA_CODE(printf_cyan("rule type: ");
                           print_expression_type(rule.expr_type);
                           printf("\n"););

            // check if the derivation was successful
            bool derivation_ok = rule.expr_type != TYPE_INVALID && rule.type != TOKEN_EOF;

            /// CODE GENERATION

            if (derivation_ok && top.type == TOKEN_BINARY_OPERATOR)
            {
                // binary operator is not represented by one or more stack instructions
                generate_nil_coelacing();
            }
            else if (derivation_ok && isTokenBinaryOperator(top.type))
            {
                // binary operators are (in most cases) represented by one or more stack instructions
                switch (top.type)
                {
                case TOKEN_PLUS: // + is a special case, as it can be used for both string (concatenation) and numeric addition
                    if (rule.expr_type == TYPE_STRING)
                    {
                        generate_string_concat();
                    }
                    else
                    {
                        generate_instruction(ADDS);
                    }
                    break;
                default:
                {
                    // get the stack instruction list for the given operation
                    Instruction_list inst_list = tokenTypeToStackInstruction(top.type);
                    for (int i = 0; i < inst_list.len; i++)
                    {
                        // an exception is made for the division of doubles and integers, as it is represented by two differenc stack instructions
                        Instruction inst = inst_list.inst[i];
                        if (inst == IDIVS && rule.expr_type == TYPE_DOUBLE)
                        {
                            inst = DIVS;
                        }
                        generate_instruction(inst);
                    }
                    break;
                }
                }
            }
            else if (derivation_ok && (isTokenLiteral(top.type) || top.type == TOKEN_IDENTIFICATOR))
            {
                // for operands, we just need to push them on the stack
                generate_instruction(PUSHS, symbol(convertPSATokenToToken(top)));
            }

            if (!PSA_Token_stack_empty(s) && PSA_Token_stack_top(s).type == TOKEN_NOT)
            {
                // if the top of the stack is '!', then we need to generate the NOTS instruction
                generate_instruction(NOTS);
            }

            // the resulting successfully derivated token of the derivation is pushed on the stack
            if (rule.type != TOKEN_EOF)
            {
                PSA_Token_stack_push(s, rule);
            }
            else
            {
                // if the derivation was not successful, we need to throw an error
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
            // a missing (on an ivalid '-') rule in the precedence table is an error
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
    DEBUG_PSA_CODE(printf("\n");
                   printf_green("PSA: ✅ | All good! \n"););

    // check for unmatched brackets (for function parameters, the number of brackets may be -1, as the expression may end with a right bracket)
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

    // free the stack and return the result
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
    return parse_expression_base(false); // false - not a parameter expression
}

psa_return_type parse_expression_param()
{
    return parse_expression_base(true); // true - parameter expression
}
