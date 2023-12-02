/**
 * @file generator.h
 * @author Tomáš Hobza (xhobza03@vutbr.cz), Jakub Všetečka (xvsete00@vutbr.cz)
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

extern FILE *out_code_file;

typedef enum {
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
}           Instruction;

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
 * @brief Prints the IFJcode23 instruction without operands to the output file.
 *
 * @param inst Instruction to be printed.
 */
void handle_label_instructions(Instruction inst);

/**
 * @brief Prints the IFJcode23 instruction with one operand to the output file.
 *
 * @param inst Instruction to be printed.
 * @param var Variable to be printed.
 */
void handle_var_instructions(Instruction inst, Token var);

/**
 * @brief Prints the IFJcode23 instruction with one operand to the output file.
 *
 * @param inst
 * @param symb
 */
void handle_symb_instructions(Instruction inst, Token symb);

void handle_var_symb_instructions(Instruction inst, Token var, Token symb);

void handle_var_symb_symb_instructions(Instruction inst, Token var, Token symb1, Token symb2);

void handle_var_type_instructions(Instruction inst, Token var, Token type);

void handle_no_operand_instructions(Instruction inst);

void handle_label_symb_symb_instructions(Instruction inst, Token label, Token symb1, Token symb2);

/**
 * @brief Generates the IFJcode23 instruction based on the given arguments.
 */
#define generate_instruction(INST, ...)                             \
    do                                                              \
    {                                                               \
        switch (INST)                                               \
        {                                                           \
        case CALL:                                                  \
        case LABEL:                                                 \
        case JUMP:                                                  \
        case JUMIFEQS:                                              \
        case JUMPIFNEQS:                                            \
            handle_label_instructions(INST);                        \
            break;                                                  \
        case DEFVAR:                                                \
        case POPS:                                                  \
            handle_var_instructions(INST, __VA_ARGS__);             \
            break;                                                  \
        case PUSHS:                                                 \
        case WRITE:                                                 \
        case EXIT:                                                  \
        case DPRINT:                                                \
            handle_symb_instructions(INST, __VA_ARGS__);            \
            break;                                                  \
        case MOVE:                                                  \
        case INT2FLOAT:                                             \
        case FLOAT2INT:                                             \
        case INT2CHAR:                                              \
        case STRI2INT:                                              \
        case STRLEN:                                                \
        case TYPE:                                                  \
            handle_var_symb_instructions(INST, __VA_ARGS__);        \
            break;                                                  \
        case ADD:                                                   \
        case SUB:                                                   \
        case MUL:                                                   \
        case DIV:                                                   \
        case IDIV:                                                  \
        case LT:                                                    \
        case GT:                                                    \
        case EQ:                                                    \
        case AND:                                                   \
        case OR:                                                    \
        case NOT:                                                   \
        case CONCAT:                                                \
        case GETCHAT:                                               \
        case SETCHAR:                                               \
            handle_var_symb_symb_instructions(INST, __VA_ARGS__);   \
            break;                                                  \
        case READ:                                                  \
            handle_var_type_instructions(INST, __VA_ARGS__);        \
            break;                                                  \
        case CREATEFRAME:                                           \
        case PUSHFRAME:                                             \
        case POPFRAME:                                              \
        case CLEARS:                                                \
        case ADDS:                                                  \
        case SUBS:                                                  \
        case MULS:                                                  \
        case DIVS:                                                  \
        case IDIVS:                                                 \
        case LTS:                                                   \
        case EQS:                                                   \
        case GTS:                                                   \
        case ANDS:                                                  \
        case ORS:                                                   \
        case NOTS:                                                  \
        case INT2FLOAT2:                                            \
        case FLOAT2INTS:                                            \
        case INT2CHARS:                                             \
        case STRI2INTS:                                             \
        case BREAK:                                                 \
            handle_no_operand_instructions(INST);                   \
            break;                                                  \
        case JUMPIFEQ:                                              \
        case JUMPIFNEQ:                                             \
            handle_label_symb_symb_instructions(INST, __VA_ARGS__); \
            break;                                                  \
        default:                                                    \
            break;                                                  \
        }                                                           \
    } while (0)

/**
 * @brief Prints the output code to stdout.
 */
void print_out_code();

#endif // GENERATOR_H
