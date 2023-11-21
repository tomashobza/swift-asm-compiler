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
 * @def get_token
 * @param pointer to the token structure
 */
void get_token(Token **token)
{
    DEBUG_CODE(printf(YELLOW "popped: %s" RESET "\n", (*token)->token_value););
    main_scanner(*token);
}

bool cmp_type(Token **token, Token_type type, Sem_rule sem_rule)
{
    switch ((*token)->type)
    {
    case TOKEN_EOF:
        return type == TOKEN_EOF;
    default:
    {
        bool result = ((*token)->type == type);

        if (result)
        {
            check_semantic(token, sem_rule);
        }
        get_token(token);
        return result;
    }
    }
}

bool START(Token **token)
{
    DEBUG_CODE(printf("START    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch ((*token)->type)
    {
    // START -> STMT_LIST eof
    case TOKEN_EOF:
    case TOKEN_FUNC:
    case TOKEN_IF:
    case TOKEN_IDENTIFICATOR:
    case TOKEN_WHILE:
    case TOKEN_VAR:
    case TOKEN_LET:
        return STMT_LIST(token) && cmp_type(token, TOKEN_EOF, SEM_NONE);
    default:
        return false;
    }
}

bool STMT_LIST(Token **token)
{
    DEBUG_CODE(printf("STMT_LIST    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch ((*token)->type)
    {
    // STMT_LIST -> eps
    case TOKEN_EOF:
    case TOKEN_R_CURLY:
        return true;
    // STMT_LIST -> STMT STMT_LIST
    case TOKEN_FUNC:
    case TOKEN_IF:
    case TOKEN_IDENTIFICATOR:
    case TOKEN_WHILE:
    case TOKEN_VAR:
    case TOKEN_LET:
        return STMT(token) && STMT_LIST(token);
    default:
        return false;
    }
}

bool STMT(Token **token)
{
    DEBUG_CODE(printf("STMT    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch ((*token)->type)
    {
    // STMT -> DEF_FUNC
    case TOKEN_FUNC:
        return DEF_FUNC(token);
    // STMT -> IF_STMT
    case TOKEN_IF:
        return IF_STMT(token);
    // STMT -> LOAD_ID
    case TOKEN_IDENTIFICATOR:
        return LOAD_ID(token);
    // STMT -> WHILE_STMT
    case TOKEN_WHILE:
        return WHILE_STMT(token);
    // STMT -> VAR_LET
    case TOKEN_VAR:
        return VAR_LET(token);
    // STMT -> VAR_LET
    case TOKEN_LET:
        return VAR_LET(token);
    default:
        return false;
    }
}

bool VAR_LET(Token **token)
{
    DEBUG_CODE(printf("VAR_LET    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch ((*token)->type)
    {
    // VAR_LET -> VAR_SCOPE id TYPE_AND_ASSIGN
    case TOKEN_VAR:
    case TOKEN_LET:
        return VAR_SCOPE(token) && cmp_type(token, TOKEN_IDENTIFICATOR, VAR_ID) && TYPE_AND_ASIGN(token);
    default:
        return false;
    }
}

bool VAR_SCOPE(Token **token)
{
    DEBUG_CODE(printf("VAR_SCOPE    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch ((*token)->type)
    {
    case TOKEN_VAR:
        return cmp_type(token, TOKEN_VAR, VAR);
    case TOKEN_LET:
        return cmp_type(token, TOKEN_LET, LET);
    default:
        return false;
    }
}

bool TYPE_AND_ASIGN(Token **token)
{
    DEBUG_CODE(printf("TYPE_AND_ASIGN    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch ((*token)->type)
    {
    // TYPE_AND_ASIGN -> : D_TYPE R_FLEX
    case TOKEN_DOUBLE_DOT:
        return cmp_type(token, TOKEN_DOUBLE_DOT, SEM_NONE) && D_TYPE(token, VAR_TYPE) && R_FLEX(token);
    // TYPE_AND_ASIGN -> = EXP
    case TOKEN_ASSIGN:
        return cmp_type(token, TOKEN_ASSIGN, VAR_ASSIGN) && EXP(token, VAR_EXP);
    default:
        return false;
    }
}

bool R_FLEX(Token **token)
{
    DEBUG_CODE(printf("R_FLEX    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch ((*token)->type)
    {
    // R_FLEX -> = EXP
    case TOKEN_ASSIGN:
        return cmp_type(token, TOKEN_ASSIGN, SEM_NONE) && EXP(token, VAR_EXP);
    // R_FLEX -> eps
    case TOKEN_EOF:
    case TOKEN_IDENTIFICATOR:
    case TOKEN_VAR:
    case TOKEN_LET:
    case TOKEN_FUNC:
    case TOKEN_R_CURLY:
    case TOKEN_RETURN:
    case TOKEN_WHILE:
    case TOKEN_IF:
        return true;
    default:
        return false;
    }
}

bool D_TYPE(Token **token, Sem_rule sem_rule)
{
    DEBUG_CODE(printf("D_TYPE    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch ((*token)->type)
    {
    case TOKEN_TYPE_STRING:
        return cmp_type(token, TOKEN_TYPE_STRING, sem_rule);
    case TOKEN_TYPE_INT:
        return cmp_type(token, TOKEN_TYPE_INT, sem_rule);
    case TOKEN_TYPE_DOUBLE:
        return cmp_type(token, TOKEN_TYPE_DOUBLE, sem_rule);
    case TOKEN_TYPE_BOOL:
        return cmp_type(token, TOKEN_TYPE_BOOL, sem_rule);
    default:
        return false;
    }
}

bool DEF_FUNC(Token **token)
{
    DEBUG_CODE(printf("DEF_FUNC    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch ((*token)->type)
    {
    //  DEF_FUNC -> func func_id ( P_LIST ) RET_TYPE { FUNC_STMT_LIST }
    case TOKEN_FUNC:
        return cmp_type(token, TOKEN_FUNC, SEM_NONE) && cmp_type(token, TOKEN_FUNC_ID, FUNC_ID) && cmp_type(token, TOKEN_L_BRACKET, SEM_NONE) &&
               P_LIST(token) && cmp_type(token, TOKEN_R_BRACKET, SEM_NONE) && RET_TYPE(token) && cmp_type(token, TOKEN_L_CURLY, FUNC_HEADER_DONE) &&
               FUNC_STMT_LIST(token) && cmp_type(token, TOKEN_R_CURLY, POP_SCOPE);
    default:
        return false;
    }
}

bool P_LIST(Token **token)
{
    DEBUG_CODE(printf("P_LIST    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch ((*token)->type)
    {
    // P_LIST -> PARAM
    case TOKEN_IDENTIFICATOR:
        return PARAM(token);
    // P_LIST -> eps
    case TOKEN_R_BRACKET:
        return true;
    default:
        return false;
    }
}

bool PARAM(Token **token)
{
    DEBUG_CODE(printf("PARAM    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch ((*token)->type)
    {
    // PARAM -> id id : D_TYPE SEP
    case TOKEN_IDENTIFICATOR:
        return cmp_type(token, TOKEN_IDENTIFICATOR, P_NAME) && cmp_type(token, TOKEN_IDENTIFICATOR, P_ID) && cmp_type(token, TOKEN_DOUBLE_DOT, SEM_NONE) &&
               D_TYPE(token, P_TYPE) && P_SEP(token);
    default:
        return false;
    }
}

bool P_SEP(Token **token)
{
    DEBUG_CODE(printf("P_SEP    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch ((*token)->type)
    {
    // P_SEP -> eps
    case TOKEN_R_BRACKET:
        return true;
    // P_SEP -> , PARAM
    case TOKEN_COMMA:
        return cmp_type(token, TOKEN_COMMA, SEM_NONE) && PARAM(token);
    default:
        return false;
    }
}

bool RET_TYPE(Token **token)
{
    DEBUG_CODE(printf("RET_TYPE    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch ((*token)->type)
    {
    // RET_TYPE -> eps
    case TOKEN_L_CURLY:
        return true;
    // RET_TYPE -> -> D_TYPE
    case TOKEN_ARROW:
        return cmp_type(token, TOKEN_ARROW, SEM_NONE) && D_TYPE(token, R_TYPE);
    default:
        return false;
    }
}

bool FUNC_STMT_LIST(Token **token)
{
    DEBUG_CODE(printf("FUNC_STMT_LIST    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch ((*token)->type)
    {
    // FUNC_STMT_LIST -> FUNC_STMT FUNC_STMT_LIST
    case TOKEN_IDENTIFICATOR:
    case TOKEN_VAR:
    case TOKEN_LET:
    case TOKEN_RETURN:
    case TOKEN_WHILE:
    case TOKEN_IF:
        return FUNC_STMT(token) && FUNC_STMT_LIST(token);
    // FUNC_STMT_LIST -> eps
    case TOKEN_R_CURLY:
        return true;
    default:
        return false;
    }
}

bool FUNC_STMT(Token **token)
{
    DEBUG_CODE(printf("FUNC_STMT    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch ((*token)->type)
    {
    // FUNC_STMT -> LOAD_ID
    case TOKEN_IDENTIFICATOR:
        return LOAD_ID(token);
    // FUNC_STMT -> VAR_LET
    case TOKEN_VAR:
        return VAR_LET(token);
    case TOKEN_LET:
        return VAR_LET(token);
    // FUNC_STMT -> RET
    case TOKEN_RETURN:
        return RET(token);
    // FUNC_STMT -> FUNC_WHILE
    case TOKEN_WHILE:
        return FUNC_WHILE(token);
    // FUNC_STMT -> FUNC_IF
    case TOKEN_IF:
        return FUNC_IF(token);
    default:
        return false;
    }
}

bool RET(Token **token)
{
    DEBUG_CODE(printf("RET    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch ((*token)->type)
    {
    // RET -> return EXP
    case TOKEN_RETURN:
        return cmp_type(token, TOKEN_RETURN, SEM_NONE) && EXP(token, R_EXP);
    default:
        return false;
    }
}

bool FUNC_WHILE(Token **token)
{
    DEBUG_CODE(printf("FUNC_WHILE    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch ((*token)->type)
    {
    // FUNC_WHILE -> while EXP { FUNC_STMT_LIST }
    case TOKEN_WHILE:
        return cmp_type(token, TOKEN_WHILE, SEM_NONE) && EXP(token, COND_EXP) && cmp_type(token, TOKEN_L_CURLY, PUSH_SCOPE) &&
               FUNC_STMT_LIST(token) && cmp_type(token, TOKEN_R_CURLY, POP_SCOPE);
    default:
        return false;
    }
}

bool FUNC_IF(Token **token)
{
    DEBUG_CODE(printf("FUNC_IF    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch ((*token)->type)
    {
    // 	FUNC_IF -> if EXP { FUNC_STMT_LIST } FUNC_ELSE_CLAUSE
    case TOKEN_IF:
        return cmp_type(token, TOKEN_IF, SEM_NONE) && EXP(token, COND_EXP) && cmp_type(token, TOKEN_L_CURLY, PUSH_SCOPE) &&
               FUNC_STMT_LIST(token) && cmp_type(token, TOKEN_R_CURLY, POP_SCOPE) && FUNC_ELSE_CLAUSE(token);
    default:
        return false;
    }
}

bool FUNC_ELSE_CLAUSE(Token **token)
{
    DEBUG_CODE(printf("FUNC_ELSE_CLAUSE    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch ((*token)->type)
    {
    // FUNC_ELSE_CLAUSE -> eps
    case TOKEN_IDENTIFICATOR:
        return true;
    case TOKEN_VAR:
        return true;
    case TOKEN_LET:
        return true;
    case TOKEN_R_CURLY:
        return true;
    case TOKEN_RETURN:
        return true;
    case TOKEN_WHILE:
        return true;
    case TOKEN_IF:
        return true;
    // FUNC_ELSE_CLAUSE -> else FUNC_AFTER_ELSE
    case TOKEN_ELSE:
        return cmp_type(token, TOKEN_ELSE, SEM_NONE) && FUNC_AFTER_ELSE(token);
    default:
        return false;
    }
}

bool FUNC_AFTER_ELSE(Token **token)
{
    DEBUG_CODE(printf("FUNC_AFTER_ELSE    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch ((*token)->type)
    {
    // FUNC_AFTER_ELSE -> { FUNC_STMT_LIST }
    case TOKEN_L_CURLY:
        return cmp_type(token, TOKEN_L_CURLY, PUSH_SCOPE) && FUNC_STMT_LIST(token) && cmp_type(token, TOKEN_R_CURLY, POP_SCOPE);
    // FUNC_AFTER_ELSE -> FUNC_IF
    case TOKEN_IF:
        return FUNC_IF(token);
    default:
        return false;
    }
}

bool IF_STMT(Token **token)
{
    DEBUG_CODE(printf("IF_STMT    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch ((*token)->type)
    {
    // IF_STMT -> if EXP { STMT_LIST } ELSE_CLAUSE
    case TOKEN_IF:
        return cmp_type(token, TOKEN_IF, SEM_NONE) && EXP(token, COND_EXP) && cmp_type(token, TOKEN_L_CURLY, PUSH_SCOPE) &&
               STMT_LIST(token) && cmp_type(token, TOKEN_R_CURLY, POP_SCOPE) && ELSE_CLAUSE(token);
    default:
        return false;
    }
}

bool ELSE_CLAUSE(Token **token)
{
    DEBUG_CODE(printf("ELSE_CLAUSE    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch ((*token)->type)
    {
    // ELSE_CLAUSE -> eps
    case TOKEN_IDENTIFICATOR:
    case TOKEN_VAR:
    case TOKEN_LET:
    case TOKEN_FUNC:
    case TOKEN_R_CURLY:
    case TOKEN_WHILE:
    case TOKEN_IF:
    case TOKEN_EOF:
        return true;
    // ELSE_CLAUSE -> else AFTER_ELSE
    case TOKEN_ELSE:
        return cmp_type(token, TOKEN_ELSE, SEM_NONE) && AFTER_ELSE(token);
    default:
        return false;
    }
}

bool AFTER_ELSE(Token **token)
{
    DEBUG_CODE(printf("AFTER_ELSE    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch ((*token)->type)
    {
    // AFTER_ELSE -> { STMT_LIST }
    case TOKEN_L_CURLY:
        return cmp_type(token, TOKEN_L_CURLY, PUSH_SCOPE) && STMT_LIST(token) && cmp_type(token, TOKEN_R_CURLY, POP_SCOPE);
    // AFTER_ELSE -> IF_STMT
    case TOKEN_IF:
        return IF_STMT(token);
    default:
        return false;
    }
}

bool WHILE_STMT(Token **token)
{
    DEBUG_CODE(printf("WHILE_STMT    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch ((*token)->type)
    {
    // WHILE_STMT -> while EXP { STMT_LIST }
    case TOKEN_WHILE:
        return cmp_type(token, TOKEN_WHILE, SEM_NONE) && EXP(token, COND_EXP) && cmp_type(token, TOKEN_L_CURLY, PUSH_SCOPE) &&
               STMT_LIST(token) && cmp_type(token, TOKEN_R_CURLY, POP_SCOPE);
    default:
        return false;
    }
}

bool LOAD_ID(Token **token)
{
    DEBUG_CODE(printf("LOAD_ID    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch ((*token)->type)
    {
    // LOAD_ID -> id ALL_AFTER_ID
    case TOKEN_IDENTIFICATOR:
        return cmp_type(token, TOKEN_IDENTIFICATOR, LOAD_IDENTIF) && ALL_AFTER_ID(token);
    default:
        return false;
    }
}

bool ALL_AFTER_ID(Token **token)
{
    DEBUG_CODE(printf("ALL_AFTER_ID    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    switch ((*token)->type)
    {
    // ALL_AFTER_ID -> = EXP
    case TOKEN_ASSIGN:
        return cmp_type(token, TOKEN_ASSIGN, SEM_NONE) && EXP(token, IDENTIF_EXP);
    // ALL_AFTER_ID -> FUNC_CALL
    case TOKEN_L_BRACKET:
        return FUNC_CALL(token);
    default:
        return false;
    }
}

bool FUNC_CALL(Token **token)
{
    DEBUG_CODE(printf("FUNC_CALL    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    token = token;
    return true;
}

bool EXP(Token **token, Sem_rule sem_rule)
{
    DEBUG_CODE(printf("EXP    token: %d   value: %s\n", (*token)->type, (*token)->token_value););
    return_token(*token);
    check_semantic(token, sem_rule);
    get_token(token);
    return true;
}

/**
 * @brief PARSER MAIN
 */
int parser_main()
{
    Token *token = malloc(sizeof(Token));

    scanner_init();
    semantic_init();

    get_token(&token);
    bool all_ok = START(&token);
    if (all_ok)
    {
        printf(GREEN "All OK" RESET "\n");
    }
    else
    {
        printf(RED "Something went wrong" RESET "\n");
    }
    free(token);

    semantic_destroy();

    return EXIT_SUCCESS;
}