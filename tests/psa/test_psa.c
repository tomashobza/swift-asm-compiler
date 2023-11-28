/**
 * @file main.c
 * @brief testing main function of the program for testing purposes
 * @author Tomáš Hobza
 *
 * Project: IFJ compiler
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "symtable.h"

symtable_stack *sym_st;

#include "psa.h"
#include "error.h"
#include "parser.h"
#include "utils.h"

int main(int argc, char const *argv[])
{
    scanner_init();

    sym_st = symtable_stack_init();

    symtable_stack_push(sym_st, symtable_init());

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

    symtable_add(new_item, symtable_stack_top(sym_st));

    symtable_item new_item2 = (symtable_item){
        .id = "prom",
        .type = VARIABLE,
        .data = {
            .var_data = &(VariableData){
                .type = TYPE_INT_NIL,
                .is_const = false,
                .is_initialized = false,
                .is_param = false,
            },
        },
    };

    symtable_add(new_item2, symtable_stack_top(sym_st));

    psa_return_type psa_ret = parse_expression();

    DEBUG_PSA_CODE(
        fprintf_magenta(stderr, "\n\n==== PSA RETURN ====\n");
        fprintf(stderr, "Is ok: ");
        if (psa_ret.is_ok) {
            fprintf_green(stderr, "true\n");
        } else {
            fprintf_red(stderr, "false\n");
        }

        fprintf(stderr, "Type: ");
        fprint_expression_type(psa_ret.type, stderr);

        fprintf(stderr, "End token: ");
        fprint_token_type(psa_ret.end_token, stderr);

        fprintf_magenta(stderr, "====================\n\n");

        Token next_token;
        main_scanner(&next_token);
        fprintf_yellow(stderr, "Next token is: {'%s':%d}\n", next_token.token_value, next_token.type);

        if (argc == 3) {
            bool should_be_ok = atoi(argv[1]) != 0 ? true : false;
            Expression_type expected_type = (Expression_type)atoi(argv[2]);

            assert(psa_ret.is_ok == should_be_ok);
            assert(psa_ret.type == expected_type);
        });

    // print errors
    return print_errors();
}
