// TODO check comments
/**
 * @file scanner.c
 * @brief Scanner of project
 * @author Simona Valkovská
 *
 * Project: IFJ compiler
 */

#ifndef IFJ2023_SCANNER_H
#define IFJ2023_SCANNER_H

#include "error.h"
#include <stdbool.h>
#include "stack.h"
#include "debug.h"

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
#define HEX_START 50
#define IDENTIFICATOR 51
#define UNDERSCORE 52
#define END_STATE 60

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
    TOKEN_FUNC,            // Keyword func 12
    TOKEN_TYPE_SUFFIX,     // Prefix of type ? 13
    TOKEN_IDENTIFICATOR,   // Identificator 14
    TOKEN_EOF,             // EOF 15
    TOKEN_INT,             // Integer 16
    TOKEN_DOUBLE,          // Decimal number 17
    TOKEN_BOOL,            // Bool value 18
    TOKEN_EXP,             // Exponent 19
    TOKEN_STRING,          // String 20
    TOKEN_EQ,              // Equals == 21
    TOKEN_NEQ,             // Not equal !== 22
    TOKEN_LESS,            // Less than < 23
    TOKEN_MORE,            // More than > 24
    TOKEN_MORE_EQ,         // More than or equal >= 25
    TOKEN_LESS_EQ,         // Less than or equal <= 26
    TOKEN_PLUS,            // Plus + 27
    TOKEN_MINUS,           // Minus - 28
    TOKEN_MUL,             // Multiply * 29
    TOKEN_DIV,             // Divide / 30
    TOKEN_BINARY_OPERATOR, // Binary operator ?? 31
    TOKEN_ASSIGN,          // Assign = 32
    TOKEN_L_BRACKET,       // Left bracket ( 33
    TOKEN_R_BRACKET,       // Right bracket ) 34
    TOKEN_R_CURLY,         // Right bracket } 35
    TOKEN_L_CURLY,         // Left bracket { 36
    TOKEN_COMMA,           // Comma , 37
    TOKEN_ARROW,           // Arrow -> 38
    TOKEN_NIL,             // nil has been read 39
    TOKEN_DOUBLE_DOT,      // : has been read 40
    TOKEN_NOT,             // ! has been read 41
    TOKEN_AND,             // && has been read 42
    TOKEN_OR,              // || has been read 43
    TOKEN_UNDERSCORE,      // _ has been read 44
    TOKEN_READSTRING,      // Builtin function readString 45
    TOKEN_READINT,         // Builtin function readInt 46
    TOKEN_READDOUBLE,      // Builtin function readDouble 47
    TOKEN_WRITE,           // Builtin function write 48
    TOKEN_INT2DOUBLE,      // Builtin function Int2Double 49
    TOKEN_DOUBLE2INT,      // Builtin function Double2Int 50
    TOKEN_LENGTH,          // Builtin function length 51
    TOKEN_SUBSTRING,       // Builtin function substring 52
    TOKEN_ORD,             // Builtin function ord 53
    TOKEN_CHR,             // Builtin function chr 54
    TOKEN_FUNC_ID,         // func_id 55
    TOKEN_EXPRSN,          // E 56
    TOKEN_SHIFT,           // < 57
    TOKEN_UNSHIFT,         // > 58
} Token_type;
/*
 * Structure Token - used to store token type and token value read from stdin
 */
typedef struct
{
    Token_type type;
    char *token_value;
    bool preceded_by_nl;
} Token;

/*
 * Structure Token_map - used to define keywords and builtin functions with their particular tokens
 */
typedef struct
{
    const char *code;
    int token;
} Token_map;

// STACK FUNCTIONS
DECLARE_STACK_FUNCTIONS(Token);

/**
 * Declaration of token map of builtin functions and keywords
 */
extern Token_map defined_tokens[]; // Declaration of the variable

/**
 * @def function generate_token works based on final state automat of scanner states
 * @param token empty token structure to which read values and determined token type will be saved
 * @param code `dynamically allocated sequence of characters read from stdin
 * @return returns error code if any occurred during lexical analysis
 */
int generate_token(Token *token, char *code);

/**
 * @def function check_lenght is called always when character needs to be added to dynamically allocated
 * char *code which contains all read characters in the same token, if needed the allocated memory is regit resallocated to twice its size
 * @param code_len current count of characters in code
 * @param add count of charactes that need to be added to string code
 * @param code pointer to char * that is being reallocated
 */
void check_length(int *code_len, int add, char *code);

/**
 * @def function main_scanner is called from parser.c after 1 token has already been parsed
 * @param token address of allocated memory for token values to which read values will be inserted
 * @return returns potential error that could occur during lexical analysis
 */
int main_scanner(Token *token);

/**
 * @def function set_token is called after proper token value and type has been read
 * @param next_state state in which will scanner be after reading current token
 * @param val character sequence that contains the value of current token
 * @param type one of Token_type enums that will determine the type of the token
 * @param token address to which we are inserting the values type and code
 * @return returns potential error that could occur during lexical analysis
 */
int set_token(int next_state, char *val, Token_type type, Token *token);

/**
 * @def function initializes scanner stack
 */
void scanner_init();

/**
 * @def function pushes token into scanner stack
 * @param token token which is to be pushed into the stack
 */
void return_token(Token token);

#endif // IFJ2023_SCANNER_H