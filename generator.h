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

/**
 * @brief Returns the format of the variable for IFJcode23.
 *
 * @param var Symbol record of the variable.
 * @return char* - string with the variable in the format for IFJcode23
 */
char *variable_to_ifjcode23(symtable_item *var);

void handle_label_instructions(Instruction inst);
void handle_var_instructions(Instruction inst, int var);
void handle_symb_instructions(Instruction inst, int symb);
void handle_var_symb_instructions(Instruction inst, int var, int symb);
void handle_var_symb_symb_instructions(Instruction inst, int var, int symb1, int symb2);
void handle_var_type_instructions(Instruction inst, int var, int type);
void handle_no_operand_instructions(Instruction inst);
void handle_label_symb_symb_instructions(Instruction inst, int label, int symb1, int symb2);

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
    // Add other cases as necessary
}
}
while (0)

    /**
     * @brief Prints the output code to stdout.
     */
    void print_out_code();

#endif // GENERATOR_H