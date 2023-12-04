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

/// COUNTERS

int if_counter = 0;
int while_counter = 0;
int tmp_counter = 0;

/// OPERAND FUNCTIONS

char *label(char *name)
{
    char *label = malloc(sizeof(char) * (strlen(name) + 1));
    sprintf(label, "%s", name);
    return label;
}

char *type(Expression_type type)
{
    char *type_str = malloc(sizeof(char) * 10);
    switch (type)
    {
    case TYPE_INT:
    case TYPE_INT_NIL:
        sprintf(type_str, "int");
        break;
    case TYPE_DOUBLE:
    case TYPE_DOUBLE_NIL:
        sprintf(type_str, "float");
        break;
    case TYPE_STRING:
    case TYPE_STRING_NIL:
        sprintf(type_str, "string");
        break;
    case TYPE_BOOL:
    case TYPE_BOOL_NIL:
        sprintf(type_str, "bool");
        break;
    case TYPE_NIL:
        sprintf(type_str, "nil");
        break;
    case TYPE_INVALID:
        sprintf(type_str, "invalid");
        break;
    default:
        sprintf(type_str, "unknown");
        break;
    }
    return type_str;
}

char *variable(char *id, int scope, bool has_suffix)
{
    char *var_name = malloc(sizeof(char) * (10 + strlen(id)));
    char *scope_prefix = scope == 0 ? "GF" : (scope < 0 ? "TF" : "LF");
    char *suffix = malloc(sizeof(char) * 10);
    sprintf(suffix, "%d", scope);

    sprintf(var_name, "%s@$%s%s", scope_prefix, id, has_suffix ? suffix : "");
    return var_name;
}

char *literal(Token token)
{
    return format_token(token);
}

char *symbol(Token symbol)
{
    return symb_resolve(symbol);
}

/// INSTRUCTION FUNCTIONS

void handle_0_operand_instructions(Instruction inst)
{
    char *instruction = instructionToString(inst);
    fprintf(out_code_file, "%s\n", instruction);
    free(instruction);
}

void handle_1_operand_instructions(Instruction inst, char *op1)
{
    char *instruction = instructionToString(inst);
    fprintf(out_code_file, "%s %s\n", instruction, op1);
    free(instruction);
}

void handle_2_operand_instructions(Instruction inst, char *op1, char *op2)
{
    char *instruction = instructionToString(inst);
    fprintf(out_code_file, "%s %s %s\n", instruction, op1, op2);
    free(instruction);
}

void handle_3_operand_instructions(Instruction inst, char *op1, char *op2, char *op3)
{
    char *instruction = instructionToString(inst);
    fprintf(out_code_file, "%s %s %s %s\n", instruction, op1, op2, op3);
    free(instruction);
}

void processInstruction(Instruction inst, char **operands, int operands_count)
{
    switch (operands_count)
    {
    case 0:
        handle_0_operand_instructions(inst);
        break;
    case 1:
        handle_1_operand_instructions(inst, operands[0]);
        break;
    case 2:
        handle_2_operand_instructions(inst, operands[0], operands[1]);
        break;
    case 3:
        handle_3_operand_instructions(inst, operands[0], operands[1], operands[2]);
        break;
    default:
        throw_error(INTERNAL_ERR, -1, "Invalid number of tokens\n");
        break;
    }
}

/// FORMATTING FUNCTIONS

char *symb_resolve(Token token)
{
    char *var_name = malloc(sizeof(char) * (10 + strlen(token.token_value)));
    switch (token.type)
    {
    case TOKEN_IDENTIFICATOR:
    {
        symtable_item *found = symtable_find_in_stack(token.token_value, sym_st, false);
        if (found == NULL)
        {
            sprintf(var_name, "%s@$%s", "TF", token.token_value);
            break;
        }
        symtable_item item = *found;
        if (item.type == VARIABLE)
        {
            if (item.data.var_data->type == TYPE_INVALID)
            {
                throw_error(INTERNAL_ERR, -1, "Variable '%s' is invalid\n", token.token_value);
            }
            sprintf(var_name, "%s@$%s%d", item.scope == 0 ? "GF" : (item.scope < 0 ? "TF" : "LF"), item.id, item.scope);
            break;
        }
        else if (item.type == FUNCTION)
        {
            sprintf(var_name, "%s", item.id);
            break;
        }
        else
        {
            sprintf(var_name, "%s", token.token_value);
            break;
        }
        break;
    }
    case TOKEN_STRING:
    case TOKEN_INT:
    case TOKEN_DOUBLE:
    case TOKEN_NIL:
    case TOKEN_BOOL:
    {
        free(var_name);
        var_name = format_token(token);
        break;
    }
    default:
        sprintf(var_name, "%s", token.token_value);
        break;
    }
    return var_name;
}

char *format_token(Token token)
{
    char *formatted_value = NULL;

    switch (token.type)
    {
    case TOKEN_IDENTIFICATOR:
        throw_error(INTERNAL_ERR, -1, "Identificator\n");
        break;
    case TOKEN_INT:
    {
        // Format integer literals with "int@"
        formatted_value = malloc(strlen(token.token_value) + 5); //"int@" and '\0'
        sprintf(formatted_value, "int@%s", token.token_value);
        break;
    }
    case TOKEN_DOUBLE:
    {
        // Format floating-point literals with "float@"
        double double_value = atof(token.token_value); // Convert to double
        formatted_value = malloc(sizeof(char) * 60);   // Allocating enough space
        sprintf(formatted_value, "float@%a", double_value);
        break;
    }
    case TOKEN_STRING:
    {
        // Format string literals with "string@"
        formatted_value = malloc(strlen(token.token_value) + 8); //"string@" and '\0'
        sprintf(formatted_value, "string@%s", token.token_value);
        break;
    }
    case TOKEN_BOOL:
    {
        // Format bool literals with "bool@"
        formatted_value = malloc(strlen(token.token_value) + 6); //"bool@" and '\0'
        sprintf(formatted_value, "bool@%s", token.token_value);
        break;
    }
    case TOKEN_NIL:
    {
        // Format nil with "nil@"
        formatted_value = malloc(strlen("nil@") + 1); //"nil@" and '\0'
        sprintf(formatted_value, "nil@");
        break;
    }
    default:
    {
        // Format other tokens with their value
        formatted_value = malloc(strlen(token.token_value) + 1);
        sprintf(formatted_value, "%s", token.token_value);
        break;
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

/// GENERATION FUNCTIONS

void generate_func_header(symtable_item func_item)
{
    char *func_lbl = malloc(sizeof(char) * (strlen(func_item.id) + 5));
    strcpy(func_lbl, func_item.id);

    char *func_end_lbl = malloc(sizeof(char) * (strlen(func_item.id) + 5));
    sprintf(func_end_lbl, "%s_end", func_item.id);

    generate_instruction(JUMP, label(func_end_lbl));
    generate_instruction(LABEL, label(func_lbl));

    fprintf(out_code_file, "\n");

    generate_instruction(PUSHFRAME);

    fprintf(out_code_file, "# function params\n");
    for (int i = func_item.data.func_data->params_count - 1; i >= 0; i--)
    {
        Token param = (Token){
            .type = TOKEN_IDENTIFICATOR,
            .token_value = malloc(sizeof(char) * (strlen(func_item.data.func_data->params[i].id) + 1)),
        };
        strcpy(param.token_value, func_item.data.func_data->params[i].id);

        char *var_name = symbol(param);

        generate_instruction(DEFVAR, var_name);
        generate_instruction(POPS, var_name);

        free(var_name);
    }

    fprintf(out_code_file, "# function params end\n\n");

    free(func_end_lbl);
    free(func_lbl);
}

void generate_func_end(symtable_item func_item)
{
    char *func_lbl;
    func_lbl = malloc(sizeof(char) * (strlen(func_item.id) + 5));
    sprintf(func_lbl, "%s_end", func_item.id);

    generate_instruction(LABEL, label(func_lbl));
    fprintf(out_code_file, "\n");

    free(func_lbl);
}

void generate_builtin_func_call(Token func)
{
    char *tmp_token = malloc(sizeof(char) * 20);
    sprintf(tmp_token, "tmp%d", tmp_counter);
    char *tmp_token_name = variable(tmp_token, -1, false);
    Instruction builting_inst = stringToInstruction(getBuiltInFunctionName(func));

    switch (builting_inst)
    {
    case WRITE:
        generate_instruction(DEFVAR, tmp_token_name);
        generate_instruction(POPS, tmp_token_name);
        generate_instruction(builting_inst, tmp_token_name);
        fprintf(out_code_file, "\n");
        break;
    case READ:
        generate_instruction(DEFVAR, tmp_token_name);
        generate_instruction(READ, tmp_token_name, type(getReadType(func)));
        generate_instruction(PUSHS, tmp_token_name);
        fprintf(out_code_file, "\n");
        break;
    case INT2FLOAT:
        generate_instruction(DEFVAR, tmp_token_name);
        generate_instruction(POPS, tmp_token_name);
        generate_instruction(INT2FLOATS, tmp_token_name, tmp_token_name);
        generate_instruction(PUSHS, tmp_token_name);
        fprintf(out_code_file, "\n");
        break;
    case FLOAT2INT:
        generate_instruction(DEFVAR, tmp_token_name);
        generate_instruction(POPS, tmp_token_name);
        generate_instruction(FLOAT2INTS, tmp_token_name, tmp_token_name);
        generate_instruction(PUSHS, tmp_token_name);
        fprintf(out_code_file, "\n");
        break;
    case STRLEN:
        break;
    default:
        throw_error(INTERNAL_ERR, -1, "Invalid built-in function.\n");
        break;
    }

    tmp_counter++;
}

void generate_if_start()
{
    char *tmp_token = malloc(sizeof(char) * 20);
    sprintf(tmp_token, "tmp%d", tmp_counter);
    char *tmp_op = variable(tmp_token, -1, false);
    char *true_op = literal((Token){
        .type = TOKEN_BOOL,
        .token_value = "true",
    });

    char *if_lbl = malloc(sizeof(char) * 10);
    sprintf(if_lbl, "else%d", if_counter);

    fprintf(out_code_file, "# if%d start\n", if_counter);

    generate_instruction(DEFVAR, tmp_op);
    generate_instruction(POPS, tmp_op);
    generate_instruction(JUMPIFNEQS, label(if_lbl), tmp_op, true_op);
    fprintf(out_code_file, "\n");

    fprintf(out_code_file, "\n");

    free(if_lbl);

    if_counter++;
}

void generate_else()
{
    if_counter--;

    char *if_lbl = malloc(sizeof(char) * 10);
    sprintf(if_lbl, "else%d", if_counter);

    char *endif_lbl = malloc(sizeof(char) * 10);
    sprintf(endif_lbl, "endif%d", if_counter);

    fprintf(out_code_file, "# if%d else\n", if_counter);
    generate_instruction(JUMP, label(endif_lbl));
    generate_instruction(LABEL, label(if_lbl));

    fprintf(out_code_file, "\n");

    free(if_lbl);

    if_counter++;
}

void generate_if_end()
{
    if_counter--;

    fprintf(out_code_file, "# if%d end\n", if_counter);

    char *endif_lbl = malloc(sizeof(char) * 10);
    sprintf(endif_lbl, "endif%d", if_counter);
    generate_instruction(LABEL, label(endif_lbl));

    fprintf(out_code_file, "\n");

    free(endif_lbl);
}

void generate_while_start()
{
    char *while_lbl = malloc(sizeof(char) * 10);
    sprintf(while_lbl, "while%d", while_counter);

    fprintf(out_code_file, "# while%d start\n", while_counter);
    generate_instruction(LABEL, label(while_lbl));

    fprintf(out_code_file, "\n");

    free(while_lbl);

    while_counter++;
}

void generate_while_condition()
{
    while_counter--;

    char *endwhile_lbl = malloc(sizeof(char) * 10);
    sprintf(endwhile_lbl, "endwhile%d", while_counter);

    char *tmp_token = malloc(sizeof(char) * 20);
    sprintf(tmp_token, "tmp%d", tmp_counter);
    char *tmp_op = variable(tmp_token, -1, false);

    char *true_op = literal((Token){
        .type = TOKEN_BOOL,
        .token_value = "true",
    });

    fprintf(out_code_file, "# while%d condition\n", while_counter);

    generate_instruction(DEFVAR, tmp_op);
    generate_instruction(POPS, tmp_op);
    generate_instruction(JUMPIFNEQS, label(endwhile_lbl), tmp_op, true_op);

    fprintf(out_code_file, "\n");

    while_counter++;
}

void generate_while_end()
{
    if_counter--;

    fprintf(out_code_file, "# while%d end\n", if_counter);

    char *endwhile_lbl = malloc(sizeof(char) * 10);
    sprintf(endwhile_lbl, "endwhile%d", while_counter);
    generate_instruction(LABEL, label(endwhile_lbl));

    fprintf(out_code_file, "\n");

    free(endwhile_lbl);
}

/// UTILITY FUNCTIONS
char *instructionToString(Instruction in)
{
    char *instruction = malloc(sizeof(char) * 100);
    switch (in)
    {
    case CREATEFRAME:
        sprintf(instruction, "CREATEFRAME");
        break;
    case PUSHFRAME:
        sprintf(instruction, "PUSHFRAME");
        break;
    case POPFRAME:
        sprintf(instruction, "POPFRAME");
        break;
    case RETURN:
        sprintf(instruction, "RETURN");
        break;
    case CLEARS:
        sprintf(instruction, "CLEARS");
        break;
    case ADDS:
        sprintf(instruction, "ADDS");
        break;
    case SUBS:
        sprintf(instruction, "SUBS");
        break;
    case DIVS:
        sprintf(instruction, "DIVS");
        break;
    case IDIVS:
        sprintf(instruction, "IDIVS");
        break;
    case MULS:
        sprintf(instruction, "MULS");
        break;
    case LTS:
        sprintf(instruction, "LTS");
        break;
    case EQS:
        sprintf(instruction, "EQS");
        break;
    case GTS:
        sprintf(instruction, "GTS");
        break;
    case ANDS:
        sprintf(instruction, "ANDS");
        break;
    case ORS:
        sprintf(instruction, "ORS");
        break;
    case NOTS:
        sprintf(instruction, "NOTS");
        break;
    case INT2FLOATS:
        sprintf(instruction, "INT2FLOATS");
        break;
    case FLOAT2INTS:
        sprintf(instruction, "FLOAT2INTS");
        break;
    case INT2CHARS:
        sprintf(instruction, "INT2CHARS");
        break;
    case STRI2INTS:
        sprintf(instruction, "STRI2INTS");
        break;
    case BREAK:
        sprintf(instruction, "BREAK");
        break;
    case CALL:
        sprintf(instruction, "CALL");
        break;
    case LABEL:
        sprintf(instruction, "LABEL");
        break;
    case JUMP:
        sprintf(instruction, "JUMP");
        break;
    case JUMPIFEQS:
        sprintf(instruction, "JUMPIFEQS");
        break;
    case JUMPIFNEQS:
        sprintf(instruction, "JUMPIFNEQS");
        break;
    case DEFVAR:
        sprintf(instruction, "DEFVAR");
        break;
    case POPS:
        sprintf(instruction, "POPS");
        break;
    case PUSHS:
        sprintf(instruction, "PUSHS");
        break;
    case WRITE:
        sprintf(instruction, "WRITE");
        break;
    case EXIT:
        sprintf(instruction, "EXIT");
        break;
    case DPRINT:
        sprintf(instruction, "DPRINT");
        break;
    case READ:
        sprintf(instruction, "READ");
        break;
    case MOVE:
        sprintf(instruction, "MOVE");
        break;
    case INT2FLOAT:
        sprintf(instruction, "INT2FLOAT");
        break;
    case FLOAT2INT:
        sprintf(instruction, "FLOAT2INT");
        break;
    case INT2CHAR:
        sprintf(instruction, "INT2CHAR");
        break;
    case STRI2INT:
        sprintf(instruction, "STRI2INT");
        break;
    case STRLEN:
        sprintf(instruction, "STRLEN");
        break;
    case TYPE:
        sprintf(instruction, "TYPE");
        break;
    case ADD:
        sprintf(instruction, "ADD");
        break;
    case SUB:
        sprintf(instruction, "SUB");
        break;
    case DIV:
        sprintf(instruction, "DIV");
        break;
    case IDIV:
        sprintf(instruction, "IDIV");
        break;
    case MUL:
        sprintf(instruction, "MUL");
        break;
    case LT:
        sprintf(instruction, "LT");
        break;
    case GT:
        sprintf(instruction, "GT");
        break;
    case EQ:
        sprintf(instruction, "EQ");
        break;
    case AND:
        sprintf(instruction, "AND");
        break;
    case OR:
        sprintf(instruction, "OR");
        break;
    case NOT:
        sprintf(instruction, "NOT");
        break;
    case CONCAT:
        sprintf(instruction, "CONCAT");
        break;
    case GETCHAR:
        sprintf(instruction, "GETCHAR");
        break;
    case SETCHAR:
        sprintf(instruction, "SETCHAR");
        break;
    case JUMPIFEQ:
        sprintf(instruction, "JUMPIFEQ");
        break;
    case JUMPIFNEQ:
        sprintf(instruction, "JUMPIFNEQ");
        break;
    default:
        sprintf(instruction, "UNKNOWN");
        break;
    }

    return instruction;
}

Instruction stringToInstruction(char *str)
{
    if (strcmp(str, "CREATEFRAME") == 0)
    {
        return CREATEFRAME;
    }
    else if (strcmp(str, "PUSHFRAME") == 0)
    {
        return PUSHFRAME;
    }
    else if (strcmp(str, "POPFRAME") == 0)
    {
        return POPFRAME;
    }
    else if (strcmp(str, "RETURN") == 0)
    {
        return RETURN;
    }
    else if (strcmp(str, "CLEARS") == 0)
    {
        return CLEARS;
    }
    else if (strcmp(str, "ADDS") == 0)
    {
        return ADDS;
    }
    else if (strcmp(str, "SUBS") == 0)
    {
        return SUBS;
    }
    else if (strcmp(str, "DIVS") == 0)
    {
        return DIVS;
    }
    else if (strcmp(str, "IDIVS") == 0)
    {
        return IDIVS;
    }
    else if (strcmp(str, "MULS") == 0)
    {
        return MULS;
    }
    else if (strcmp(str, "LTS") == 0)
    {
        return LTS;
    }
    else if (strcmp(str, "EQS") == 0)
    {
        return EQS;
    }
    else if (strcmp(str, "GTS") == 0)
    {
        return GTS;
    }
    else if (strcmp(str, "ANDS") == 0)
    {
        return ANDS;
    }
    else if (strcmp(str, "ORS") == 0)
    {
        return ORS;
    }
    else if (strcmp(str, "NOTS") == 0)
    {
        return NOTS;
    }
    else if (strcmp(str, "INT2FLOATS") == 0)
    {
        return INT2FLOATS;
    }
    else if (strcmp(str, "FLOAT2INTS") == 0)
    {
        return FLOAT2INTS;
    }
    else if (strcmp(str, "INT2CHARS") == 0)
    {
        return INT2CHARS;
    }
    else if (strcmp(str, "STRI2INTS") == 0)
    {
        return STRI2INTS;
    }
    else if (strcmp(str, "BREAK") == 0)
    {
        return BREAK;
    }
    else if (strcmp(str, "CALL") == 0)
    {
        return CALL;
    }
    else if (strcmp(str, "LABEL") == 0)
    {
        return LABEL;
    }
    else if (strcmp(str, "JUMP") == 0)
    {
        return JUMP;
    }
    else if (strcmp(str, "JUMPIFEQS") == 0)
    {
        return JUMPIFEQS;
    }
    else if (strcmp(str, "JUMPIFNEQS") == 0)
    {
        return JUMPIFNEQS;
    }
    else if (strcmp(str, "DEFVAR") == 0)
    {
        return DEFVAR;
    }
    else if (strcmp(str, "POPS") == 0)
    {
        return POPS;
    }
    else if (strcmp(str, "PUSHS") == 0)
    {
        return PUSHS;
    }
    else if (strcmp(str, "WRITE") == 0)
    {
        return WRITE;
    }
    else if (strcmp(str, "EXIT") == 0)
    {
        return EXIT;
    }
    else if (strcmp(str, "DPRINT") == 0)
    {
        return DPRINT;
    }
    else if (strcmp(str, "READ") == 0)
    {
        return READ;
    }
    else if (strcmp(str, "MOVE") == 0)
    {
        return MOVE;
    }
    else if (strcmp(str, "INT2FLOAT") == 0)
    {
        return INT2FLOAT;
    }
    else if (strcmp(str, "FLOAT2INT") == 0)
    {
        return FLOAT2INT;
    }
    else if (strcmp(str, "INT2CHAR") == 0)
    {
        return INT2CHAR;
    }
    else if (strcmp(str, "STRI2INT") == 0)
    {
        return STRI2INT;
    }
    else if (strcmp(str, "STRLEN") == 0)
    {
        return STRLEN;
    }
    else if (strcmp(str, "TYPE") == 0)
    {
        return TYPE;
    }
    else if (strcmp(str, "ADD") == 0)
    {
        return ADD;
    }
    else if (strcmp(str, "SUB") == 0)
    {
        return SUB;
    }
    else if (strcmp(str, "DIV") == 0)
    {
        return DIV;
    }
    else if (strcmp(str, "IDIV") == 0)
    {
        return IDIV;
    }
    else if (strcmp(str, "MUL") == 0)
    {
        return MUL;
    }
    else if (strcmp(str, "LT") == 0)
    {
        return LT;
    }
    else if (strcmp(str, "GT") == 0)
    {
        return GT;
    }
    else if (strcmp(str, "EQ") == 0)
    {
        return EQ;
    }
    else if (strcmp(str, "AND") == 0)
    {
        return AND;
    }
    else if (strcmp(str, "OR") == 0)
    {
        return OR;
    }
    else if (strcmp(str, "NOT") == 0)
    {
        return NOT;
    }
    else if (strcmp(str, "CONCAT") == 0)
    {
        return CONCAT;
    }
    else if (strcmp(str, "GETCHAR") == 0)
    {
        return GETCHAR;
    }
    else if (strcmp(str, "SETCHAR") == 0)
    {
        return SETCHAR;
    }
    else if (strcmp(str, "JUMPIFEQ") == 0)
    {
        return JUMPIFEQ;
    }
    else if (strcmp(str, "JUMPIFNEQ") == 0)
    {
        return JUMPIFNEQ;
    }
    return EMPTY;
}

bool isBuiltInFunction(Token token)
{

    if (strcmp(token.token_value, "readString") == 0)
    {
        return true;
    }
    else if (strcmp(token.token_value, "readInt") == 0)
    {
        return true;
    }
    else if (strcmp(token.token_value, "readDouble") == 0)
    {
        return true;
    }
    else if (strcmp(token.token_value, "write") == 0)
    {
        return true;
    }
    else if (strcmp(token.token_value, "Int2Double") == 0)
    {
        return true;
    }
    else if (strcmp(token.token_value, "Double2Int") == 0)
    {
        return true;
    }
    else if (strcmp(token.token_value, "length") == 0)
    {
        return true;
    }
    else if (strcmp(token.token_value, "substring") == 0)
    {
        return true;
    }
    else if (strcmp(token.token_value, "ord") == 0)
    {
        return true;
    }
    else if (strcmp(token.token_value, "chr") == 0)
    {
        return true;
    }

    return false;
}

char *getBuiltInFunctionName(Token token)
{
    char *name = malloc(sizeof(char) * (strlen(token.token_value) + 1));
    if (strcmp(token.token_value, "readString") == 0)
    {
        strcpy(name, "READ");
    }
    else if (strcmp(token.token_value, "readInt") == 0)
    {
        strcpy(name, "READ");
    }
    else if (strcmp(token.token_value, "readDouble") == 0)
    {
        strcpy(name, "READ");
    }
    else if (strcmp(token.token_value, "write") == 0)
    {
        strcpy(name, "WRITE");
    }
    else if (strcmp(token.token_value, "Int2Double") == 0)
    {
        strcpy(name, "INT2FLOAT");
    }
    else if (strcmp(token.token_value, "Double2Int") == 0)
    {
        strcpy(name, "FLOAT2INT");
    }
    else if (strcmp(token.token_value, "length") == 0)
    {
        strcpy(name, "STRLEN");
    }
    else if (strcmp(token.token_value, "substring") == 0)
    {
        strcpy(name, "TODO_add"); // TODO: add this functionality
    }
    else if (strcmp(token.token_value, "ord") == 0)
    {
        strcpy(name, "TODO_add"); // TODO: add this functionality
    }
    else if (strcmp(token.token_value, "chr") == 0)
    {
        strcpy(name, "TODO_add"); // TODO: add this functionality
    }
    return name;
}

Expression_type getReadType(Token token)
{
    if (strcmp(token.token_value, "readString") == 0)
    {
        return TYPE_STRING;
    }
    else if (strcmp(token.token_value, "readInt") == 0)
    {
        return TYPE_INT;
    }
    else if (strcmp(token.token_value, "readDouble") == 0)
    {
        return TYPE_DOUBLE;
    }
    return TYPE_INVALID;
}