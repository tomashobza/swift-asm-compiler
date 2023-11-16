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

int main_parser(Token *tok);

#endif  // PARSER_H