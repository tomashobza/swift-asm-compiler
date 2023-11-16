/**
 * @file parser.c
 * @brief Parser of project
 * @details This module is used for parsing tokens from scanner and generating code in IFJcode23
 * @author Jakub Všetečka
 * 
 * Project: IFJ compiler
*/

#include "parser.h"


/**
 * @def START
 * @brief START	->	STMT_LIST eof
 * @brief first: {'',$,func,if,id,while,var,let}
 * @brief follow: {$}
*/
bool START (Token *token) {


}

/**
 * @brief PARSER MAIN
*/
int parser_main() {
    Token *token = malloc(sizeof(Token));
    
    while (token->type != 18) {
        generate_token(token, "\0", false);
        printf("Token type: %d, value: %s\n", token->type, token->token_value);
    }

    free(token);
    return EXIT_SUCCESS;
}