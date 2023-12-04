/**
 * @file parser_utils.c
 * @author Jakub Všetečka <xvsete00@vutbr.cz>
 * @brief Parser utils.
 * @version 0.1
 * @date 2023-12-01
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "parser.h"

void add_builtin_functions(sym_items *items)
{
    // readString() -> String?
    items->funcItem = init_symtable_item(true);
    items->funcItem->id = "readString";
    items->funcItem->data.func_data->return_type = TYPE_STRING_NIL;
    items->funcItem->data.func_data->found_return = true;
    symtable_add(items->funcItem, symtable_stack_top(sym_st));
    // readInt() -> Int?
    items->funcItem = init_symtable_item(true);
    items->funcItem->id = "readInt";
    items->funcItem->data.func_data->return_type = TYPE_INT_NIL;
    items->funcItem->data.func_data->found_return = true;
    symtable_add(items->funcItem, symtable_stack_top(sym_st));
    // readDouble() -> Double?
    items->funcItem = init_symtable_item(true);
    items->funcItem->id = "readDouble";
    items->funcItem->data.func_data->return_type = TYPE_DOUBLE_NIL;
    items->funcItem->data.func_data->found_return = true;
    symtable_add(items->funcItem, symtable_stack_top(sym_st));
    // write ( term1 , term2 , …, term𝑛 )
    items->funcItem = init_symtable_item(true);
    items->funcItem->id = "write";
    items->funcItem->data.func_data->return_type = TYPE_EMPTY;
    items->funcItem->data.func_data->found_return = true;
    items->funcItem->data.func_data->params_count = -1;
    symtable_add(items->funcItem, symtable_stack_top(sym_st));
    // Int2Double(_ term ∶ Int) -> Double
    items->funcItem = init_symtable_item(true);
    items->funcItem->id = "Int2Double";
    items->funcItem->data.func_data->return_type = TYPE_DOUBLE;
    items->funcItem->data.func_data->found_return = true;
    // reset param
    add_param(items->funcItem->data.func_data);
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count - 1].name = "_";
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count - 1].id = ""; // TODO check if okay
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count - 1].type = TYPE_INT;
    symtable_add(items->funcItem, symtable_stack_top(sym_st));
    // Double2Int(_ term ∶ Double) -> Int
    items->funcItem = init_symtable_item(true);
    items->funcItem->id = "Double2Int";
    items->funcItem->data.func_data->return_type = TYPE_INT;
    items->funcItem->data.func_data->found_return = true;
    // reset param
    add_param(items->funcItem->data.func_data);
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count - 1].name = "_";
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count - 1].id = ""; // TODO check if okay
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count - 1].type = TYPE_DOUBLE;
    symtable_add(items->funcItem, symtable_stack_top(sym_st));
    // length(_ 𝑠 : String) -> Int
    items->funcItem = init_symtable_item(true);
    items->funcItem->id = "length";
    items->funcItem->data.func_data->return_type = TYPE_INT;
    items->funcItem->data.func_data->found_return = true;
    // reset param
    add_param(items->funcItem->data.func_data);
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count - 1].name = "_";
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count - 1].id = ""; // TODO check if okay
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count - 1].type = TYPE_STRING;
    symtable_add(items->funcItem, symtable_stack_top(sym_st));
    // substring(of 𝑠 : String, startingAt 𝑖 : Int, endingBefore 𝑗 : Int) -> String?
    items->funcItem = init_symtable_item(true);
    items->funcItem->id = "substring";
    items->funcItem->data.func_data->return_type = TYPE_STRING_NIL;
    items->funcItem->data.func_data->found_return = true;
    // reset param
    add_param(items->funcItem->data.func_data);
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count - 1].name = "of";
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count - 1].id = ""; // TODO check if okay
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count - 1].type = TYPE_STRING;
    // reset param
    add_param(items->funcItem->data.func_data);
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count - 1].name = "startingAt";
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count - 1].id = ""; // TODO check if okay
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count - 1].type = TYPE_INT;
    // reset param
    add_param(items->funcItem->data.func_data);
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count - 1].name = "endingBefore";
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count - 1].id = ""; // TODO check if okay
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count - 1].type = TYPE_INT;
    symtable_add(items->funcItem, symtable_stack_top(sym_st));
    // ord(_ 𝑐 : String) -> Int
    items->funcItem = init_symtable_item(true);
    items->funcItem->id = "ord";
    items->funcItem->data.func_data->return_type = TYPE_INT;
    items->funcItem->data.func_data->found_return = true;
    // reset param
    add_param(items->funcItem->data.func_data);
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count - 1].name = "_";
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count - 1].id = ""; // TODO check if okay
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count - 1].type = TYPE_STRING;
    symtable_add(items->funcItem, symtable_stack_top(sym_st));
    // chr(_ 𝑖 : Int) -> String
    items->funcItem = init_symtable_item(true);
    items->funcItem->id = "chr";
    items->funcItem->data.func_data->return_type = TYPE_STRING;
    items->funcItem->data.func_data->found_return = true;
    // reset param
    add_param(items->funcItem->data.func_data);
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count - 1].name = "_";
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count - 1].id = ""; // TODO check if okay
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count - 1].type = TYPE_INT;
    symtable_add(items->funcItem, symtable_stack_top(sym_st));

    DEBUG_SEMANTIC_CODE(symtable_print(symtable_stack_top(sym_st)););
}

Expression_type get_expression_type(Token *token)
{
    switch (token->type)
    {
    case TOKEN_TYPE_INT:
        return TYPE_INT;
        break;
    case TOKEN_TYPE_DOUBLE:
        return TYPE_DOUBLE;
        break;
    case TOKEN_TYPE_STRING:
        return TYPE_STRING;
        break;
    case TOKEN_TYPE_BOOL:
        return TYPE_BOOL;
        break;
    case TOKEN_TYPE_INT_NIL:
        return TYPE_INT_NIL;
        break;
    case TOKEN_TYPE_DOUBLE_NIL:
        return TYPE_DOUBLE_NIL;
        break;
    case TOKEN_TYPE_STRING_NIL:
        return TYPE_STRING_NIL;
        break;
    case TOKEN_TYPE_BOOL_NIL:
        return TYPE_BOOL_NIL;
        break;
    default:
        return TYPE_INVALID;
        break;
    }
}

void print_items(sym_items *items)
{
    printf("FUNC: %p, VAR: %p\n", items->funcItem, items->varItem);
    DEBUG_SEMANTIC_CODE(
        if (items->funcItem != NULL) {
            printf(MAGENTA "FUNCTION: %s, return type: %d, param_count: %d" RESET "\n", items->funcItem->id, items->funcItem->data.func_data->return_type, items->funcItem->data.func_data->params_count - 1);
        } if (items->varItem != NULL) {
            for (int i = 0; i < items->funcItem->data.func_data->params_count - 1; i++)
            {
                printf(MAGENTA "PARAM: %s, id: %s, type: %d" RESET "\n", items->funcItem->data.func_data->params[i].name, items->funcItem->data.func_data->params[i].id, items->funcItem->data.func_data->params[i].type);
            }
            printf(BLUE "VARIABLE: %s, type: %d, is const: %d" RESET "\n", items->varItem->id, items->varItem->data.var_data->type, items->varItem->data.var_data->is_const);
        } printf("\n"););
}

int push_token_get_next(Token *token, Token_stack *token_stack)
{
    Token_stack_push(token_stack, *token);
    return main_scanner(token);
}

bool get_func_definition(Token *token, char *name, symtable_item *psa_item)
{
    Token_stack *token_stack = Token_stack_init();

    bool is_ok = true;

    do
    {
        push_token_get_next(token, token_stack);
    } while (token != NULL && token->type != TOKEN_EOF && (strcmp(token->token_value, name) || token->type != TOKEN_FUNC_ID));

    if (token == NULL || token->type == TOKEN_EOF)
    {
        is_ok = false;
        goto return_tokens;
    }

    psa_item->data.func_data = malloc(sizeof(FunctionData));

    typedef enum
    {
        FUNC_ID,
        L_BRACKET,
        P_LIST,
        PARAM,
        P_NAME,
        P_ID,
        P_TYPE,
        P_SEP,
        R_BRACKET,
        RET_TYPE,
        DONE
    } nstate_t;
    nstate_t nstate = FUNC_ID;
    //  DEF_FUNC -> func func_id ( P_LIST ) RET_TYPE { FUNC_STMT_LIST }
    while (nstate != DONE)
    {
        switch (nstate)
        {
        case FUNC_ID:
            if (token->type == TOKEN_FUNC_ID)
            {
                nstate = L_BRACKET;
                psa_item->id = token->token_value;
            }
            else
            {
                is_ok = false;
                goto return_tokens;
            }
            push_token_get_next(token, token_stack);
            break;
        case L_BRACKET:
            if (token->type == TOKEN_L_BRACKET)
            {
                nstate = P_LIST;
            }
            else
            {
                is_ok = false;
                goto return_tokens;
            }
            push_token_get_next(token, token_stack);
            break;
        case P_LIST:
            if (token->type == TOKEN_R_BRACKET)
            {
                nstate = R_BRACKET;
            }
            else if (token->type == TOKEN_IDENTIFICATOR)
            {
                nstate = PARAM;
            }
            else
            {
                is_ok = false;
                goto return_tokens;
            }
            // push_token_get_next(token, token_stack);
            break;
        case PARAM:
        {
            add_param(psa_item->data.func_data);                                                                        // new psa param
            ParamData *new_psa_param = &(psa_item->data.func_data->params[psa_item->data.func_data->params_count - 1]); // TODO
            if (token->type == TOKEN_IDENTIFICATOR || token->type == TOKEN_UNDERSCORE)
            {
                new_psa_param->name = token->token_value;
                push_token_get_next(token, token_stack);
                if (token->type == TOKEN_IDENTIFICATOR)
                {
                    new_psa_param->id = token->token_value;
                    push_token_get_next(token, token_stack);
                    if (token->type == TOKEN_DOUBLE_DOT)
                    {
                        push_token_get_next(token, token_stack);
                        if (token->type == TOKEN_TYPE_INT || token->type == TOKEN_TYPE_DOUBLE || token->type == TOKEN_TYPE_STRING || token->type == TOKEN_TYPE_BOOL)
                        {
                            new_psa_param->type = get_expression_type(token);
                            nstate = P_SEP;
                        }
                        else
                        {
                            is_ok = false;
                            goto return_tokens;
                        }
                    }
                    else
                    {
                        is_ok = false;
                        goto return_tokens;
                    }
                }
                else
                {
                    is_ok = false;
                    goto return_tokens;
                }
            }
            else
            {
                is_ok = false;
                goto return_tokens;
            }
            push_token_get_next(token, token_stack);
            break;
        }
        case P_SEP:
            if (token->type == TOKEN_COMMA)
            {
                nstate = PARAM;
                push_token_get_next(token, token_stack);
            }
            else if (token->type == TOKEN_R_BRACKET)
            {
                nstate = R_BRACKET;
                push_token_get_next(token, token_stack);
            }
            else
            {
                is_ok = false;
                goto return_tokens;
            }
            break;
        case R_BRACKET:
            if (token->type == TOKEN_ARROW)
            {
                nstate = RET_TYPE;
            }
            else if (token->type == TOKEN_L_CURLY)
            {
                nstate = DONE;
            }
            else
            {
                is_ok = false;
                goto return_tokens;
            }
            push_token_get_next(token, token_stack);
            break;
        case RET_TYPE:
            if (token->type == TOKEN_TYPE_INT || token->type == TOKEN_TYPE_DOUBLE || token->type == TOKEN_TYPE_STRING || token->type == TOKEN_TYPE_BOOL)
            {
                psa_item->data.func_data->return_type = get_expression_type(token);
                nstate = DONE;
            }
            else
            {
                is_ok = false;
                goto return_tokens;
            }
            push_token_get_next(token, token_stack);
            break;
        default:
            is_ok = false;
            goto return_tokens;
        }
    }

return_tokens:
    // return tokens to scanner
    return_token(*token);
    while (token_stack->size > 1)
    {
        *token = Token_stack_pop(token_stack);
        return_token(*token);
    }

    return is_ok;
}

bool check_ret_values(Expression_type t_exp, Expression_type t_id)
{
    switch (t_exp)
    {
    case TYPE_INT:
        return (t_id == TYPE_INT || t_id == TYPE_INT_NIL);
    case TYPE_DOUBLE:
        return (t_id == TYPE_DOUBLE || t_id == TYPE_DOUBLE_NIL);
    case TYPE_STRING:
        return (t_id == TYPE_STRING || t_id == TYPE_STRING_NIL);
    case TYPE_BOOL:
        return (t_id == TYPE_BOOL || t_id == TYPE_BOOL_NIL);
    case TYPE_INT_NIL:
        return (t_id == TYPE_INT || t_id == TYPE_INT_NIL);
    case TYPE_DOUBLE_NIL:
        return (t_id == TYPE_DOUBLE || t_id == TYPE_DOUBLE_NIL);
    case TYPE_STRING_NIL:
        return (t_id == TYPE_STRING || t_id == TYPE_STRING_NIL);
    case TYPE_BOOL_NIL:
        return (t_id == TYPE_BOOL || t_id == TYPE_BOOL_NIL);
    case TYPE_NIL:
        return (t_id == TYPE_DOUBLE_NIL || t_id == TYPE_STRING_NIL || t_id == TYPE_BOOL_NIL || t_id == TYPE_INT_NIL);
    default:
        return false;
    }
}

bool isTypeConvertable(Expression_type variable_type, Expression_type expression_type, bool is_expression_literal)
{
    bool var_is_double = variable_type == TYPE_DOUBLE || variable_type == TYPE_DOUBLE_NIL;
    bool exprsn_is_int = expression_type == TYPE_INT || expression_type == TYPE_INT_NIL;

    return (var_is_double && exprsn_is_int && is_expression_literal);
}