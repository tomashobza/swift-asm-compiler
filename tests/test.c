/**
 * @file main.c
 * @brief main function of the program
 * @author Tomáš Hobza
 *
 * Project: IFJ compiler
 */

#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "psa.h"
#include "parser.h"
#include "utils.h"
#include "symtable.h"

int main(void)
{
    scanner_init();

    symtable_stack *st_stack = symtable_stack_init();
    symtable_stack_push(st_stack, symtable_init());

    ParamData params[2] = {
        (ParamData){
            .name = "_",
            .id = "a",
            .type = TYPE_INT,
        },
        (ParamData){
            .name = "b",
            .id = "b",
            .type = TYPE_BOOL,
        },
    };

    symtable_item new_item = (symtable_item){
        .id = "foo",
        .type = FUNCTION,
        .data = {
            .func_data = &(FunctionData){
                .return_type = TYPE_INT,
                .params = params,
                .params_count = 2,
                .capacity = 0,
            },
        },
    };

    symtable_add(new_item, symtable_stack_top(st_stack));

    psa_return_type psa_ret = parse_expression(st_stack);

    printf_magenta("\n\n==== PSA RETURN ====\n");
    printf("Is ok: ");
    if (psa_ret.is_ok)
    {
        printf_green("true\n");
    }
    else
    {
        printf_red("false\n");
    }

    printf("Type: ");
    print_expression_type(psa_ret.type);

    printf("End token: ");
    print_token_type(psa_ret.end_token);

    printf("Can be nil: ");
    if (psa_ret.canBeNil)
    {
        printf_green("true\n");
    }
    else
    {
        printf_red("false\n");
    }
    printf_magenta("====================\n");

    // parser_main();

    // throw_error(SYNTACTIC_ERR, 3, "This is a syntactic error");
    // throw_error(LEXICAL_ERR, 9, "This is a lexical error");
    // throw_error(SEMANTICS_ERR, 5, "This is a semantic error");
    // throw_error(INTERNAL_ERR, 1, "This is an internal error");
    // throw_error(TYPE_ERR, 7, "This is a type error");

    // print errors
    print_errors();

    return EXIT_SUCCESS;
}
