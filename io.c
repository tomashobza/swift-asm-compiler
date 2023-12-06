/**
 * @file io.c
 * @author Tomáš Hobza (xhobza03@vutbr.cz)
 * @brief Functions for input (reading the next token of the expression) and output (printing, mainly) needs for the PSA.
 * @version 0.1
 * @date 2023-11-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "psa.h"

PSA_Token readNextToken(PSA_Token_stack *s, char *next_token_error, int *num_of_brackets, bool ignore_func_call)
{
    Token *tkn = malloc(sizeof(Token));
    if (tkn == NULL)
    {
        throw_error(INTERNAL_ERR, -1, "Memory allocation for next PSA token failed.");
        return PSA_TOKEN_EOF;
    }
    *tkn = (Token){
        .type = (Token_type)TOKEN_EOF,
        .token_value = "$",
        .preceded_by_nl = true,
    };

    Error_code scanner_returned = (Error_code)main_scanner(tkn);
    if (scanner_returned != NO_ERR)
    {
        throw_error(scanner_returned, tkn->line_num, "Scanner error.");
        DEBUG_PSA_CODE(printf_red("\nSCANNER VRATIL: ");
                       printError((Error){
                           .code = scanner_returned,
                           .line_num = line_num,
                           .message = "Scanner error."});
                       printf("\n\n"););
    }

    if (tkn->type == TOKEN_EOF)
    {
        return_token(*tkn);
    }

    PSA_Token b = {
        .type = tkn->type,
        .token_value = tkn->token_value,
        .expr_type = getTypeFromToken(tkn->type),
        .preceded_by_nl = tkn->type == TOKEN_EOF ? true : tkn->preceded_by_nl,
        .is_literal = isTokenLiteral(tkn->type),
        .line_num = tkn->line_num,
    };

    free(tkn);

    PSA_Token a = PSA_TOKEN_EOF;

    if (s != NULL && !PSA_Token_stack_empty(s) && s->top != NULL)
    {
        a = PSA_Token_stack_top(s); // HERE IS THE SEGFAULT
    }

    bool is_func_call = b.type == TOKEN_FUNC_ID;
    bool is_after_binary_operator = isTokenBinaryOperator(a.type);
    bool is_after_unwrap_operator = s->size >= 2 && a.type == TOKEN_NOT && s->top->next->data.type == TOKEN_EXPRSN;
    bool is_first_token = a.type == TOKEN_EOF;
    if (is_func_call && !ignore_func_call)
    {
        if ((is_after_binary_operator || is_first_token) && !is_after_unwrap_operator)
        {

            DEBUG_PSA_CODE(PSA_Token og_b = b;
                           printf("func call: %s\n", og_b.token_value););

            int param_count = 0;
            b = parseFunctionCall(s, b, &param_count);

            // YEET - tady se vola a funkce

            if (b.expr_type != TYPE_INVALID && b.type != TOKEN_EOF)
            {
                if (isBuiltInFunction(convertPSATokenToToken(b)))
                {
                    generate_builtin_func_call(convertPSATokenToToken(b), param_count);
                }
                else
                {
                    generate_instruction(CALL, label(b.token_value));
                    generate_instruction(PUSHS, variable("retval", -1, false));
                }
            }

            DEBUG_PSA_CODE(printf_cyan("func call type: ");
                           print_expression_type(b.expr_type););
        }
        else
        {
            return_token(convertPSATokenToToken(b));
            b = (PSA_Token){
                .type = TOKEN_EOF,
                .token_value = "$",
                .preceded_by_nl = true,
            };
        }
    }

    *next_token_error = 0;

    // detect expression end by splitting operator and operand with EOL
    *next_token_error += (a.type == TOKEN_NOT && b.preceded_by_nl) ? 1 : 0;
    *next_token_error = *next_token_error << 1;

    // detect expression end by a missing operator between operands
    *next_token_error += (isTokenOperand(a.type) && !isTokenBinaryOperator(b.type) && !isTokenBracket(b.type) &&
                          b.type != TOKEN_NOT)
                             ? 1
                             : 0;
    *next_token_error = *next_token_error << 1;

    // detect expression end by an illegal token for expression being read
    *next_token_error += (getSymbolValue(b.type) >= 99) ? 1 : 0;
    *next_token_error = *next_token_error << 1;

    // update the bracket counter
    if (num_of_brackets != NULL)
    {
        switch (b.type)
        {
        case TOKEN_L_BRACKET:
            (*num_of_brackets)++;
            break;
        case TOKEN_R_BRACKET:
            (*num_of_brackets)--;
            break;
        default:
            break;
        }
    }

    return b;
}

void printStackRec(PSA_Token_node *top)
{
    if (top == NULL)
    {
        return;
    }
    printStackRec(top->next);
    printf("%s", top->data.token_value);
}

void printStack(PSA_Token_node *top)
{
    if (top == NULL)
    {
        printf("Stack empty!\n");
        return;
    }
    printStackRec(top);
    printf("\n");
}

void printTokenArray(PSA_Token *handle, unsigned int len)
{
    printf("handle: '");
    for (unsigned int i = 0; i < len; i++)
    {
        printf("%s", handle[i].token_value);
    }
    printf("'\n");
}
