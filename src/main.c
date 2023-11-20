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
    // printf("\n\n");
    // Token *token = malloc(sizeof(Token));
    // int ch = getchar();
    // ;
    // while (ch != EOF)
    // {
    //     ungetc(ch, stdin);

    //     Token *token = malloc(sizeof(Token));
    //     generate_token(token, "\0", false);
    //     printf("{ type: %d, value: '%s' }\n", token->type, token->token_value);

    //     ch = getchar();
    // }
    // printf("Hello World!\n");
    parse_expression();

    return EXIT_SUCCESS;
}
