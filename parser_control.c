/**
 * @file parser_controlr.c
 * @author Jakub Všetečka <xvsete00@vutbr.cz>
 * @brief Parser controller.
 * @version 0.1
 * @date 2023-11-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "parser.h"

void run_parser()
{
    Token *token = malloc(sizeof(Token));

    symtable s_tb = symtable_init();
    symtable_stack_push(sym_st, s_tb);
    sym_items *items = malloc(sizeof(sym_items));
    items->funcItem = init_symtable_item(true);
    items->varItem = init_symtable_item(false);

    add_builtin_functions(items);

    get_token(token);

    bool all_ok = START(token, items);
    if (all_ok)
    {
        DEBUG_SYNTAX_CODE(printf(GREEN "\nAll OK" RESET "\n"););
    }
    else
    {
        throw_error(SYNTACTIC_ERR, token->line_num, RED "Unexpected token: '%s'!" RESET "\n", token->token_value);
    }
    free(token);
}

Token defined_var = (Token){
    .type = TOKEN_EOF,
    .token_value = "ERROE",
};

int run_control(Token *token, sym_items *items, Control_state sem_rule)
{
    switch (sem_rule)
    {
    case LET:
        sem_let(token, items);
        break;
    case VAR:
        sem_var(token, items);

        break;
    case VAR_ID:
        sem_var_id(token, items);

        fprintf(out_code_file, "# variable definition\n");

        defined_var = *token;
        generate_instruction(DEFVAR, variable(token->token_value, sym_st->size - 1, true));
        fprintf(out_code_file, "\n");

        break;
    case VAR_TYPE:
        sem_var_type(token, items);
        break;
    case VAR_EXP:
        sem_var_exp(token, items);

        fprintf(out_code_file, "# variable initialization\n");

        generate_instruction(POPS, variable(defined_var.token_value, sym_st->size - 1, true));
        fprintf(out_code_file, "\n");

        break;
    case VAR_ADD:
        sem_var_add(token, items);
        break;
    case FUNC_ID:
        sem_func_id(token, items);
        break;
    case P_NAME:
        sem_p_name(token, items);
        break;
    case P_ID:
        sem_p_id(token, items);
        break;
    case P_TYPE:
        sem_p_type(token, items);
        break;
    case R_TYPE:
        sem_r_type(token, items);
        break;
    case FUNC_HEADER_DONE:

        fprintf(out_code_file, "# function header\n");
        sem_func_header_done(token, items);
        generate_func_header(*(items->funcItem));
        break;
    case PUSH_SCOPE:
        sem_push_scope(token, items);
        break;
    case POP_SCOPE:
        sem_pop_scope(token, items);
        break;
    case R_EXP:
    {
        sem_r_exp(token, items);

        fprintf(out_code_file, "# return\n");

        generate_instruction(DEFVAR, variable("retval", 1, false));
        generate_instruction(POPS, variable("retval", 1, false));
        generate_instruction(POPFRAME);
        generate_instruction(RETURN);
        fprintf(out_code_file, "\n");
    }
    break;
    case COND_EXP:
        sem_cond_exp(token, items);
        break;
    case LET_IN_IF:
        sem_let_in_if(token, items);
        break;
    case FUNC_BODY_DONE:
        sem_func_body_done(token, items);

        fprintf(out_code_file, "# function end\n");
        generate_func_end(*(items->funcItem));
        break;
    case LOAD_IDENTIF:
        sem_load_identif(token, items);

        defined_var = *token;

        break;
    case IDENTIF_EXP:
        sem_identif_exp(token, items);

        fprintf(out_code_file, "# variable assigment\n");

        generate_instruction(POPS, variable(defined_var.token_value, sym_st->size - 1, true));
        fprintf(out_code_file, "\n");

        break;
    case FUNC_CALL_PSA:
        sem_func_call_psa(token, items);
        break;
    default:
        break;
    }

    return 0;
}