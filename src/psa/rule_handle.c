/**
 * @file rule_handle.c
 * @author Anastasia Butok (xbutok00@vutbr.cz)
 * @brief Functions for derivation of handles according to set rules in PSA.
 * @version 0.1
 * @date 2023-11-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "psa.h"

// TODO: handle implicit type conversion only for literals

PSA_Token getRule(PSA_Token *handle, unsigned int len)
{
    uint32_t handle_val = handleToUInt32(handle, len);
    /*
        E -> f
        E -> i
        E -> (E)
        E -> !E
        E -> +E
        E -> -E
        E -> E*E
        E -> E/E
        E -> E+E
        E -> E-E
        E -> E==E
        E -> E!=E
        E -> E<E
        E -> E>E
        E -> E<=E
        E -> E>=E
        E -> E??E
    */
    switch (handle_val)
    {
    case RULE_0:
        DEBUG_PSA_CODE(printf_cyan("rule: E -> f\n"););
        return (PSA_Token){
            .type = (Token_type)TOKEN_EXPRSN,
            .token_value = "E",
            .expr_type = handle[0].expr_type,
        };
    case RULE_1a:
    case RULE_1b:
    case RULE_1c:
    case RULE_1d:
    case RULE_1e:
    case RULE_1f:
        DEBUG_PSA_CODE(printf_cyan("rule: E -> i\n"););
        Expression_type type = getTypeFromToken(handle[0].type);
        if (handle[0].type == TOKEN_IDENTIFICATOR)
        {
            symtable_item *found = symtable_find_in_stack(handle[0].token_value, sym_st, true);
            if (found != NULL && found->type == VARIABLE && found->data.var_data != NULL && found->data.var_data->type != TYPE_INVALID)
            {
                type = found->data.var_data->type;
            }
            else
            {
                throw_error(VARIABLES_ERR, "Variable '%s' not found!", handle[0].token_value);
            }
        }
        return (PSA_Token){
            .type = (Token_type)TOKEN_EXPRSN,
            .token_value = "E",
            .expr_type = type,
        };
    case RULE_2:
        DEBUG_PSA_CODE(printf_cyan("rule: E -> (E)\n"););
        return (PSA_Token){
            .type = (Token_type)TOKEN_EXPRSN,
            .token_value = "E",
            .expr_type = handle[1].expr_type,
        };
    case RULE_3:
        DEBUG_PSA_CODE(printf_cyan("rule: E -> !E\n"););
        return (PSA_Token){
            .type = (Token_type)TOKEN_EXPRSN,
            .token_value = "E",
            .expr_type = handle[1].expr_type,
        };
    case RULE_4:
        DEBUG_PSA_CODE(printf_cyan("rule: E -> +E\n"););
        return (PSA_Token){
            .type = (Token_type)TOKEN_EXPRSN,
            .token_value = "E",
            .expr_type = handle[1].expr_type,
        };
    case RULE_5:
        DEBUG_PSA_CODE(printf_cyan("rule: E -> -E\n"););
        return (PSA_Token){
            .type = (Token_type)TOKEN_EXPRSN,
            .token_value = "E",
            .expr_type = handle[1].expr_type,
        };
    case RULE_6:
        DEBUG_PSA_CODE(printf_cyan("rule: E -> E*E\n"););
        return getHandleType(handle[0], handle[1].type, handle[2]);
    case RULE_7:
        DEBUG_PSA_CODE(printf_cyan("rule: E -> E/E\n"););
        return getHandleType(handle[0], handle[1].type, handle[2]);
    case RULE_8:
        DEBUG_PSA_CODE(printf_cyan("rule: E -> E+E\n"););
        return getHandleType(handle[0], handle[1].type, handle[2]);
    case RULE_9:
        DEBUG_PSA_CODE(printf_cyan("rule: E -> E-E\n"););
        return getHandleType(handle[0], handle[1].type, handle[2]);
    case RULE_10:
        DEBUG_PSA_CODE(printf_cyan("rule: E -> E==E\n"););
        return getHandleType(handle[0], handle[1].type, handle[2]);
    case RULE_11:
        DEBUG_PSA_CODE(printf_cyan("rule: E -> E!=E\n"););
        return getHandleType(handle[0], handle[1].type, handle[2]);
    case RULE_12:
        DEBUG_PSA_CODE(printf_cyan("rule: E -> E<E\n"););
        return getHandleType(handle[0], handle[1].type, handle[2]);
    case RULE_13:
        DEBUG_PSA_CODE(printf_cyan("rule: E -> E>E\n"););
        return getHandleType(handle[0], handle[1].type, handle[2]);
    case RULE_14:
        DEBUG_PSA_CODE(printf_cyan("rule: E -> E<=E\n"););
        return getHandleType(handle[0], handle[1].type, handle[2]);
    case RULE_15:
        DEBUG_PSA_CODE(printf_cyan("rule: E -> E>=E\n"););
        return getHandleType(handle[0], handle[1].type, handle[2]);
    case RULE_16:
        DEBUG_PSA_CODE(printf_cyan("rule: E -> E&&E\n"););
        return getHandleType(handle[0], handle[1].type, handle[2]);
    case RULE_17:
        DEBUG_PSA_CODE(printf_cyan("rule: E -> E||E\n"););
        return getHandleType(handle[0], handle[1].type, handle[2]);
    case RULE_18:
        DEBUG_PSA_CODE(printf_cyan("rule: E -> E??E\n"););
        return getHandleType(handle[0], handle[1].type, handle[2]);
    default:
        DEBUG_PSA_CODE(printf_red("rule: EOF\n"););
        return (PSA_Token){
            .type = (Token_type)TOKEN_EOF,
            .token_value = "$",
            .expr_type = TYPE_INVALID,
        };
    }

    return (PSA_Token){
        .type = (Token_type)TOKEN_EOF,
        .token_value = "$",
    };
}
