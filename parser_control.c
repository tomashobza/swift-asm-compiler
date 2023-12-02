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

#include "parser_control.h"

int run_control(Token *token, Control_state control_state)
{
    printf("token: %d\n", token->type);
    switch (control_state)
    {
    case LET:
        break;
    case VAR:
        break;
    case VAR_ID:
        break;
    case VAR_TYPE:
        break;
    case VAR_EXP:
        break;
    case VAR_ADD:
        break;
    case FUNC_ID:
        break;
    case P_NAME:
        break;
    case P_ID:
        break;
    case P_TYPE:
        break;
    case R_TYPE:
        break;
    case FUNC_HEADER_DONE:
        break;
    case PUSH_SCOPE:
        break;
    case POP_SCOPE:
        break;
    case R_EXP:
        break;
    case COND_EXP:
        break;
    case LET_IN_IF:
        break;
    case FUNC_BODY_DONE:
        break;
    case LOAD_IDENTIF:
        break;
    case IDENTIF_EXP:
        break;
    case FUNC_CALL_PSA:
        break;
    default:
        break;
    }

    return 0;
}