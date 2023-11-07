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
#include "psa.h"

int main(void)
{
    // Token *token = malloc(sizeof(Token));
    // int ch = getchar();
    // ;
    // while (ch != EOF)
    // {
    //     ungetc(ch, stdin);
    //     main_scanner(token);
    //     ch = getchar();
    // }
    // printf("Hello World!\n");
    parse_expression();

    return EXIT_SUCCESS;
}
