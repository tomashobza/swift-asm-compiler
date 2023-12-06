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
FILE *while_def_out_code_file = NULL;
bool is_in_loop = false;

int main(void)
{
    // open the temporary file
    out_code_file = tmpfile();
    if (out_code_file == NULL)
    {
        return INTERNAL_ERR;
    }

    // push necessary instructions to the beginning of the file
    fprintf(out_code_file, ".IFJcode23\n");
    generate_instruction(CREATEFRAME);
    generate_instruction(PUSHFRAME);
    generate_instruction(CREATEFRAME);
    fprintf(out_code_file, "\n");

    scanner_init(); // initialize the scanner

    sym_st = symtable_stack_init(); // initialize the symbol table stack

    run_parser(); // run the parser

    // if there were any errors, print them and exit
    Error_code error_code = print_errors();
    if (error_code == NO_ERR)
    {
        print_out_code(); // if there were no errors, print the compiled code
    }
    fclose(out_code_file);
    return error_code; // return the appropriate error code
}
