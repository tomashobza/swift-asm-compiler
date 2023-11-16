/**
 * @file parser.c
 * @brief Parser of project
 * @details This module is used for parsing tokens from scanner and generating code in IFJcode23
 * @author Jakub Všetečka
 * 
 * Project: IFJ compiler
*/

#include "parser.h"

#define RED   "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define BLUE  "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN  "\x1B[36m"
#define RESET "\x1B[0m"

#define DEBUG 1
#if DEBUG
#define DEBUG_CODE(code) \
    do                   \
    {                    \
        code             \
    } while (0)
#else
#define DEBUG_CODE(code) \
    do                   \
    {                    \
    } while (0)
#endif


/**
 * @def get_token
 * @param pointer to the token structure
*/
void get_token(Token **token) {
    generate_token(*token, "\0", false);
}

bool cmp_type(Token **token, Token_type type) {
    switch ((*token)->type) {
        case TOKEN_EOF: 
            return type == TOKEN_EOF;
        default:
            bool result = (*token)->type == type;
            get_token(token);
            return result;
    }
}

/**
 * @def START
 * @brief START	->	STMT_LIST eof
 *        first: {'',$,func,if,id,while,var,let}
 *        follow: {$}
*/
bool START(Token **token) {
    DEBUG_CODE(printf("START    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch ((*token)->type) {
        case TOKEN_EOF: return STMT_LIST(token) && cmp_type(token, TOKEN_EOF);
        case TOKEN_FUNC: return STMT_LIST(token) && cmp_type(token, TOKEN_EOF);
        case TOKEN_IF: return STMT_LIST(token) && cmp_type(token, TOKEN_EOF);
        case TOKEN_IDENTIFICATOR : return STMT_LIST(token) && cmp_type(token, TOKEN_EOF);
        case TOKEN_WHILE: return STMT_LIST(token) && cmp_type(token, TOKEN_EOF);
        case TOKEN_VAR: return STMT_LIST(token) && cmp_type(token, TOKEN_EOF);
        case TOKEN_LET: return STMT_LIST(token) && cmp_type(token, TOKEN_EOF);
        default: return false;
    }

}

/**
 * @def STMT_LIST
 * @brief STMT_LIST	->	STMT STMT_LIST
 *        STMT_LIST	->  eps
 *                    first: {'',func,if,id,while,var,let}
 *                    follow: {$,}}
*/
bool STMT_LIST(Token **token) {
    DEBUG_CODE(printf("STMT_LIST    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch ((*token)->type) {
        case TOKEN_EOF: return true;
        case TOKEN_FUNC: return STMT(token) && STMT_LIST(token);
        case TOKEN_IF: return STMT(token) && STMT_LIST(token);
        case TOKEN_IDENTIFICATOR : return STMT(token) && STMT_LIST(token);
        case TOKEN_WHILE: return STMT(token) && STMT_LIST(token);
        case TOKEN_VAR: return STMT(token) && STMT_LIST(token);
        case TOKEN_LET: return STMT(token) && STMT_LIST(token);
        case TOKEN_R_CURLY: return true; 
        default: return false;
    }
}

bool STMT(Token **token) {
    DEBUG_CODE(printf("STMT    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    get_token(token);
    return true;
}

/**
 * @brief PARSER MAIN
*/
int parser_main() {
    Token *token = malloc(sizeof(Token));
    
    //while (token->type != 18) {
    //    generate_token(token, "\0", false);
    //    printf("Token type: %d, value: %s\n", token->type, token->token_value);
    //}
    get_token(&token);
    bool all_ok = START(&token);
    if (all_ok) {
        printf(GREEN "All OK" RESET "\n");
    } else {
        printf(RED "Something went wrong" RESET "\n");
    }
    free(token);
    return EXIT_SUCCESS;
}