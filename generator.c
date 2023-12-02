/**
 * @file generator.c
 * @author Tomáš Hobza (xhobza03@vutbr.cz), Jakub Všetečka (xvsete00@vutbr.cz)
 * @brief Generator of IFJcode20.
 * @version 0.1
 * @date 2023-11-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "generator.h"
// TODO: napsat funkci, ktera bere token (token.type je jeho typ a token.token_value je string s jeho hodnotou) a vraci string s jeho hodnotou ve formatu pro IFJcode23

void handle_label_instructions(Instruction inst);
void handle_var_instructions(Instruction inst, int var);
void handle_symb_instructions(Instruction inst, int symb);
void handle_var_symb_instructions(Instruction inst, int var, int symb);
void handle_var_symb_symb_instructions(Instruction inst, int var, int symb1, int symb2);
void handle_var_type_instructions(Instruction inst, int var, int type);
void handle_no_operand_instructions(Instruction inst);
void handle_label_symb_symb_instructions(Instruction inst, int label, int symb1, int symb2);

char *variable_to_ifjcode23(symtable_item *var)
{
    if (var == NULL || var->id == NULL)
    {
        throw_error(INTERNAL_ERR, -1, "Error: variable_to_ifjcode23 got NULL as an argument.\n");
        return NULL;
    }

    char *var_name = malloc(sizeof(char) * (10 + strlen(var->id)));
    sprintf(var_name, "%s@$%s%d", var->scope == 0 ? "GF" : "LF", var->id, var->scope);
    return var_name;
}

void print_out_code()
{
    // Check if out_code_file is NULL
    if (out_code_file == NULL)
    {
        throw_error(INTERNAL_ERR, -1, "Error: out_code_file is not initialized.\n");
        return;
    }

    // Reset the file position to the beginning of the file
    if (fseek(out_code_file, 0, SEEK_SET) != 0)
    {
        throw_error(INTERNAL_ERR, -1, "Error: Failed to seek in out_code_file.\n");
        return;
    }

    int c; // fgetc returns int, not char
    while ((c = fgetc(out_code_file)) != EOF)
    {
        printf("%c", (char)c);
    }
}
