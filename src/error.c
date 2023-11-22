#include "error.h"

DEFINE_STACK_FUNCTIONS(Error)

static Error_stack *error_st = NULL;

void throw_error(Error_code code, unsigned int line_num, char *message)
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

void print_errors()
{
    if (error_st == NULL || Error_stack_empty(error_st))
    {
        printf_green("✅ The compiler found no errors.\n");
        return;
    }

    // sort the errors by line number
    Error_stack *sorted = Error_stack_init();
    Error_stack *sorting_helper = Error_stack_init();
    if (sorted == NULL || sorting_helper == NULL)
    {
        return;
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

    // print the sorted errors
    while (!Error_stack_empty(sorted))
    {
        printError(Error_stack_pop(sorted));
    }

    Error_stack_free(sorted);
    Error_stack_free(sorting_helper);
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