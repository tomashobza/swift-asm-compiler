/**
 * @file scanner.c
 * @brief Header file for lexical analysis of the project
 * @author Simona Valkovská <xvalko12@vutbr.cz>
 *
 * Project: IFJ compiler
 */

#ifndef IFJ2023_SCANNER_H
#define IFJ2023_SCANNER_H

#include "error.h"
#include <stdbool.h>
#include "stack.h"
#include "debug.h"

/**
 * Enumeration to represent different states of the scanner.
 */
typedef enum
{
    NEW_TOKEN,           // 0
    COMMENTARY,          // 1
    COMMENTARY_BL,       // 2
    UNDERSCORE,          // 3
    END_STATE,           // 4
    INTEGER,             // 5
    DEC_POINT,           // 6
    EXP_START,           // 7
    STRING,              // 8
    STRING_ESCAPE,       // 9
    STRING_1,            // 10
    STRING_2,            // 11
    STRING_3,            // 12
    STRING_1_END,        // 13
    STRING_2_END,        // 14
    STRING_BLOCK,        // 15
    STRING_BLOCK_ESCAPE, // 16
    HEX_START_BLOCK,     // 17
    HEX_START,           // 18
    IDENTIFICATOR,       // 19
} Scanner_state;

extern unsigned int line_num;

/**
 * Enumeration to represent different token types.
 */
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
    TOKEN_BOOL,            // Bool value 21
    TOKEN_EXP,             // Exponent 22
    TOKEN_STRING,          // String 23
    TOKEN_EQ,              // Equals == 24
    TOKEN_NEQ,             // Not equal != 25
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
} Token_type;

/**
 * @brief - Used to store token type and token value read from stdin
 * @param type - type of token
 * @param token_value - value of the token
 * @param preceded_by_nl - bool flag to indicate whether the token has been preceded by newline character
 * @param line_num - number of the line in stdin file
 */
typedef struct
{
    Token_type type;
    char *token_value;
    bool preceded_by_nl;
    int line_num;
} Token;

// STACK FUNCTIONS
DECLARE_STACK_FUNCTIONS(Token);

/**
 * @brief - Used to define keywords and builtin functions with their particular tokens
 * @param code - value of the token
 * @param token - token type
 */
typedef struct
{
    const char *code;
    Token_type token;
} Token_map;

/**
 * Declaration of token map of builtin functions and keywords
 */
extern Token_map defined_tokens[];

/**
 * @brief Generates the next token from the input code.
 * This function implements a finite state machine to recognize different tokens in the input code.
 * @param token A pointer to the Token structure where the generated token information will be stored.
 * @param code A dynamically allocated sequence of characters read from stdin.
 * @return Returns an error code if any occurs during lexical analysis.
 */
int generate_token(Token *token, char *code);

/**
 * @brief Function check_lenght is called always when character needs to be added to dynamically allocated
 * char *code which contains all read characters in the same token, if needed the allocated memory is regit resallocated to twice its size
 * @param code_len current count of characters in code
 * @param add count of charactes that need to be added to string code
 * @param code pointer to char * that is being reallocated
 */
void check_length(int *code_len, int add, char **code);

/**
 * @brief Main function for the scanner to obtain the next token in lexical analysis.
 * This function either generates a new token from the input code or retrieves a token from the scanner stack.
 * @param token A pointer to the Token structure where the next token information will be stored.
 * @return Returns 0 if successful, otherwise returns an error code.
 */
int main_scanner(Token *token);

/**
 * @breif function set_token is called after proper token value and type has been read
 * @param next_state state in which will scanner be after reading current token
 * @param val character sequence that contains the value of current token
 * @param type one of Token_type enums that will determine the type of the token
 * @param token address to which we are inserting the values type and code
 * @return returns potential error that could occur during lexical analysis
 */
int set_token(int next_state, char *val, Token_type type, Token *token);

/**
 * @brief function initializes scanner stack
 */
void scanner_init();

/**
 * @brief function pushes token into scanner stack
 * @param token token which is to be pushed into the stack
 */
void return_token(Token token);

/**
 * @brief function frees all allocated memory for scanner_stack
 */
void free_scanner_stack();

#endif // IFJ2023_SCANNER_H