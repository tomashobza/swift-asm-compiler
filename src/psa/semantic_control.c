/**
 * @file semantic_control.c
 * @author Tomáš Hobza (xhobza03@vutbr.cz)
 * @brief Functions for semantic control of expressions for the PSA.
 * @version 0.1
 * @date 2023-11-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "psa.h"

Expression_type getTypeCombination(PSA_Token l_operand, PSA_Token r_operand)
{
    switch (((char)l_operand.expr_type << 8) | r_operand.expr_type)
    {
    case ((char)TYPE_INT << 8) | TYPE_INT:
        return TYPE_INT;
    case ((char)TYPE_INT << 8) | TYPE_DOUBLE:
        if (l_operand.type != TOKEN_IDENTIFICATOR)
        {
            DEBUG_PSA_CODE(printf("implicite Int2Double for left operand '%s'\n", l_operand.token_value););
            return TYPE_DOUBLE;
        }
        else
        {
            return TYPE_INVALID;
        }
    case ((char)TYPE_DOUBLE << 8) | TYPE_INT:
        if (r_operand.type != TOKEN_IDENTIFICATOR)
        {
            DEBUG_PSA_CODE(printf("impicite Int2Double for right operand '%s'\n", r_operand.token_value););
            return TYPE_DOUBLE;
        }
        else
        {
            return TYPE_INVALID;
        }
    case ((char)TYPE_DOUBLE << 8) | TYPE_DOUBLE:
        return TYPE_DOUBLE;
    default:
        return TYPE_INVALID;
    }
}

PSA_Token getHandleType(PSA_Token l_operand, Token_type operation, PSA_Token r_operand)
{
    if (l_operand.expr_type == TYPE_INVALID || r_operand.expr_type == TYPE_INVALID)
    {
        return (PSA_Token){
            .type = (Token_type)TOKEN_EXPRSN,
            .token_value = "E",
            .expr_type = TYPE_INVALID,
        };
    }

    switch (operation)
    {
    // for: +, -, *, /
    case TOKEN_PLUS:
        // can be (string, string), ...
        if (l_operand.expr_type == TYPE_STRING && r_operand.expr_type == TYPE_STRING && !(canTypeBeNil(l_operand.expr_type) || canTypeBeNil(r_operand.expr_type)))
        {
            return (PSA_Token){
                .type = (Token_type)TOKEN_EXPRSN,
                .token_value = "E",
                .expr_type = TYPE_STRING,
            };
        }
        __attribute__((fallthrough));
    case TOKEN_MINUS:
    case TOKEN_MUL:
    case TOKEN_DIV:

        if (canTypeBeNil(l_operand.expr_type) || canTypeBeNil(r_operand.expr_type))
        {
            return (PSA_Token){
                .type = (Token_type)TOKEN_EXPRSN,
                .token_value = "E",
                .expr_type = TYPE_INVALID,
            };
        }

        // can be (int, int), (int, double), (double, int), (double, double)
        Expression_type type = getTypeCombination(l_operand, r_operand);

        if (type != TYPE_INVALID)
        {
            return (PSA_Token){
                .type = (Token_type)TOKEN_EXPRSN,
                .token_value = "E",
                .expr_type = type,
            };
        }

        break;

    // for: ==, !=
    case TOKEN_EQ:
    case TOKEN_NEQ:

        // can be (int, int), (int, double), (double, int), (double, double), (string, string), (bool, bool)
        if (l_operand.expr_type == r_operand.expr_type || getTypeCombination(l_operand, r_operand) != TYPE_INVALID)
        {
            return (PSA_Token){
                .type = (Token_type)TOKEN_EXPRSN,
                .token_value = "E",
                .expr_type = TYPE_BOOL,
            };
        }
        break;
    // for: <, >, <=, >=
    case TOKEN_LESS:
    case TOKEN_MORE:
    case TOKEN_LESS_EQ:
    case TOKEN_MORE_EQ:
        // can be (int, int), (int, double), (double, int), (double, double), (string, string)
        if (getTypeCombination(l_operand, r_operand) != TYPE_INVALID || (l_operand.expr_type == TYPE_STRING && r_operand.expr_type == TYPE_STRING))
        {
            return (PSA_Token){
                .type = (Token_type)TOKEN_EXPRSN,
                .token_value = "E",
                .expr_type = TYPE_BOOL,
            };
        }
        break;
    // for: ??, &&, ||
    case TOKEN_AND:
    case TOKEN_OR:
        if (l_operand.expr_type == TYPE_BOOL && r_operand.expr_type == TYPE_BOOL)
        {
            return (PSA_Token){
                .type = (Token_type)TOKEN_EXPRSN,
                .token_value = "E",
                .expr_type = TYPE_BOOL,
            };
        }
        break;
    // for: !
    case TOKEN_BINARY_OPERATOR:
        if (canTypeBeNil(l_operand.expr_type) || canTypeBeNil(r_operand.expr_type))
        {
            return (PSA_Token){
                .type = (Token_type)TOKEN_EXPRSN,
                .token_value = "E",
                .expr_type = getTypeCombination(l_operand, r_operand),
            };
        }
        break;
    default:
        break;
    }

    return (PSA_Token){
        .type = (Token_type)TOKEN_EXPRSN,
        .token_value = "E",
        .expr_type = TYPE_INVALID,
    };
}

Expression_type getIdType(PSA_Token id)
{
    symtable_item *found_id = symtable_find_in_stack(id.token_value, sym_st, id.type == TOKEN_FUNC_ID);
    if (found_id == NULL)
    {
        return TYPE_INVALID;
    }

    switch (found_id->type)
    {
    case FUNCTION:
        return found_id->data.func_data->return_type;
    case VARIABLE:
        return found_id->data.var_data->type;
    default:
        return TYPE_INVALID;
    }
}
