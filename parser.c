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
    DEBUG_CODE(printf(YELLOW "popped: %s" RESET "\n", (*token)->token_value););
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

/**
 * @def STMT
 * @brief   STMT	->	VAR_LET
            STMT	->	DEF_FUNC
            STMT	->	IF_STMT
            STMT	->	LOAD_ID
            STMT	->	WHILE_STMT
            VAR_LET	->	VAR_SCOPE id TYPE_AND_ASSIGN

    first: func, if, id, while, var, let
    follow; $, func, if, id, while, var, let, }
*/
bool STMT(Token **token) {
    DEBUG_CODE(printf("STMT    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch((*token)->type) {
        case TOKEN_FUNC: return DEF_FUNC(token);
        case TOKEN_IF: return IF_STMT(token);
        case TOKEN_IDENTIFICATOR: return LOAD_ID(token);
        case TOKEN_WHILE: return WHILE_STMT(token);
        case TOKEN_VAR: return VAR_LET(token);
        case TOKEN_LET: return VAR_LET(token);
        default: return false;
    }
}

bool VAR_LET(Token **token) {
    DEBUG_CODE(printf("VAR_LET    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch((*token)->type) {
        case TOKEN_VAR: return VAR_SCOPE(token) && cmp_type(token, TOKEN_IDENTIFICATOR) && TYPE_AND_ASIGN(token);
        case TOKEN_LET: return VAR_SCOPE(token) && cmp_type(token, TOKEN_IDENTIFICATOR) && TYPE_AND_ASIGN(token);
        default: return false;
    }
}

bool VAR_SCOPE(Token **token) {
    DEBUG_CODE(printf("VAR_SCOPE    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch((*token)->type) {
        case TOKEN_VAR: return cmp_type(token, TOKEN_VAR);
        case TOKEN_LET: return cmp_type(token, TOKEN_LET);
        default: return false;
    }
}

bool TYPE_AND_ASIGN(Token **token) {
    DEBUG_CODE(printf("TYPE_AND_ASIGN    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch((*token)->type) {
        // TYPE_AND_ASIGN -> : D_TYPE R_FLEX
        case TOKEN_DOUBLE_DOT: return cmp_type(token, TOKEN_DOUBLE_DOT) && D_TYPE(token) && R_FLEX(token);
        // TYPE_AND_ASIGN -> = RIGID
        case TOKEN_ASSIGN: return cmp_type(token, TOKEN_ASSIGN) && R_RIGID(token);
        default: return false;
    }
}

bool R_RIGID(Token **token) {
    DEBUG_CODE(printf("R_RIGID    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch((*token)->type) {
        case TOKEN_ASSIGN: return cmp_type(token, TOKEN_ASSIGN) && EXP(token);
        default: return false;
    }
}

bool D_TYPE(Token **token) {
    DEBUG_CODE(printf("D_TYPE    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch((*token)->type) {
        case TOKEN_TYPE_STRING: return cmp_type(token, TOKEN_TYPE_STRING);
        case TOKEN_TYPE_INT: return cmp_type(token, TOKEN_TYPE_INT);
        case TOKEN_TYPE_DOUBLE: return cmp_type(token, TOKEN_TYPE_DOUBLE);
        case TOKEN_TYPE_BOOL: return cmp_type(token, TOKEN_TYPE_BOOL);
        default: return false;
    }
}

bool R_FLEX(Token **token) {
    DEBUG_CODE(printf("R_FLEX    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch((*token)->type) {
        case TOKEN_EOF: return true;
        case TOKEN_IDENTIFICATOR: return true;
        case TOKEN_VAR: return true;
        case TOKEN_LET: return true;
        case TOKEN_ASSIGN: return cmp_type(token, TOKEN_ASSIGN) && EXP(token);
        case TOKEN_FUNC: return true;
        case TOKEN_R_CURLY: return true;
        case TOKEN_RETURN: return true;
        case TOKEN_WHILE: return true;
        case TOKEN_IF: return true;
        default: return false;
    }
}

bool DEF_FUNC(Token **token) {
    DEBUG_CODE(printf("DEF_FUNC    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch((*token)->type) {
        //  DEF_FUNC -> func id ( P_LIST ) RET_TYPE { FUNC_STMT_LIST }	
        case TOKEN_FUNC: return cmp_type(token, TOKEN_FUNC) && cmp_type(token, TOKEN_IDENTIFICATOR) && cmp_type(token, TOKEN_L_BRACKET) &&
                                P_LIST(token) && cmp_type(token, TOKEN_R_BRACKET) && RET_TYPE(token) && cmp_type(token, TOKEN_L_CURLY) &&
                                FUNC_STMT_LIST(token) && cmp_type(token, TOKEN_R_CURLY);
        default: return false;
    }
}

bool P_LIST(Token **token) {
    DEBUG_CODE(printf("P_LIST    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch((*token)->type) {
        case TOKEN_IDENTIFICATOR: return PARAM(token);
        case TOKEN_R_BRACKET: return true;
        default: return false;
    }
}

bool PARAM(Token **token) {
    DEBUG_CODE(printf("PARAM    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch((*token)->type) {
        // PARAM -> id id : D_TYPE SEP
        case TOKEN_IDENTIFICATOR: return cmp_type(token, TOKEN_IDENTIFICATOR) && cmp_type(token, TOKEN_IDENTIFICATOR) && cmp_type(token, TOKEN_DOUBLE_DOT) &&
                                D_TYPE(token) && P_SEP(token);
        default: return false;
    }
}   

bool P_SEP(Token **token) {
    DEBUG_CODE(printf("P_SEP    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch((*token)->type) {
        // P_SEP -> eps
        case TOKEN_R_BRACKET: return true;
        // P_SEP -> , PARAM
        case TOKEN_COMMA: return cmp_type(token, TOKEN_COMMA) && PARAM(token);
        default: return false;
    }
}

bool RET_TYPE(Token **token) {
    DEBUG_CODE(printf("RET_TYPE    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch((*token)->type) {
        // RET_TYPE -> eps
        case TOKEN_L_CURLY: return true;
        // RET_TYPE -> -> D_TYPE
        case TOKEN_ARROW: return cmp_type(token, TOKEN_ARROW) && D_TYPE(token);
        default: return false;
    }
}

bool FUNC_STMT_LIST(Token **token) {
    DEBUG_CODE(printf("FUNC_STMT_LIST    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch((*token)->type) {
        // FUNC_STMT_LIST -> FUNC_STMT FUNC_STMT_LIST
        case TOKEN_IDENTIFICATOR: return FUNC_STMT(token) && FUNC_STMT_LIST(token);
        case TOKEN_VAR: return FUNC_STMT(token) && FUNC_STMT_LIST(token);
        case TOKEN_LET: return FUNC_STMT(token) && FUNC_STMT_LIST(token);
        case TOKEN_RETURN: return FUNC_STMT(token) && FUNC_STMT_LIST(token);
        case TOKEN_WHILE: return FUNC_STMT(token) && FUNC_STMT_LIST(token);
        case TOKEN_IF: return FUNC_STMT(token) && FUNC_STMT_LIST(token);
        // FUNC_STMT_LIST -> eps
        case TOKEN_R_CURLY: return true;
        default: return false;
    }
}

bool FUNC_STMT(Token **token) {
    DEBUG_CODE(printf("FUNC_STMT    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch((*token)->type) {
        // FUNC_STMT -> LOAD_ID
        case TOKEN_IDENTIFICATOR: return LOAD_ID(token);
        // FUNC_STMT -> VAR_LET
        case TOKEN_VAR: return VAR_LET(token);
        case TOKEN_LET: return VAR_LET(token);
        // FUNC_STMT -> RET
        case TOKEN_RETURN: return RET(token);
        // FUNC_STMT -> FUNC_WHILE
        case TOKEN_WHILE: return FUNC_WHILE(token);
        // FUNC_STMT -> FUNC_IF
        case TOKEN_IF: return FUNC_IF(token);
    }
}

bool RET(Token **token) {
    DEBUG_CODE(printf("RET    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch((*token)->type) {
        // RET -> return EXP
        case TOKEN_RETURN: return cmp_type(token, TOKEN_RETURN) && EXP(token);
        default: return false;
    }
}

bool FUNC_WHILE(Token **token) {
    DEBUG_CODE(printf("FUNC_WHILE    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch((*token)->type) {
        // FUNC_WHILE -> while EXP { FUNC_STMT_LIST }
        case TOKEN_WHILE: return cmp_type(token, TOKEN_WHILE) && EXP(token) && cmp_type(token, TOKEN_L_CURLY) &&
                                FUNC_STMT_LIST(token) && cmp_type(token, TOKEN_R_CURLY);
        default: return false;
    }
}

bool FUNC_IF(Token **token) {
    DEBUG_CODE(printf("FUNC_IF    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch((*token)->type) {
        // 	FUNC_IF -> if EXP { FUNC_STMT_LIST } FUNC_ELSE_CLAUSE	
        case TOKEN_IF: return cmp_type(token, TOKEN_IF) && EXP(token) && cmp_type(token, TOKEN_L_CURLY) &&
                                FUNC_STMT_LIST(token) && cmp_type(token, TOKEN_R_CURLY) && FUNC_ELSE_CLAUSE(token);
        default: return false;
    }
}

bool FUNC_ELSE_CLAUSE(Token **token) {
    DEBUG_CODE(printf("FUNC_ELSE_CLAUSE    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch((*token)->type) {
        // FUNC_ELSE_CLAUSE -> eps
        case TOKEN_IDENTIFICATOR: return true;
        case TOKEN_VAR: return true;
        case TOKEN_LET: return true;
        case TOKEN_R_CURLY: return true;
        case TOKEN_RETURN: return true;
        case TOKEN_WHILE: return true;
        case TOKEN_IF: return true;
        // FUNC_ELSE_CLAUSE -> else FUNC_AFTER_ELSE
        case TOKEN_ELSE: return cmp_type(token, TOKEN_ELSE) && FUNC_AFTER_ELSE(token);
    }
}

bool FUNC_AFTER_ELSE(Token **token) {
    DEBUG_CODE(printf("FUNC_AFTER_ELSE    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch((*token)->type) {
        // FUNC_AFTER_ELSE -> { FUNC_STMT_LIST }
        case TOKEN_L_CURLY: return cmp_type(token, TOKEN_L_CURLY) && FUNC_STMT_LIST(token) && cmp_type(token, TOKEN_R_CURLY);
        // FUNC_AFTER_ELSE -> FUNC_IF
        case TOKEN_IF: return FUNC_IF(token);
    }
}

bool IF_STMT(Token **token) {
    DEBUG_CODE(printf("IF_STMT    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch((*token)->type) {
        // IF_STMT -> if EXP { STMT_LIST } ELSE_CLAUSE
        case TOKEN_IF: return cmp_type(token, TOKEN_IF) && EXP(token) && cmp_type(token, TOKEN_L_CURLY) &&
                                STMT_LIST(token) && cmp_type(token, TOKEN_R_CURLY) && ELSE_CLAUSE(token);
        default: return false;
    }
}

bool ELSE_CLAUSE(Token **token) {
    DEBUG_CODE(printf("ELSE_CLAUSE    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch((*token)->type) {
        // FUNC_AFTER_ELSE -> FUNC_IF
        case TOKEN_IDENTIFICATOR: return true;
        case TOKEN_VAR: return true;
        case TOKEN_LET: return true;
        case TOKEN_FUNC: return true;
        case TOKEN_R_CURLY: return true;
        case TOKEN_WHILE: return true;
        case TOKEN_IF: return true;
        // ELSE_CLAUSE -> else AFTER_ELSE
        case TOKEN_ELSE: return cmp_type(token, TOKEN_ELSE) && AFTER_ELSE(token);
        default: return false;
    }
}

bool AFTER_ELSE(Token **token) {
    DEBUG_CODE(printf("AFTER_ELSE    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch((*token)->type) {
        // AFTER_ELSE -> { STMT_LIST }
        case TOKEN_L_CURLY: return cmp_type(token, TOKEN_L_CURLY) && STMT_LIST(token) && cmp_type(token, TOKEN_R_CURLY);
        // AFTER_ELSE -> IF_STMT
        case TOKEN_IF: return IF_STMT(token);
        default: return false;
    }
}

bool WHILE_STMT(Token **token) {
    DEBUG_CODE(printf("WHILE_STMT    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch((*token)->type) {
        // WHILE_STMT -> while EXP { STMT_LIST }
        case TOKEN_WHILE: return cmp_type(token, TOKEN_WHILE) && EXP(token) && cmp_type(token, TOKEN_L_CURLY) &&
                                STMT_LIST(token) && cmp_type(token, TOKEN_R_CURLY);
        default: return false;
    }
}

bool LOAD_ID(Token **token) {
    DEBUG_CODE(printf("LOAD_ID    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch((*token)->type) {
        // LOAD_ID -> id ALL_AFTER_ID
        case TOKEN_IDENTIFICATOR: return cmp_type(token, TOKEN_IDENTIFICATOR) && ALL_AFTER_ID(token);
        default: return false;
    }
}

bool ALL_AFTER_ID(Token **token) {
    DEBUG_CODE(printf("ALL_AFTER_ID    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch((*token)->type) {
        // ALL_AFTER_ID -> = EXP
        case TOKEN_ASSIGN: return cmp_type(token, TOKEN_ASSIGN) && EXP(token);
        // ALL_AFTER_ID -> FUNC_CALL
        case TOKEN_L_BRACKET: return cmp_type(token, TOKEN_L_BRACKET) && IN_P_LIST(token) && cmp_type(token, TOKEN_R_BRACKET);
        default: return false;
    }
}

bool FUNC_CALL(Token **token) {
    DEBUG_CODE(printf("FUNC_CALL    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    return true;
}

bool EXP(Token **token) {
    DEBUG_CODE(printf("EXP    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
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
    generate_token(token, "\0", false);
    bool all_ok = START(&token);
    if (all_ok) {
        printf(GREEN "All OK" RESET "\n");
    } else {
        printf(RED "Something went wrong" RESET "\n");
    }
    free(token);
    return EXIT_SUCCESS;
}