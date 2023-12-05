/**
 * @file error.c
 * @author Tomáš Hobza (xhobza03@vutbr.cz), Anastasia Butok (xbutok00@vutbr.cz)
 * @brief Functions for error handling (adding errors to the stack, printing them, etc.).
 * @version 0.1
 * @date 2023-11-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "error.h"

DEFINE_STACK_FUNCTIONS(Error)

static Error_stack *error_st = NULL;

void throw_error_base(Error_code code, char *message, int line_num)
{
    if (error_st == NULL)
    {

        error_st = Error_stack_init();
        if (error_st == NULL)
        {
            return;
        }
    }
    Error_stack_push(error_st, (Error){
                                   .code = code,
                                   .line_num = line_num,
                                   .message = message});

    exit(print_errors());
}

Error_code print_errors()
{
    if (error_st == NULL || Error_stack_empty(error_st))
    {
        // fprintf_green(stderr, "✅ The compiler found no errors.\n");
        return NO_ERR;
    }

    int first_error_code = NO_ERR;

    // TODO: add stderr

    fprintf(stderr, "\nCOMPILER FOUND ");
    fprintf_red(stderr, "%d ERRORS:\n\n", error_st->size);

    while (!Error_stack_empty(error_st))
    {
        first_error_code = Error_stack_top(error_st).code;
        printError(Error_stack_pop(error_st));
    }

    Error_stack_free(error_st);

    return first_error_code;
}

void printError(Error error)
{
    fprintf(stderr, "code:");
    fprintf_red(stderr, "%d", error.line_num);
    fprintf(stderr, ": ");
    fprintf_red(stderr, "error: ");
    printErrorCode(error.code);
    fprintf_red(stderr, "%s\n\n", error.message);
}

void printErrorCode(Error_code code)
{
    switch (code)
    {
    case LEXICAL_ERR:
        fprintf_red(stderr, "LEXICAL_ERR");
        break;
    case SYNTACTIC_ERR:
        fprintf_red(stderr, "SYNTACTIC_ERR");
        break;
    case FUNCTIONS_ERR:
        fprintf_red(stderr, "FUNCTIONS_ERR");
        break;
    case PARAM_TYPE_ERR:
        fprintf_red(stderr, "PARAM_TYPE_ERR");
        break;
    case VARIABLES_ERR:
        fprintf_red(stderr, "VARIABLES_ERR");
        break;
    case RETURN_ERR:
        fprintf_red(stderr, "RETURN_ERR");
        break;
    case COMPATIBILITY_ERR:
        fprintf_red(stderr, "COMPATIBILITY_ERR");
        break;
    case TYPE_ERR:
        fprintf_red(stderr, "TYPE_ERR");
        break;
    case SEMANTICS_ERR:
        fprintf_red(stderr, "SEMANTICS_ERR");
        break;
    case INTERNAL_ERR:
        fprintf_red(stderr, "INTERNAL_ERR");
        break;
    default:
        fprintf_red(stderr, "UNKNOWN_ERR");
        break;
    }
    fprintf_red(stderr, "\n");
}