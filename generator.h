/**
 * @file generator.h
 * @author Tomáš Hobza (xhobza03@vutbr.cz), Jakub Všetečka (xvsete00@vutbr.cz), Simona Valkovská <xvalko12@vutbr.cz>, Anastasia Butok <xbutok00@vutbr.cz>
 * @brief Header file for the ifjcode2023 code generator.
 * @version 0.1
 * @date 2023-11-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef GENERATOR_H
#define GENERATOR_H

#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "symtable.h"
#include "scanner.h"

extern FILE *out_code_file;

typedef enum
{
    EMPTY,
    // -
    CREATEFRAME, // 0
    PUSHFRAME,   // 1
    POPFRAME,    // 2
    RETURN,      // 3
    CLEARS,      // 4
    ADDS,        // 5
    SUBS,        // 6
    DIVS,        // 7
    IDIVS,       // 8
    MULS,        // 9
    LTS,         // 10
    EQS,         // 11
    GTS,         // 12
    ANDS,        // 13
    ORS,         // 14
    NOTS,        // 15
    INT2FLOATS,  // 16
    FLOAT2INTS,  // 17
    INT2CHARS,   // 18
    STRI2INTS,   // 19
    BREAK,       // 20
                 // <label>
    CALL,        // 21
    LABEL,       // 22
    JUMP,        // 23
    JUMPIFEQS,   // 24
    JUMPIFNEQS,  // 25
                 // <var>
    DEFVAR,      // 26
    POPS,        // 27
                 // <symb>
    PUSHS,       // 28
    WRITE,       // 29
    EXIT,        // 30
    DPRINT,      // 31
                 // <var> <type>
    READ,        // 32
                 // <var> <symb>
    MOVE,        // 33
    INT2FLOAT,   // 34
    FLOAT2INT,   // 35
    INT2CHAR,    // 36
    STRI2INT,    // 37
    STRLEN,      // 38
    TYPE,        // 39
                 // <var> <symb> <symb>
    ADD,         // 40
    SUB,         // 41
    DIV,         // 42
    IDIV,        // 43
    MUL,         // 44
    LT,          // 45
    GT,          // 46
    EQ,          // 47
    AND,         // 48
    OR,          // 49
    NOT,         // 50
    CONCAT,      // 51
    GETCHAR,     // 52
    SETCHAR,     // 53
                 // <label> <symb> <symb>
    JUMPIFEQ,    // 54
    JUMPIFNEQ,   // 55
} Instruction;

typedef enum {
    OP_VAR,
    OP_LIT,
    OP_LBL,
} OperandType;

typedef struct {
    OperandType type;
    union {
        struct {
            char *id;
            int scope; // -1 for TF, 0 for GF, 1+ for LF
        } var;
        Token lit;
        char *lbl;
    } data;
} Operand;

/**
 * @brief Returns the format of symb for IFJcode23.
 *
 * @param token Token with type and value.
 * @return char* - string with the variable in the format for IFJcode23
 */
char *symb_resolve(Token *token);

/**
 * @brief Returns the format of the literal for IFJcode23.
 *
 * @param token Token record of the literal.
 * @return char* - string with the literal in the format for IFJcode23
 */
char *format_token(Token *token);

/**
 * @brief Returns the format of the literal for IFJcode23.
 *
 * @param token Token record of the literal.
 * @return char* - string with the literal in the format for IFJcode23
 */
char *format_token_for_IFJcode23(Token *token);

/**
 * @brief -
 *
 * @param inst
 */
void handle_0_operand_instructions(Instruction inst);

/**
 * @brief <label>/<var>/<symb>
 *
 * @param inst
 * @param symb
 */
void handle_1_operand_instructions(Instruction inst, Token op1);

/**
 * @brief <var> <symb>/<var> <type>
 *
 * @param inst
 * @param var
 * @param symb
 */
void handle_2_operand_instructions(Instruction inst, Token op1, Token op2);

/**
 * @brief <var> <symb> <symb>/<label> <symb> <symb>
 *
 * @param inst
 * @param var
 * @param symb1
 * @param symb2
 */
void handle_3_operand_instructions(Instruction inst, Token op1, Token op2, Token op3);

/**
 * @brief Based on the number of tokens provided, calls the appropriate function to handle the instruction.
 *
 * @param inst Instruction to be handled.
 * @param tokens Array of tokens.
 * @param tokens_count Number of tokens in the array.
 */
void processInstruction(Instruction inst, Token *tokens, int tokens_count);

/**
 * @brief Macro that generates the IFJcode23 instruction based on the given arguments.
 */
#define generate_instruction(instruction, ...)                                   \
    do                                                                           \
    {                                                                            \
        Token tokens[] = {__VA_ARGS__};                                          \
        processInstruction(instruction, tokens, sizeof(tokens) / sizeof(Token)); \
    } while (0)

/**
 * @brief Converts the instruction to string.
 *
 * @param in Instruction to be converted.
 * @return char* - string with the instruction
 */
char *instructionToString(Instruction in);

/**
 * @brief Prints the output code to stdout.
 */
void print_out_code();

#endif // GENERATOR_H
