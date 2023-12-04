/**
 * @file gen.h
 * @brief Header for gen.c
 */

#ifndef GEN_H
#define GEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

typedef enum
{
    TOKEN_IF,              // Keyword if 0
    TOKEN_ELSE,            // Keyword else 1
    TOKEN_WHILE,           // Keyword while 2
    TOKEN_RETURN,          // Keyword return 3
    TOKEN_VAR,             // Keyword var 4
    TOKEN_LET,             // Keyword let 5
    TOKEN_BREAK,           // Keyword break 6
    TOKEN_CONTINUE,        // Keyword continue 7
    TOKEN_TYPE_STRING,     // Keyword String 8
    TOKEN_TYPE_INT,        // Keyword Int 9
    TOKEN_TYPE_DOUBLE,     // Keyword Double 10
    TOKEN_TYPE_BOOL,       // Keyword Bool 11
    TOKEN_TYPE_STRING_NIL, // Keyword String? 12
    TOKEN_TYPE_INT_NIL,    // Keyword Int? 13
    TOKEN_TYPE_DOUBLE_NIL, // Keyword Double? 14
    TOKEN_TYPE_BOOL_NIL,   // Keyword Bool? 15
    TOKEN_FUNC,            // Keyword func 16
    TOKEN_IDENTIFICATOR,   // Identificator 17
    TOKEN_EOF,             // EOF 18
    TOKEN_INT,             // Integer 19
    TOKEN_DOUBLE,          // Decimal number 20
    TOKEN_BOOL,            // Bool value 21;
    TOKEN_EXP,             // Exponent 22
    TOKEN_STRING,          // String 23
    TOKEN_EQ,              // Equals == 24
    TOKEN_NEQ,             // Not equal !== 25
    TOKEN_LESS,            // Less than < 26
    TOKEN_MORE,            // More than > 27
    TOKEN_MORE_EQ,         // More than or equal >= 28
    TOKEN_LESS_EQ,         // Less than or equal <= 29
    TOKEN_PLUS,            // Plus + 30
    TOKEN_MINUS,           // Minus - 31
    TOKEN_MUL,             // Multiply * 32
    TOKEN_DIV,             // Divide / 33
    TOKEN_BINARY_OPERATOR, // Binary operator ?? 34
    TOKEN_ASSIGN,          // Assign = 35
    TOKEN_L_BRACKET,       // Left bracket ( 36
    TOKEN_R_BRACKET,       // Right bracket ) 37
    TOKEN_R_CURLY,         // Right bracket } 38
    TOKEN_L_CURLY,         // Left bracket { 39
    TOKEN_COMMA,           // Comma , 40
    TOKEN_ARROW,           // Arrow -> 41
    TOKEN_NIL,             // nil has been read 42
    TOKEN_DOUBLE_DOT,      // : has been read 43
    TOKEN_NOT,             // ! has been read 44
    TOKEN_AND,             // && has been read 45
    TOKEN_OR,              // || has been read 46
    TOKEN_UNDERSCORE,      // _ has been read 47
    TOKEN_FUNC_ID,         // func_id 48
    TOKEN_EXPRSN,          // E 49
    TOKEN_SHIFT,           // < 50
    TOKEN_UNSHIFT,         // > 51
    TOKEN_FUNC_CALL,
} Token_type;

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define RESET "\x1b[0m"

void generate_func_call();
bool cmp_type(Token_type type);
void printIndentation();

bool START();
bool STMT_LIST();
bool LOCAL_STMT_LIST();
bool STMT();
bool LOCAL_STMT();
bool VAR_LET();
bool VAR_SCOPE();
bool TYPE_AND_ASIGN();
bool R_FLEX();
bool D_TYPE();
bool DEF_FUNC();
bool P_LIST();
bool PARAM();
bool P_SEP();
bool RET_TYPE();
bool FUNC_STMT_LIST();
bool FUNC_STMT();
bool RET();
bool FUNC_WHILE();
bool FUNC_IF();
bool FUNC_ELSE_IF();
bool FUNC_ELSE_CLAUSE();
bool FUNC_AFTER_ELSE();
bool IF_STMT();
bool ELSE_IF_STMT();
bool IF_COND();
bool ELSE_CLAUSE();
bool AFTER_ELSE();
bool WHILE_STMT();
bool LOAD_ID();
bool EXP(Token_type type);

#endif // GEN_H