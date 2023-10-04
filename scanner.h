//TODO check comments
/**
 * @file scanner.c
 * @brief Scanner of project
 * @author Simona Valkovská
 *
 * Project: IFJ compiler
 */
#ifndef IFJ2022_SCANNER_H
#define IFJ2022_SCANNER_H
#include "error.h"
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
#define OCT_START 411
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
    TOKEN_KEYWORD, // Keyword 0
    TOKEN_FUNC, // Keyword func 1
    TOKEN_TYPE_ID, // ID of a type id 2
    TOKEN_TYPE_SUFFIX, // Prefix of type id ? 3
    TOKEN_IDENTIFICATOR, // Variable id 4
    TOKEN_EOL, // EOL 5
    TOKEN_EOF, // EOF 6
    TOKEN_INT, // Integer 7
    TOKEN_DOUBLE, // Decimal number 8
    TOKEN_EXP, // Exponent 9
    TOKEN_STRING, // String 10
    TOKEN_EQ, // Equals == 11
    TOKEN_NEQ, // Not equal !== 12
    TOKEN_LESS, // Less than < 13
    TOKEN_MORE, // More than > 14
    TOKEN_MORE_EQ, // More than or equal >= 15
    TOKEN_LESS_EQ, // Less than or equal <= 16
    TOKEN_PLUS, // Plus + 17
    TOKEN_MINUS, // Minus - 18
    TOKEN_MUL, // Multiply * 19
    TOKEN_DIV, // Divide / 20
    TOKEN_BINARY_OPERATOR, // Binary operator ?? 21
    TOKEN_ASSIGN, // Assign = 22
    TOKEN_L_BRACKET, // Left bracket ( 23
    TOKEN_R_BRACKET, // Right bracket ) 24
    TOKEN_R_CURLY, // Left bracket { 25
    TOKEN_L_CURLY, // Right bracket } 26
    TOKEN_COMMA, // Comma , 27
    TOKEN_ARROW, // Arrow -> 28
    TOKEN_BUILTIN_FUNC, // Builtin function identificator has been read 29
    TOKEN_NIL, // nil has been read 30
    TOKEN_DOUBLE_DOT, // : has been read 31
    TOKEN_NOT, // ! has been read 32
    TOKEN_AND, // && has been read 33
    TOKEN_OR, // || has been read 34
    TOKEN_UNDERSCORE,
}Token_type;
/*
* Structure Token - used to store token type and token value read from stdin
*/
typedef struct {
    int type;
    char *token_value;
}Token;

/**
 * @def function generate_token works based on final state automat of scanner states
 * @param token empty token structure to which read values and determined token type will be saved
 * @param code dynamically allocated sequence of characers read from stdin
 * @return returns error code if any occured during lexical anaysis
*/
int generate_token(Token *token,char *code);

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

#endif //IFJ2022_SCANNER_H