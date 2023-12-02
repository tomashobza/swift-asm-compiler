/**
 * @file generator.c
 * @author Tomáš Hobza (xhobza03@vutbr.cz), Jakub Všetečka (xvsete00@vutbr.cz), Simona Valkovská <xvalko12@vutbr.cz>, Anastasia Butok <xbutok00@vutbr.cz>
 * @brief Generator of IFJcode20.
 * @version 0.1
 * @date 2023-11-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "generator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO: napsat funkci, ktera bere token (token.type je jeho typ a token.token_value je string s jeho hodnotou) a vraci string s jeho hodnotou ve formatu pro IFJcode23

void handle_label_instructions(Instruction inst);
void handle_var_instructions(Instruction inst, Token var);
void handle_symb_instructions(Instruction inst, Token symb);
void handle_var_symb_instructions(Instruction inst, Token var, Token symb);
void handle_var_symb_symb_instructions(Instruction inst, Token var, Token symb1, Token symb2);
void handle_var_type_instructions(Instruction inst, Token var, Token type);
void handle_no_operand_instructions(Instruction inst);
void handle_label_symb_symb_instructions(Instruction inst, Token label, Token symb1, Token symb2);

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

char *format_token_for_IFJcode23(Token *token)
{
    char *formatted_value;

    switch (token->type)
    {
    case TOKEN_IDENTIFICATOR:
    {
        formatted_value = malloc(strlen(token->token_value) + 12); // Including "identifier@" and '\0'
        sprintf(formatted_value, "identifier@%s", token->token_value);
    }
    case TOKEN_INT:
    {
        // Format integer literals with "int@"
        formatted_value = malloc(strlen(token->token_value) + 5); //"int@" and '\0'
        sprintf(formatted_value, "int@%s", token->token_value);
    }
    case TOKEN_DOUBLE:
    {
        // Format floating-point literals with "float@"
        double double_value = atof(token->token_value); // Convert to double
        formatted_value = malloc(sizeof(char) * 60);    // Allocating enough space
        sprintf(formatted_value, "float@%a", double_value);
    }
    case TOKEN_STRING:
    {
        // Format string literals with "string@"
        formatted_value = malloc(strlen(token->token_value) + 8); //"string@" and '\0'
        sprintf(formatted_value, "string@%s", token->token_value);
    }
    case TOKEN_NIL:
    {
        // Format nil with "nil@"
        formatted_value = strdup("nil@");
    }
    default:
    {
        // Format other tokens with their value
        formatted_value = strdup(token->token_value);
    }
    }

    return formatted_value;
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
