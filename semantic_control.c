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
    if (l_operand.expr_type == TYPE_INVALID || r_operand.expr_type == TYPE_INVALID)
    {
        return TYPE_INVALID;
    }

    switch (((char)l_operand.expr_type << 8) | r_operand.expr_type)
    {
    case ((char)TYPE_INT << 8) | TYPE_INT:
        return TYPE_INT;
    case ((char)TYPE_INT << 8) | TYPE_DOUBLE:
        if (l_operand.is_literal)
        {
            // TODO: add call the appropriate retyping function
            generate_instruction(INT2FLOATS);
            DEBUG_PSA_CODE(printf("implicite Int2Double for left operand '%s'\n", l_operand.token_value););
            return TYPE_DOUBLE;
        }
        else
        {
            throw_error(COMPATIBILITY_ERR, l_operand.line_num, "Cannot convert operand '%s' from type Int to Double.", l_operand.token_value);
            return TYPE_INVALID;
        }
    case ((char)TYPE_DOUBLE << 8) | TYPE_INT:
        if (r_operand.is_literal)
        {
            generate_instruction(INT2FLOATS);
            DEBUG_PSA_CODE(printf("impicite Int2Double for right operand '%s'\n", r_operand.token_value););
            return TYPE_DOUBLE;
        }
        else
        {
            throw_error(COMPATIBILITY_ERR, r_operand.line_num, "Cannot convert operand '%s' from type Int to Double.", r_operand.token_value);
            return TYPE_INVALID;
        }
    case ((char)TYPE_DOUBLE << 8) | TYPE_DOUBLE:
        return TYPE_DOUBLE;
    case ((char)TYPE_STRING << 8) | TYPE_STRING:
        return TYPE_STRING;
    case ((char)TYPE_INT_NIL << 8) | TYPE_INT_NIL:
        return TYPE_INT_NIL;
    case ((char)TYPE_DOUBLE_NIL << 8) | TYPE_DOUBLE_NIL:
        return TYPE_DOUBLE_NIL;
    case ((char)TYPE_STRING_NIL << 8) | TYPE_STRING_NIL:
        return TYPE_STRING_NIL;
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
            .preceded_by_nl = false,
            .is_literal = false,
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
                .preceded_by_nl = false,
                .is_literal = false,
            };
        }
        __attribute__((fallthrough));
    case TOKEN_MINUS:
    case TOKEN_MUL:
    case TOKEN_DIV:

        if (canTypeBeNil(l_operand.expr_type) || canTypeBeNil(r_operand.expr_type))
        {
            throw_error(COMPATIBILITY_ERR, l_operand.line_num, "Invalid operand types for operation '%c'.", getOperationChar(operation));
            break;
        }

        // can be (int, int), (int, double), (double, int), (double, double)
        Expression_type type = getTypeCombination(l_operand, r_operand);

        if (type != TYPE_INVALID)
        {
            return (PSA_Token){
                .type = (Token_type)TOKEN_EXPRSN,
                .token_value = "E",
                .expr_type = type,
                .preceded_by_nl = false,
                .is_literal = false,
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
                .preceded_by_nl = false,
                .is_literal = false,
            };
        }
        break;
    // for: <, >, <=, >=
    case TOKEN_LESS:
    case TOKEN_MORE:
    case TOKEN_LESS_EQ:
    case TOKEN_MORE_EQ:
    {
        // can be (int, int), (int, double), (double, int), (double, double), (string, string)
        if (getTypeCombination(l_operand, r_operand) != TYPE_INVALID || (l_operand.expr_type == TYPE_STRING && r_operand.expr_type == TYPE_STRING))
        {
            return (PSA_Token){
                .type = (Token_type)TOKEN_EXPRSN,
                .token_value = "E",
                .expr_type = TYPE_BOOL,
                .preceded_by_nl = false,
                .is_literal = false,
            };
        }

        break;
    }
    // for: ??, &&, ||
    case TOKEN_AND:
    case TOKEN_OR:
        if (l_operand.expr_type == TYPE_BOOL && r_operand.expr_type == TYPE_BOOL)
        {
            return (PSA_Token){
                .type = (Token_type)TOKEN_EXPRSN,
                .token_value = "E",
                .expr_type = TYPE_BOOL,
                .preceded_by_nl = false,
                .is_literal = false,
            };
        }
        break;
    // for: !
    case TOKEN_BINARY_OPERATOR:
    {
        bool operand_types_valid = canTypeBeNil(l_operand.expr_type) && !canTypeBeNil(r_operand.expr_type);
        bool operand_types_match = removeTypeNil(l_operand.expr_type) == r_operand.expr_type;
        if (operand_types_valid && operand_types_match)
        {
            return (PSA_Token){
                .type = (Token_type)TOKEN_EXPRSN,
                .token_value = "E",
                .expr_type = r_operand.expr_type,
                .preceded_by_nl = false,
                .is_literal = false,
            };
        }
        break;
    }
    default:
        break;
    }

    throw_error(COMPATIBILITY_ERR, l_operand.line_num, "Invalid operand types for operation '%c'.", getOperationChar(operation));

    return (PSA_Token){
        .type = (Token_type)TOKEN_EXPRSN,
        .token_value = "E",
        .expr_type = TYPE_INVALID,
        .preceded_by_nl = false,
        .is_literal = false,
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
