//TODO check comments
/**
 * @file scanner.c
 * @brief Scanner of project
 * @author Simona Valkovská
 *
 * Project: IFJ compiler
 */
#include "error.h"
#include <stdbool.h>

#ifndef IFJ2023_SCANNER_H
#define IFJ2023_SCANNER_H

/*
 * Declaration of scanners states
 */

#define NEW_TOKEN 1
#define COMMENTARY 20
#define COMMENTARY_BL 21
#define INTEGER 30
#define DEC_POINT 31
#define EXP_START 33
#define STRING 40
#define STRING_ESCAPE 41
#define STRING_1 42
#define STRING_2 43
#define STRING_3 44
#define STRING_1_END 45
#define STRING_2_END 46
#define STRING_BLOCK 47
#define STRING_BLOCK_ESCAPE 48
#define HEX_START_BLOCK 49
#define HEX_START 423
#define IDENTIFICATOR 50
#define UNDERSCORE 51
#define END_STATE 60
/*
 * Declaration of global variables
 * END_TYPE - last index of type keyword from *keyword[]
 */
#define END_TYPE 4

typedef enum {
    TOKEN_IF, // Keyword if 0
    TOKEN_ELSE, // Keyword else 1
    TOKEN_WHILE, // Keyword while 2
    TOKEN_RETURN, // Keyword return 3
    TOKEN_VAR, // Keyword var 4
    TOKEN_LET, // Keyword let 5
    TOKEN_FOR, // Keyword for 6
    TOKEN_IN, // Keyword in 7
    TOKEN_BREAK, // Keyword break 8
    TOKEN_CONTINUE, // Keyword continue 9
    TOKEN_TYPE_STRING, // Keyword String 10
    TOKEN_TYPE_INT, // Keyword Int 11
    TOKEN_TYPE_DOUBLE, // Keyword Double 12
    TOKEN_TYPE_BOOL, // Keyword Bool 13
    TOKEN_FUNC, // Keyword func 14
    TOKEN_TYPE_SUFFIX, // Prefix of type ? 15
    TOKEN_IDENTIFICATOR, // Identificator 16
    TOKEN_EOL, // EOL 17
    TOKEN_EOF, // EOF 18
    TOKEN_INT, // Integer 19
    TOKEN_DOUBLE, // Decimal number 20
    TOKEN_EXP, // Exponent 21
    TOKEN_STRING, // String 22
    TOKEN_EQ, // Equals == 23
    TOKEN_NEQ, // Not equal !== 24
    TOKEN_LESS, // Less than < 25
    TOKEN_MORE, // More than > 26
    TOKEN_MORE_EQ, // More than or equal >= 27
    TOKEN_LESS_EQ, // Less than or equal <= 28
    TOKEN_PLUS, // Plus + 29
    TOKEN_MINUS, // Minus - 30
    TOKEN_MUL, // Multiply * 31
    TOKEN_DIV, // Divide / 32
    TOKEN_BINARY_OPERATOR, // Binary operator ?? 33
    TOKEN_ASSIGN, // Assign = 34
    TOKEN_L_BRACKET, // Left bracket ( 35
    TOKEN_R_BRACKET, // Right bracket ) 36
    TOKEN_R_CURLY, // Left bracket { 37
    TOKEN_L_CURLY, // Right bracket } 38
    TOKEN_COMMA, // Comma , 39
    TOKEN_ARROW, // Arrow -> 40
    TOKEN_NIL, // nil has been read 41
    TOKEN_DOUBLE_DOT, // : has been read 42
    TOKEN_NOT, // ! has been read 43
    TOKEN_AND, // && has been read 44
    TOKEN_OR, // || has been read 45
    TOKEN_UNDERSCORE, // _ has been read 46
    TOKEN_READSTRING, // Builtin function readString 47
    TOKEN_READINT, // Builtin function readInt 48
    TOKEN_READDOUBLE, // Builtin function readDouble 49
    TOKEN_WRITE, // Builtin function write 50
    TOKEN_INT2DOUBLE, //Builtin function Int2Double 51
    TOKEN_DOUBLE2INT, // Builtin function Double2Int 52
    TOKEN_LENGTH, // Builtin function length 53
    TOKEN_SUBSTRING, // Builtin function substring 54
    TOKEN_ORD, // Builtin function ord 55
    TOKEN_CHR, // Builtin function chr 56
    TOKEN_SEMICOLON // ; has been read 57
}Token_type;
/*
* Structure Token - used to store token type and token value read from stdin
*/
typedef struct {
    Token_type type;
    char *token_value;
}Token;

/*
 * Structure Token_map - used to define keywords and builtin functions with their particular tokens
 */
typedef struct {
    const char *code;
    int token;
} Token_map;

/*
 * Declaration of token map of builtin functions and keywords
 */
extern Token_map defined_tokens[]; // Declaration of the variable

/**
 * @def function generate_token works based on final state automat of scanner states
 * @param token empty token structure to which read values and determined token type will be saved
 * @param code `dynamically allocated sequence of characters read from stdin
 * @param exp flag that tells scanner whether to ignore newline character or not
 * @return returns error code if any occurred during lexical analysis
*/
int generate_token(Token *token,char *code, bool exp);

/**
 * @def function check_lenght is called always when character needs to be added to dynamically allocated
 * char *code which contains all read characters in the same token, if needed the allocated memory is reallocated to twice its size
 * @param code_len current count of characters in code
 * @param add count of charactes that need to be added to string code
 * @param code pointer to char * that is being reallocated
*/
void check_length(int *code_len,int add,char *code);

/**
 * @def function main_scanner is called from parser.c after 1 token has already been parsed
 * @param tok address of allocated memory for token values to which read values will be inserted
 * @return returns potential error that could occur during lexical analysis
*/
int main_scanner(Token *tok);

/**
 * @def function set_token is called after proper token value and type has been read
 * @param next_state state in which will scanner be after reading current token
 * @param type one of Token_type enums that will determine the type of the token
 * @param token address to which we are inserting the values type and code
 * @param code character sequence that contains the value of current token
 * @return returns potential error that could occur during lexical analysis
*/
int set_token(int next_state,char *val,Token_type type,Token *token,char *code);

#endif //IFJ2023_SCANNER_H