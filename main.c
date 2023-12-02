/**
 * @file main.c
 * @brief main function of the program
 * @author Tomáš Hobza <xhobza03@vutbr.cz>
 *
 * Project: IFJ compiler
 */

#include <stdio.h>
#include <stdlib.h>
#include "symtable.h"

symtable_stack *sym_st;

#include "psa.h"
#include "error.h"
#include "parser.h"
#include "utils.h"
#include "generator.h"

FILE *out_code_file = NULL;

int main(void)
{
    FILE *out_code_file = tmpfile();
    if (out_code_file == NULL)
    {
        return INTERNAL_ERR;
    }

    scanner_init();

    sym_st = symtable_stack_init();

    parser_main();

    Error_code error_code = print_errors();
    if (error_code == NO_ERR)
    {
        print_out_code();
    }
    return error_code;
}
