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

/**
 * @brief Generates a label operand for IFJcode23.
 *
 * @param name Name of the label.
 * @return char*
 */
char *label(char *name);

/**
 * @brief Generates a type operand for IFJcode23.
 *
 * @param type Type of the expression.
 * @return char*
 */
char *type(Expression_type type);

/**
 * @brief Generates a variable operand for IFJcode23.
 *
 * @param id Token with the id of the variable.
 * @param scope -1 for temporary, 0 for global, 1+ for local
 * @param has_suffix Whether the variable has a suffix with the scope index
 * @return char*
 */
char *variable(char *id, int scope, bool has_suffix);

/**
 * @brief Generates a symbol operand for IFJcode23.
 *
 * @param token Token with type and value.
 * @return char*
 */
char *literal(Token token);

/**
 * @brief Generates a symbol operand for IFJcode23.
 *
 * @param symbol Token with type and value.
 * @return char*
 */
char *symbol(Token symbol);

// TODO: delete unused

/**
 * @brief Returns the format of symb for IFJcode23.
 *
 * @param token Token with type and value.
 * @return char* - string with the variable in the format for IFJcode23
 */
char *symb_resolve(Token token);

/**
 * @brief Returns the format of the literal for IFJcode23.
 *
 * @param token Token record of the literal.
 * @return char* - string with the literal in the format for IFJcode23
 */
char *format_token(Token token);

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
void handle_1_operand_instructions(Instruction inst, char *op1);

/**
 * @brief <var> <symb>/<var> <type>
 *
 * @param inst
 * @param var
 * @param symb
 */
void handle_2_operand_instructions(Instruction inst, char *op1, char *op2);

/**
 * @brief <var> <symb> <symb>/<label> <symb> <symb>
 *
 * @param inst
 * @param var
 * @param symb1
 * @param symb2
 */
void handle_3_operand_instructions(Instruction inst, char *op1, char *op2, char *op3);

/**
 * @brief Based on the number of operands provided, calls the appropriate function to handle the instruction.
 *
 * @param inst Instruction to be handled.
 * @param operands Array of operands.
 * @param operands_count Number of operands in the array.
 */
void processInstruction(Instruction inst, char **operands, int operands_count);

/**
 * @brief Macro that generates the IFJcode23 instruction based on the given arguments.
 */
#define generate_instruction(instruction, ...)                                        \
    do                                                                                \
    {                                                                                 \
        char *operands[] = {__VA_ARGS__};                                             \
        processInstruction(instruction, operands, sizeof(operands) / sizeof(char *)); \
    } while (0)

/**
 * @brief Prints the output code to stdout.
 */
void print_out_code();

/**
 * @brief Generates the header of the IFJcode23 function.
 *
 * @param func_item symtable item of the function.
 */
void generate_func_header(symtable_item func_item);

/**
 * @brief Generates the end of the IFJcode23 function.
 *
 * @param func_item symtable item of the function.
 */
void generate_func_end(symtable_item func_item);

/**
 * @brief Generates the IFJcode23 built-in function call.
 *
 * @param func Token record of the function.
 */
void generate_builtin_func_call(Token func);

/**
 * @brief Generates the IFJcode23 if header.
 */
void generate_if_start();

/**
 * @brief Generates the IFJcode23 the else part of the elseif.
 */
void generate_elseif_else();

/**
 * @brief Generates the IFJcode23 the if part of the elseif.
 */
void generate_elseif_if();

/**
 * @brief Generates the IFJcode23 else header.
 */
void generate_else();

/**
 * @brief Generates the IFJcode23 if end.
 */
void generate_if_end();

/**
 * @brief Generates the IFJcode23 while header.
 */
void generate_while_start();

/**
 * @brief Generates the IFJcode23 while condition.
 */
void generate_while_condition();

/**
 * @brief Generates the IFJcode23 while end.
 *
 */
void generate_while_end();

void generate_implicit_init(symtable_item var_item);

/// UTILITY FUNCTIONS

/**
 * @brief Converts the instruction to string.
 *
 * @param in Instruction to be converted.
 * @return char* - string with the instruction
 */
char *instructionToString(Instruction in);

/**
 * @brief Converts the string to instruction.
 *
 * @param str String to be converted.
 * @return Instruction - instruction
 */
Instruction stringToInstruction(char *str);

/**
 * @brief Checks if the token is a built-in function.
 *
 * @param token Function id token
 * @return true
 * @return false
 */
bool isBuiltInFunction(Token token);

/**
 * @brief Gets the intruction associated with the built-in function.
 *
 * @param token
 * @return char*
 */
char *getBuiltInFunctionName(Token token);

/**
 * @brief Returns the data type of the read function
 *
 * @param token
 * @return Token
 */
Expression_type getReadType(Token token);

/**
 * @brief Replaces the special characters in the string with their escape sequences.
 *
 * @param input
 * @return char*
 */
char *escapeString(char *input);

#endif // GENERATOR_H
