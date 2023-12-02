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

int run_control(Token *token, sym_items *items, Control_state sem_rule)
{

    switch (sem_rule)
    {
    case LET:
        state_let(token, items);
        break;
    case VAR:
        state_var(token, items);
        break;
    case VAR_ID:
        state_var_id(token, items);
        break;
    case VAR_TYPE:
        state_var_type(token, items);
        break;
    case VAR_EXP:
        state_var_exp(token, items);
        break;
    case VAR_ADD:
        state_var_add(token, items);
        break;
    case FUNC_ID:
        state_func_id(token, items);
        break;
    case P_NAME:
        state_p_name(token, items);
        break;
    case P_ID:
        state_p_id(token, items);
        break;
    case P_TYPE:
        state_p_type(token, items);
        break;
    case R_TYPE:
        state_r_type(token, items);
        break;
    case FUNC_HEADER_DONE:
        state_func_header_done(token, items);
        break;
    case PUSH_SCOPE:
        state_push_scope(token, items);
        break;
    case POP_SCOPE:
        state_pop_scope(token, items);
        break;
    case R_EXP:
        state_r_exp(token, items);
        break;
    case COND_EXP:
        state_cond_exp(token, items);
        break;
    case LET_IN_IF:
        state_let_in_if(token, items);
        break;
    case FUNC_BODY_DONE:
        state_func_body_done(token, items);
        break;
    case LOAD_IDENTIF:
        state_load_identif(token, items);
        break;
    case IDENTIF_EXP:
        state_identif_exp(token, items);
        break;
    case FUNC_CALL_PSA:
        state_func_call_psa(token, items);
        break;
    default:
        break;
    }

    return 0;
}