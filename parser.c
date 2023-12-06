/**
 * @file parser.c
 * @brief Parser of project
 * @details This module is used for parsing tokens from scanner and generating code in IFJcode23
 * @author Jakub Všetečka <xvsete00@vutbr.cz>
 *
 * Project: IFJ compiler
 */

#include "parser.h"

/**
 * @def get_token
 * @param pointer to the token structure
 */
void get_token(Token *token)
{
    DEBUG_SYNTAX_CODE(printf(YELLOW "popped: %s" RESET "\n", token->token_value););
    int ret = main_scanner(token);
    DEBUG_LEXER_CODE(printf(RED "lexer: %d\n" RESET, ret););
    switch ((Error_code)ret)
    {
    case LEXICAL_ERR:
        throw_error(LEXICAL_ERR, token->line_num, " ");
        break;
    case INTERNAL_ERR:
        throw_error(INTERNAL_ERR, token->line_num, " ");
    default:
        break;
    }
}

bool cmp_type(Token *token, sym_items *items, Token_type type, Control_state sem_rule)
{
    switch (token->type)
    {
    // Found end of file
    case TOKEN_EOF:
        return type == TOKEN_EOF;
    default:
    {
        // Check if token is correct
        bool result = (token->type == type);

        if (result)
        {
            // If token is correct, run semantic analysis and generate code
            run_control(token, items, sem_rule);
        }
        // Get next token
        get_token(token);
        return result;
    }
    }
}

bool START(Token *token, sym_items *items)
{
    DEBUG_SYNTAX_CODE(printf("START token: %d value: %s\n", token->type, token->token_value););
    switch (token->type)
    {
    // START -> STMT_LIST eof
    case TOKEN_EOF:
    case TOKEN_FUNC:
    case TOKEN_IF:
    case TOKEN_IDENTIFICATOR:
    case TOKEN_FUNC_ID:
    case TOKEN_WHILE:
    case TOKEN_VAR:
    case TOKEN_LET:
        run_control(token, items, STARTING);
        return STMT_LIST(token, items) && cmp_type(token, items, TOKEN_EOF, SEM_NONE);
    default:
        return false;
    }
}

bool STMT_LIST(Token *token, sym_items *items)
{
    DEBUG_SYNTAX_CODE(printf("STMT_LIST token: %d value: %s\n", token->type, token->token_value););
    switch (token->type)
    {
    // STMT_LIST -> eps
    case TOKEN_EOF:
    case TOKEN_R_CURLY:
        return true;
    // STMT_LIST -> STMT STMT_LIST
    case TOKEN_FUNC:
    case TOKEN_IF:
    case TOKEN_IDENTIFICATOR:
    case TOKEN_FUNC_ID:
    case TOKEN_WHILE:
    case TOKEN_VAR:
    case TOKEN_LET:
        return STMT(token, items) && STMT_LIST(token, items);
    default:
        return false;
    }
}

bool LOCAL_STMT_LIST(Token *token, sym_items *items)
{
    DEBUG_SYNTAX_CODE(printf("STMT_LIST token: %d value: %s\n", token->type, token->token_value););
    switch (token->type)
    {
    // STMT_LIST -> eps
    case TOKEN_EOF:
    case TOKEN_R_CURLY:
        return true;
    // STMT_LIST -> STMT STMT_LIST
    case TOKEN_IF:
    case TOKEN_IDENTIFICATOR:
    case TOKEN_FUNC_ID:
    case TOKEN_WHILE:
    case TOKEN_VAR:
    case TOKEN_LET:
        return LOCAL_STMT(token, items) && LOCAL_STMT_LIST(token, items);
    default:
        return false;
    }
}

bool STMT(Token *token, sym_items *items)
{
    DEBUG_SYNTAX_CODE(printf("STMT token: %d value: %s\n", token->type, token->token_value););
    switch (token->type)
    {
    // STMT -> DEF_FUNC
    case TOKEN_FUNC:
        return DEF_FUNC(token, items);
    // STMT -> IF_STMT
    case TOKEN_IF:
    {
        bool res = IF_STMT(token, items);
        run_control(token, items, IF_END);
        return res;
    }
    // STMT -> LOAD_ID
    case TOKEN_IDENTIFICATOR:
    case TOKEN_FUNC_ID:
        return LOAD_ID(token, items);
    // STMT -> WHILE_STMT
    case TOKEN_WHILE:
        return WHILE_STMT(token, items);
    // STMT -> VAR_LET
    case TOKEN_VAR:
        return VAR_LET(token, items);
    // STMT -> VAR_LET
    case TOKEN_LET:
        return VAR_LET(token, items);
    default:
        return false;
    }
}

bool LOCAL_STMT(Token *token, sym_items *items)
{
    DEBUG_SYNTAX_CODE(printf("STMT token: %d value: %s\n", token->type, token->token_value););
    switch (token->type)
    {
    // STMT -> IF_STMT
    case TOKEN_IF:
    {
        bool res = IF_STMT(token, items);
        run_control(token, items, IF_END);
        return res;
    }
    // STMT -> LOAD_ID
    case TOKEN_IDENTIFICATOR:
    case TOKEN_FUNC_ID:
        return LOAD_ID(token, items);
    // STMT -> WHILE_STMT
    case TOKEN_WHILE:
        return WHILE_STMT(token, items);
    // STMT -> VAR_LET
    case TOKEN_VAR:
        return VAR_LET(token, items);
    // STMT -> VAR_LET
    case TOKEN_LET:
        return VAR_LET(token, items);
    default:
        return false;
    }
}

bool VAR_LET(Token *token, sym_items *items)
{
    DEBUG_SYNTAX_CODE(printf("VAR_LET token: %d value: %s\n", token->type, token->token_value););
    switch (token->type)
    {
    // VAR_LET -> VAR_SCOPE id TYPE_AND_ASSIGN
    case TOKEN_VAR:
    case TOKEN_LET:
        return VAR_SCOPE(token, items) && cmp_type(token, items, TOKEN_IDENTIFICATOR, VAR_ID) && TYPE_AND_ASIGN(token, items);
    default:
        return false;
    }
}

bool VAR_SCOPE(Token *token, sym_items *items)
{
    DEBUG_SYNTAX_CODE(printf("VAR_SCOPE token: %d value: %s\n", token->type, token->token_value););
    switch (token->type)
    {
    case TOKEN_VAR:
        return cmp_type(token, items, TOKEN_VAR, VAR);
    case TOKEN_LET:
        return cmp_type(token, items, TOKEN_LET, LET);
    default:
        return false;
    }
}

bool TYPE_AND_ASIGN(Token *token, sym_items *items)
{
    DEBUG_SYNTAX_CODE(printf("TYPE_AND_ASIGN token: %d value: %s\n", token->type, token->token_value););
    switch (token->type)
    {
    // TYPE_AND_ASIGN -> : D_TYPE R_FLEX
    case TOKEN_DOUBLE_DOT:
        return cmp_type(token, items, TOKEN_DOUBLE_DOT, SEM_NONE) && D_TYPE(token, items, VAR_TYPE) && R_FLEX(token, items);
    // TYPE_AND_ASIGN -> = EXP
    case TOKEN_ASSIGN:
        return cmp_type(token, items, TOKEN_ASSIGN, VAR_ASSIGN1) && EXP(token, items, VAR_EXP);
    default:
        return false;
    }
}

bool R_FLEX(Token *token, sym_items *items)
{
    DEBUG_SYNTAX_CODE(printf("R_FLEX token: %d value: %s\n", token->type, token->token_value););
    switch (token->type)
    {
    // R_FLEX -> = EXP
    case TOKEN_ASSIGN:
        return cmp_type(token, items, TOKEN_ASSIGN, VAR_ASSIGN2) && EXP(token, items, VAR_EXP);
    // R_FLEX -> eps
    case TOKEN_EOF:
    case TOKEN_IDENTIFICATOR:
    case TOKEN_FUNC_ID:
    case TOKEN_VAR:
    case TOKEN_LET:
    case TOKEN_FUNC:
    case TOKEN_R_CURLY:
    case TOKEN_RETURN:
    case TOKEN_WHILE:
    case TOKEN_IF:
        run_control(token, items, VAR_ADD);
        return true;
    default:
        return false;
    }
}

bool D_TYPE(Token *token, sym_items *items, Control_state sem_rule)
{
    DEBUG_SYNTAX_CODE(printf("D_TYPE token: %d value: %s\n", token->type, token->token_value););
    switch (token->type)
    {
    case TOKEN_TYPE_STRING:
        return cmp_type(token, items, TOKEN_TYPE_STRING, sem_rule);
    case TOKEN_TYPE_INT:
        return cmp_type(token, items, TOKEN_TYPE_INT, sem_rule);
    case TOKEN_TYPE_DOUBLE:
        return cmp_type(token, items, TOKEN_TYPE_DOUBLE, sem_rule);
    case TOKEN_TYPE_BOOL:
        return cmp_type(token, items, TOKEN_TYPE_BOOL, sem_rule);
    case TOKEN_TYPE_STRING_NIL:
        return cmp_type(token, items, TOKEN_TYPE_STRING_NIL, sem_rule);
    case TOKEN_TYPE_INT_NIL:
        return cmp_type(token, items, TOKEN_TYPE_INT_NIL, sem_rule);
    case TOKEN_TYPE_DOUBLE_NIL:
        return cmp_type(token, items, TOKEN_TYPE_DOUBLE_NIL, sem_rule);
    case TOKEN_TYPE_BOOL_NIL:
        return cmp_type(token, items, TOKEN_TYPE_BOOL_NIL, sem_rule);
    default:
        return false;
    }
}

bool DEF_FUNC(Token *token, sym_items *items)
{
    DEBUG_SYNTAX_CODE(printf("DEF_FUNC token: %d value: %s\n", token->type, token->token_value););
    switch (token->type)
    {
    //  DEF_FUNC -> func func_id ( P_LIST ) RET_TYPE { FUNC_STMT_LIST }
    case TOKEN_FUNC:
        return cmp_type(token, items, TOKEN_FUNC, SEM_NONE) && cmp_type(token, items, TOKEN_FUNC_ID, FUNC_ID) && cmp_type(token, items, TOKEN_L_BRACKET, SEM_NONE) &&
               P_LIST(token, items) && cmp_type(token, items, TOKEN_R_BRACKET, SEM_NONE) && RET_TYPE(token, items) && cmp_type(token, items, TOKEN_L_CURLY, FUNC_HEADER_DONE) &&
               FUNC_STMT_LIST(token, items) && cmp_type(token, items, TOKEN_R_CURLY, FUNC_BODY_DONE);
    default:
        return false;
    }
}

bool P_LIST(Token *token, sym_items *items)
{
    DEBUG_SYNTAX_CODE(printf("P_LIST token: %d value: %s\n", token->type, token->token_value););
    switch (token->type)
    {
    // P_LIST -> PARAM
    case TOKEN_IDENTIFICATOR:
        return PARAM(token, items);
    // P_LIST -> eps
    case TOKEN_R_BRACKET:
        return true;
    default:
        return false;
    }
}

bool PARAM(Token *token, sym_items *items)
{
    DEBUG_SYNTAX_CODE(printf("PARAM token: %d value: %s\n", token->type, token->token_value););
    switch (token->type)
    {
    // PARAM -> id id : D_TYPE SEP
    case TOKEN_IDENTIFICATOR:
        return cmp_type(token, items, TOKEN_IDENTIFICATOR, P_NAME) && cmp_type(token, items, TOKEN_IDENTIFICATOR, P_ID) && cmp_type(token, items, TOKEN_DOUBLE_DOT, SEM_NONE) &&
               D_TYPE(token, items, P_TYPE) && P_SEP(token, items);
    default:
        return false;
    }
}

bool P_SEP(Token *token, sym_items *items)
{
    DEBUG_SYNTAX_CODE(printf("P_SEP token: %d value: %s\n", token->type, token->token_value););
    switch (token->type)
    {
    // P_SEP -> eps
    case TOKEN_R_BRACKET:
        return true;
    // P_SEP -> , PARAM
    case TOKEN_COMMA:
        return cmp_type(token, items, TOKEN_COMMA, SEM_NONE) && PARAM(token, items);
    default:
        return false;
    }
}

bool RET_TYPE(Token *token, sym_items *items)
{
    DEBUG_SYNTAX_CODE(printf("RET_TYPE token: %d value: %s\n", token->type, token->token_value););
    switch (token->type)
    {
    // RET_TYPE -> eps
    case TOKEN_L_CURLY:
        return true;
    // RET_TYPE -> -> D_TYPE
    case TOKEN_ARROW:
        return cmp_type(token, items, TOKEN_ARROW, SEM_NONE) && D_TYPE(token, items, R_TYPE);
    default:
        return false;
    }
}

bool FUNC_STMT_LIST(Token *token, sym_items *items)
{
    DEBUG_SYNTAX_CODE(printf("FUNC_STMT_LIST token: %d value: %s\n", token->type, token->token_value););
    switch (token->type)
    {
    // FUNC_STMT_LIST -> FUNC_STMT FUNC_STMT_LIST
    case TOKEN_IDENTIFICATOR:
    case TOKEN_FUNC_ID:
    case TOKEN_VAR:
    case TOKEN_LET:
    case TOKEN_RETURN:
    case TOKEN_WHILE:
    case TOKEN_IF:
        return FUNC_STMT(token, items) && FUNC_STMT_LIST(token, items);
    // FUNC_STMT_LIST -> eps
    case TOKEN_R_CURLY:
        return true;
    default:
        return false;
    }
}

bool FUNC_STMT(Token *token, sym_items *items)
{
    DEBUG_SYNTAX_CODE(printf("FUNC_STMT token: %d value: %s\n", token->type, token->token_value););
    switch (token->type)
    {
    // FUNC_STMT -> LOAD_ID
    case TOKEN_IDENTIFICATOR:
    case TOKEN_FUNC_ID:
        return LOAD_ID(token, items);
    // FUNC_STMT -> VAR_LET
    case TOKEN_VAR:
        return VAR_LET(token, items);
    case TOKEN_LET:
        return VAR_LET(token, items);
    // FUNC_STMT -> RET
    case TOKEN_RETURN:
        return RET(token, items);
    // FUNC_STMT -> FUNC_WHILE
    case TOKEN_WHILE:
        return FUNC_WHILE(token, items);
    // FUNC_STMT -> FUNC_IF
    case TOKEN_IF:
    {
        bool res = FUNC_IF(token, items);
        run_control(token, items, IF_END);
        return res;
    }
    default:
        return false;
    }
}

bool RET(Token *token, sym_items *items)
{
    DEBUG_SYNTAX_CODE(printf("RET token: %d value: %s\n", token->type, token->token_value););
    switch (token->type)
    {
    // RET -> return EXP
    case TOKEN_RETURN:
        return cmp_type(token, items, TOKEN_RETURN, SEM_NONE) && EXP(token, items, R_EXP);
    default:
        return false;
    }
}

bool FUNC_WHILE(Token *token, sym_items *items)
{
    DEBUG_SYNTAX_CODE(printf("FUNC_WHILE token: %d value: %s\n", token->type, token->token_value););
    switch (token->type)
    {
    // FUNC_WHILE -> while EXP { FUNC_STMT_LIST }
    case TOKEN_WHILE:
        return cmp_type(token, items, TOKEN_WHILE, WHILE_START) && EXP(token, items, COND_EXP) && cmp_type(token, items, TOKEN_L_CURLY, WHILE_COND) &&
               FUNC_STMT_LIST(token, items) && cmp_type(token, items, TOKEN_R_CURLY, WHILE_END);
    default:
        return false;
    }
}

bool FUNC_IF(Token *token, sym_items *items)
{
    DEBUG_SYNTAX_CODE(printf("FUNC_IF token: %d value: %s\n", token->type, token->token_value););
    switch (token->type)
    {
    // 	FUNC_IF -> if IF_COND { FUNC_STMT_LIST } FUNC_ELSE_CLAUSE
    case TOKEN_IF:
        return cmp_type(token, items, TOKEN_IF, FUNC_IF_FOUND) && IF_COND(token, items) && cmp_type(token, items, TOKEN_L_CURLY, IF_START) &&
               FUNC_STMT_LIST(token, items) && cmp_type(token, items, TOKEN_R_CURLY, POP_SCOPE) && FUNC_ELSE_CLAUSE(token, items);
    default:
        return false;
    }
}

bool FUNC_ELSE_IF(Token *token, sym_items *items)
{
    DEBUG_SYNTAX_CODE(printf("FUNC_IF token: %d value: %s\n", token->type, token->token_value););
    switch (token->type)
    {
    // 	FUNC_IF -> if IF_COND { FUNC_STMT_LIST } FUNC_ELSE_CLAUSE
    case TOKEN_IF:
        return cmp_type(token, items, TOKEN_IF, ELSE_IF_START) && IF_COND(token, items) && cmp_type(token, items, TOKEN_L_CURLY, ELSE_IF_AFTER_COND) &&
               FUNC_STMT_LIST(token, items) && cmp_type(token, items, TOKEN_R_CURLY, POP_SCOPE) && FUNC_ELSE_CLAUSE(token, items);
    default:
        return false;
    }
}

bool FUNC_ELSE_CLAUSE(Token *token, sym_items *items)
{
    DEBUG_SYNTAX_CODE(printf("FUNC_ELSE_CLAUSE token: %d value: %s\n", token->type, token->token_value););
    switch (token->type)
    {
    // FUNC_ELSE_CLAUSE -> eps
    case TOKEN_IDENTIFICATOR:
    case TOKEN_FUNC_ID:
    case TOKEN_VAR:
    case TOKEN_LET:
    case TOKEN_R_CURLY:
    case TOKEN_RETURN:
    case TOKEN_WHILE:
    case TOKEN_IF:
        return true;
    // FUNC_ELSE_CLAUSE -> else FUNC_AFTER_ELSE
    case TOKEN_ELSE:
        return cmp_type(token, items, TOKEN_ELSE, FUNC_ELSE) && FUNC_AFTER_ELSE(token, items);
    default:
        return false;
    }
}

bool FUNC_AFTER_ELSE(Token *token, sym_items *items)
{
    DEBUG_SYNTAX_CODE(printf("FUNC_AFTER_ELSE token: %d value: %s\n", token->type, token->token_value););
    switch (token->type)
    {
    // FUNC_AFTER_ELSE -> { FUNC_STMT_LIST }
    case TOKEN_L_CURLY:
        return cmp_type(token, items, TOKEN_L_CURLY, ELSE_START) && FUNC_STMT_LIST(token, items) && cmp_type(token, items, TOKEN_R_CURLY, POP_SCOPE);
    // FUNC_AFTER_ELSE -> FUNC_IF
    case TOKEN_IF:
        return FUNC_ELSE_IF(token, items);
    default:
        return false;
    }
}

bool IF_STMT(Token *token, sym_items *items)
{
    DEBUG_SYNTAX_CODE(printf("IF_STMT token: %d value: %s\n", token->type, token->token_value););
    switch (token->type)
    {
    // IF_STMT -> if EXP { LOCAL_STMT_LIST } ELSE_CLAUSE
    case TOKEN_IF:
        return cmp_type(token, items, TOKEN_IF, SEM_NONE) && IF_COND(token, items) && cmp_type(token, items, TOKEN_L_CURLY, IF_START) &&
               LOCAL_STMT_LIST(token, items) && cmp_type(token, items, TOKEN_R_CURLY, POP_SCOPE) && ELSE_CLAUSE(token, items);
    default:
        return false;
    }
}

bool ELSE_IF_STMT(Token *token, sym_items *items)
{
    DEBUG_SYNTAX_CODE(printf("IF_STMT token: %d value: %s\n", token->type, token->token_value););
    switch (token->type)
    {
    // ELSE_IF_STMT -> if EXP { LOCAL_STMT_LIST } ELSE_CLAUSE
    case TOKEN_IF:
        return cmp_type(token, items, TOKEN_IF, ELSE_IF_START) && IF_COND(token, items) && cmp_type(token, items, TOKEN_L_CURLY, ELSE_IF_AFTER_COND) &&
               LOCAL_STMT_LIST(token, items) && cmp_type(token, items, TOKEN_R_CURLY, POP_SCOPE) && ELSE_CLAUSE(token, items);
    default:
        return false;
    }
}

bool IF_COND(Token *token, sym_items *items)
{
    DEBUG_SYNTAX_CODE(printf("IF_COND token: %d value: %s\n", token->type, token->token_value););
    switch (token->type)
    {
    // IF_COND -> EXP
    case TOKEN_IDENTIFICATOR:
    case TOKEN_FUNC_ID:
    case TOKEN_L_BRACKET:
    case TOKEN_DOUBLE:
    case TOKEN_INT:
    case TOKEN_STRING:
    case TOKEN_BOOL:
    case TOKEN_NIL:
    case TOKEN_EXP:
    case TOKEN_NOT:
        return EXP(token, items, COND_EXP);
    // IF_COND -> let id
    case TOKEN_LET:
        return cmp_type(token, items, TOKEN_LET, SEM_NONE) && cmp_type(token, items, TOKEN_IDENTIFICATOR, LET_IN_IF);
    default:
        return false;
    }
}

bool ELSE_CLAUSE(Token *token, sym_items *items)
{
    DEBUG_SYNTAX_CODE(printf("ELSE_CLAUSE token: %d value: %s\n", token->type, token->token_value););
    switch (token->type)
    {
    // ELSE_CLAUSE -> eps
    case TOKEN_IDENTIFICATOR:
    case TOKEN_FUNC_ID:
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
        return cmp_type(token, items, TOKEN_ELSE, SEM_NONE) && AFTER_ELSE(token, items);
    default:
        return false;
    }
}

bool AFTER_ELSE(Token *token, sym_items *items)
{
    DEBUG_SYNTAX_CODE(printf("AFTER_ELSE token: %d value: %s\n", token->type, token->token_value););
    switch (token->type)
    {
    // AFTER_ELSE -> { LOCAL_STMT_LIST }
    case TOKEN_L_CURLY:
        return cmp_type(token, items, TOKEN_L_CURLY, ELSE_START) && LOCAL_STMT_LIST(token, items) && cmp_type(token, items, TOKEN_R_CURLY, POP_SCOPE);
    // AFTER_ELSE -> IF_STMT
    case TOKEN_IF:
        return ELSE_IF_STMT(token, items);
    default:
        return false;
    }
}

bool WHILE_STMT(Token *token, sym_items *items)
{
    DEBUG_SYNTAX_CODE(printf("WHILE_STMT token: %d value: %s\n", token->type, token->token_value););
    switch (token->type)
    {
    // WHILE_STMT -> while EXP { LOCAL_STMT_LIST }
    case TOKEN_WHILE:
        return cmp_type(token, items, TOKEN_WHILE, WHILE_START) && EXP(token, items, COND_EXP) && cmp_type(token, items, TOKEN_L_CURLY, WHILE_COND) &&
               LOCAL_STMT_LIST(token, items) && cmp_type(token, items, TOKEN_R_CURLY, WHILE_END);
    default:
        return false;
    }
}

bool LOAD_ID(Token *token, sym_items *items)
{
    DEBUG_SYNTAX_CODE(printf("LOAD_ID token: %d value: %s\n", token->type, token->token_value););
    switch (token->type)
    {
    // LOAD_ID -> id = EXP
    case TOKEN_IDENTIFICATOR:
        return cmp_type(token, items, TOKEN_IDENTIFICATOR, LOAD_IDENTIF) && cmp_type(token, items, TOKEN_ASSIGN, SEM_NONE) && EXP(token, items, IDENTIF_EXP);
    // LOAD_ID -> func_id
    case TOKEN_FUNC_ID:
        return EXP(token, items, FUNC_CALL_PSA);
    default:
        return false;
    }
}

bool EXP(Token *token, sym_items *items, Control_state sem_rule)
{
    DEBUG_SYNTAX_CODE(printf("EXP token: %d value: %s\n", token->type, token->token_value););

    // returns token and calls PSA to parser expression
    return_token(*token);
    run_control(token, items, sem_rule);
    get_token(token);
    return true;
}