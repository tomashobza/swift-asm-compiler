/**
 * @file main.c
 * @brief main function of the program
 * @author Tomáš Hobza
 *
 * Project: IFJ compiler
 */

#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "parser.h"

int main()
{
    parser_main();

    return EXIT_SUCCESS;
}