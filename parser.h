#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "error.h"
#include "symtable.h"


void get_token(Token **token);
bool cmp_type(Token **token, Token_type type);

bool START(Token **token);
bool STMT_LIST(Token **token);
bool STMT(Token **token);
bool VAR_LET(Token **token);
bool VAR_SCOPE(Token **token);
bool TYPE_AND_ASIGN(Token **token);
bool R_RIGID(Token **token);
bool D_TYPE(Token **token);
bool R_FLEX(Token **token);
bool DEF_FUNC(Token **token);
bool P_LIST(Token **token);
bool PARAM(Token **token);
bool P_SEP(Token **token);
bool RET_TYPE(Token **token);
bool FUNC_STMT_LIST(Token **token);
bool FUNC_STMT(Token **token);
bool RET(Token **token);
bool AFTER_RET(Token **token);
bool FUNC_WHILE(Token **token);
bool FUNC_IF(Token **token);
bool FUNC_ELSE_CLAUSE(Token **token);
bool FUNC_AFTER_ELSE(Token **token);
bool IF_STMT(Token **token);
bool ELSE_CLAUSE(Token **token);
bool AFTER_ELSE(Token **token);
bool WHILE_STMT(Token **token);
bool LOAD_ID(Token **token);
bool ALL_AFTER_ID(Token **token);
bool IN_P_LIST(Token **token);
bool IN_PARAM(Token **token);
bool TERM(Token **token);
bool VAL(Token **token);
bool IN_SEP(Token **token);

int main_parser(Token *tok);

#endif  // PARSER_H