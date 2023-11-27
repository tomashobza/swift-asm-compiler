/**
 * @file main.c
 * @brief testing main function of the program for testing purposes
 * @author Tomáš Hobza
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

    sym_st = symtable_stack_init();

    parser_main();

    // throw_error(SYNTACTIC_ERR, "This is a syntactic error");
    // throw_error(LEXICAL_ERR, "This is a lexical error");
    // throw_error(SEMANTICS_ERR, "This is a semantic error");
    // throw_error(INTERNAL_ERR, "This is an internal error");
    // throw_error(TYPE_ERR, "This is a type error");

    // print errors
    return print_errors();
}
