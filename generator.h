/**
 * @file generator.h
 * @brief generating project
 * @author Simona Valkovská <xvalko12@vutbr.cz>
 *
 * Project: IFJ compiler
 */

#ifndef IFJ_PROJEKT_GENERATOR_H
#define IFJ_PROJEKT_GENERATOR_H

#include <stdio.h>
#include "error.h"

#define ZERO_INST(name) fprintf(file, "%s\n", name)
#define LABEL_INST(name, label) fprintf(file, "%s %s\n", name, label)
#define ONE_INST(name, var_type, var_val) fprintf(file, "%s %s@%s\n ", name, var_type, var_val)
#define TWO_INST(name, var_type, var_val, var_type_2, var_val_2) fprintf(file, "%s %s@%s %s@%s\n", name, var_type, var_val, var_type_2, var_val_2)
#define THREE_INST(name, var_type, var_val, var_type_2, var_val_2, var_type_3, var_val_3) fprintf(file, "%s %s@%s %s@%s %s@%s\n", name, var_type, var_val, var_type_2, var_val_2, var_type_3, var_val_3)

extern FILE *file;
extern char *frame;

typedef enum {
    // -
    CREATEFRAME,              // 0
    PUSHFRAME,                // 1
    POPFRAME,                 // 2
    RETURN,                   // 3
    CLEARS,                   // 4
    ADDS,                     // 5
    SUBS,                     // 6
    DIVS,                     // 7
    IDIVS,                    // 8
    MULS,                     // 9
    LTS,                      // 10
    EQS,                      // 11
    GTS,                      // 12
    ANDS,                     // 13
    ORS,                      // 14
    NOTS,                     // 15
    INT2FLOATS,               // 16
    FLOAT2INTS,               // 17
    INT2CHARS,                // 18
    STRI2INTS,                // 19
    BREAK,                    // 20
    // <label>
    CALL,                     // 21
    LABEL,                    // 22
    JUMP,                     // 23
    JUMPIFEQS,                // 24
    JUMPIFNEQS,               // 25
    // <var>
    DEFVAR,                   // 26
    POPS,                     // 27
    // <symb>
    PUSHS,                    // 28
    WRITE,                    // 29
    EXIT,                     // 30
    DPRINT,                   // 31
    // <var> <type>
    READ,                     // 32
    // <var> <symb>
    MOVE,                     // 33
    INT2FLOAT,                // 34
    FLOAT2INT,                // 35
    INT2CHAR,                 // 36
    STRI2INT,                 // 37
    STRLEN,                   // 38
    TYPE,                     // 39
    // <var> <symb> <symb>
    ADD,                      // 40
    SUB,                      // 41
    DIV,                      // 42
    IDIV,                     // 43
    MUL,                      // 44
    LT,                       // 45
    GT,                       // 46
    EQ,                       // 47
    AND,                      // 48
    OR,                       // 49
    NOT,                      // 50
    CONCAT,                   // 51
    GETCHAR,                  // 52
    SETCHAR,                  // 53
    // <label> <symb> <symb>
    JUMPIFEQ,                 // 54
    JUMPIFNEQ,                // 55
}           Instruction;


int main();

void define_function(char *name, int param_num, char *param_val[]);

void return_function(char *type, char *value);

void call_function(char *name, int param_num, char *param_val[]);

void assign_func_call(char *frame_var, char *name);

void def_var(char *name, char *val, char *val_type);

void pushs(char *type, char *val);

void assign(char *frame_var_to, char *name_var_to, char *frame_var_from, char *name_var_from);

#endif //IFJ_PROJEKT_GENERATOR_H
