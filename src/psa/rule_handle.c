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
            .is_literal = isTokenLiteral(handle[0].type),
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
                throw_error(VARIABLES_ERR, handle[0].line_num, "Variable '%s' not found!", handle[0].token_value);
            }
        }
        return (PSA_Token){
            .type = (Token_type)TOKEN_EXPRSN,
            .token_value = "E",
            .expr_type = type,
            .is_literal = isTokenLiteral(handle[0].type),
        };
    case RULE_2:
        DEBUG_PSA_CODE(printf_cyan("rule: E -> (E)\n"););
        return (PSA_Token){
            .type = (Token_type)TOKEN_EXPRSN,
            .token_value = "E",
            .expr_type = handle[1].expr_type,
            .is_literal = handle[1].is_literal,
        };
    case RULE_3:
        DEBUG_PSA_CODE(printf_cyan("rule: E -> !E\n"););
        if (handle[1].expr_type == TYPE_BOOL)
        {
            return (PSA_Token){
                .type = (Token_type)TOKEN_EXPRSN,
                .token_value = "E",
                .expr_type = TYPE_BOOL,
                .is_literal = false,
            };
        }

        throw_error(COMPATIBILITY_ERR, handle[1].line_num, "Invalid operand type for operation prefix '!' (not).");
        return (PSA_Token){
            .type = (Token_type)TOKEN_EXPRSN,
            .token_value = "E",
            .expr_type = TYPE_INVALID,
            .is_literal = false,
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
    case RULE_19:
    {
        DEBUG_PSA_CODE(printf_cyan("rule: E -> E!\n"););
        Expression_type type = removeTypeNil(handle[0].expr_type);
        if (type == TYPE_INVALID)
        {
            throw_error(COMPATIBILITY_ERR, handle[1].line_num, "Invalid operand type for operation postfix '!' (forced unwrapping).");
        }
        return (PSA_Token){
            .type = (Token_type)TOKEN_EXPRSN,
            .token_value = "E",
            .expr_type = type,
            .is_literal = false,
        };
    }
    default:
        DEBUG_PSA_CODE(printf_red("rule: EOF\n"););
        throw_error(SYNTACTIC_ERR, handle[1].line_num, "Expression '%s' is not valid.", "TODO: add this"); // TODO: add printing the expression
        return (PSA_Token){
            .type = (Token_type)TOKEN_EOF,
            .token_value = "$",
            .expr_type = TYPE_INVALID,
        };
    }

    throw_error(SYNTACTIC_ERR, handle[1].line_num, "Expression '%s' is not valid.", "TODO: add this"); // TODO: add this

    return (PSA_Token){
        .type = (Token_type)TOKEN_EOF,
        .token_value = "$",
    };
}

PSA_Token *getHandleFromStack(PSA_Token_stack *s, int *i)
{
    PSA_Token *handle = malloc(sizeof(PSA_Token) * s->size);

    *i = 0;
    while (PSA_Token_stack_top(s).type != TOKEN_SHIFT)
    {
        handle[*i] = ((PSA_Token)PSA_Token_stack_pop(s));
        *i = *i + 1;
    }
    (void)PSA_Token_stack_pop(s); // pop the <

    // reverse the array
    for (int j = 0; j < *i / 2; j++)
    {
        PSA_Token tmp = handle[j];
        handle[j] = handle[*i - j - 1];
        handle[*i - j - 1] = tmp;
    }

    return handle;
}
