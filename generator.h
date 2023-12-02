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

int main();

void define_function(char *name, int param_num, char *param_val[]);
void return_function(char *type, char *value);
void call_function(char *name, int param_num, char *param_val[]);
void assign_func_call(char *frame_var, char *name);
void def_var(char *name, char *val, char *val_type);
void pushs(char *type, char *val);
void assign(char *frame_var_to, char *name_var_to, char *frame_var_from, char *name_var_from);

#endif //IFJ_PROJEKT_GENERATOR_H
