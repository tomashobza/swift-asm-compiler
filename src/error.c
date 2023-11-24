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

void throw_error_base(Error_code code, char *message)
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
}

Error_code print_errors()
{
    if (error_st == NULL || Error_stack_empty(error_st))
    {
        printf_green("✅ The compiler found no errors.\n");
        return NO_ERR;
    }

    // sort the errors by line number
    Error_stack *sorted = Error_stack_init();
    Error_stack *sorting_helper = Error_stack_init();
    if (sorted == NULL || sorting_helper == NULL)
    {
        return INTERNAL_ERR;
    }

    // sort the errors by line number
    while (!Error_stack_empty(error_st))
    {
        Error insertee = Error_stack_pop(error_st);

        if (Error_stack_empty(sorted))
        {
            Error_stack_push(sorted, insertee);
        }
        else
        {
            // pop items from the sorted stack into the sorting_helper stack that have a line number smaller than Pavel's line number
            Error tmp = Error_stack_top(sorted);
            while (tmp.line_num < insertee.line_num)
            {
                Error_stack_push(sorting_helper, Error_stack_pop(sorted));
                if (Error_stack_empty(sorted))
                {
                    break;
                }
                else
                {
                    tmp = Error_stack_top(sorted);
                }
            }

            // push Pavel into the sorted stack
            Error_stack_push(sorted, insertee);

            while (!Error_stack_empty(sorting_helper))
            {
                Error_stack_push(sorted, Error_stack_pop(sorting_helper));
            }
        }
    }

    // TODO: add stderr

    printf("COMPILER FOUND ");
    printf_red("%d ERRORS:\n\n", sorted->size);

    int first_error_code = NO_ERR;

    // print the sorted errors
    while (!Error_stack_empty(sorted))
    {
        if (sorted->size == 1)
        {
            first_error_code = Error_stack_top(sorted).code;
        }

        printError(Error_stack_pop(sorted));
    }

    Error_stack_free(sorted);
    Error_stack_free(sorting_helper);

    return first_error_code;
}

void printError(Error error)
{
    printf("code:");
    printf_red("%d", error.line_num);
    printf(": ");
    printf_red("error: ");
    printErrorCode(error.code);
    printf("%s\n\n", error.message);
}

void printErrorCode(Error_code code)
{
    switch (code)
    {
    case LEXICAL_ERR:
        printf_red("LEXICAL_ERR");
        break;
    case SYNTACTIC_ERR:
        printf_red("SYNTACTIC_ERR");
        break;
    case FUNCTIONS_ERR:
        printf_red("FUNCTIONS_ERR");
        break;
    case PARAM_TYPE_ERR:
        printf_red("PARAM_TYPE_ERR");
        break;
    case VARIABLES_ERR:
        printf_red("VARIABLES_ERR");
        break;
    case RETURN_ERR:
        printf_red("RETURN_ERR");
        break;
    case COMPATIBILITY_ERR:
        printf_red("COMPATIBILITY_ERR");
        break;
    case TYPE_ERR:
        printf_red("TYPE_ERR");
        break;
    case SEMANTICS_ERR:
        printf_red("SEMANTICS_ERR");
        break;
    case INTERNAL_ERR:
        printf_red("INTERNAL_ERR");
        break;
    default:
        printf_yellow("UNKNOWN_ERR");
        break;
    }
    printf("\n");
}