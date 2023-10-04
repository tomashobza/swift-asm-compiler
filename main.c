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

int main()
{
    Token *token = malloc(sizeof(Token));
    int ch = getchar(); ;
    while(ch != EOF){
        ungetc(ch, stdin);
        main_scanner(token);
        ch = getchar();
    }
    //printf("Hello World!\n");

    return EXIT_SUCCESS;
}
