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

int main(void)
{
    scanner_init();

    printf("Hlavni program. :)\n");

    sym_st = symtable_stack_init();

    parser_main();

    return print_errors();
}
