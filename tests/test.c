/**
 * @file main.c
 * @brief main function of the program
 * @author Tomáš Hobza
 *
 * Project: IFJ compiler
 */

#include <stdio.h>
#include <stdlib.h>
#include "psa.h"
#include "utils.h"
#include "symtable.h"

int main(void)
{
    symtable_t st = symtable_init();

    symtable_print(st);

    symtable_add()

        // psa_return_type psa_ret = parse_expression();

        // // // print type and is ok
        // printf_magenta("\n\n==== PSA RETURN ====\n");
        // printf("Is ok: ");
        // if (psa_ret.is_ok)
        // {
        //     printf_green("true\n");
        // }
        // else
        // {
        //     printf_red("false\n");
        // }

        // printf("Type: ");
        // print_expression_type(psa_ret.type);

        // printf("End token: ");
        // print_token_type(psa_ret.end_token);

        // printf("Can be nil: ");
        // if (psa_ret.canBeNil)
        // {
        //     printf_green("true\n");
        // }
        // else
        // {
        //     printf_red("false\n");
        // }
        // printf_magenta("====================\n");

        return EXIT_SUCCESS;
}
