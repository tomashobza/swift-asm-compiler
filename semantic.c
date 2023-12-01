/**
 * @file semantic.c
 * @author Jakub Všetečka <xvsete00@vutbr.cz>
 * @brief Semantic analysis.
 * @version 0.1
 * @date 2023-11-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "semantic.h"

symtable mySymtable; // Symtable

void add_builtin_functions(sym_items *items)
{
    // readString() -> String?
    init_symtable_item(true);
    items->funcItem->id = "readString";
    items->funcItem->data.func_data->return_type = TYPE_STRING_NIL;
    items->funcItem->data.func_data->found_return = true;
    symtable_add(items->funcItem, symtable_stack_top(sym_st));
    // readInt() -> Int?
    init_symtable_item(true);
    items->funcItem->id = "readInt";
    items->funcItem->data.func_data->return_type = TYPE_INT_NIL;
    items->funcItem->data.func_data->found_return = true;
    symtable_add(items->funcItem, symtable_stack_top(sym_st));
    // readDouble() -> Double?
    init_symtable_item(true);
    items->funcItem->id = "readDouble";
    items->funcItem->data.func_data->return_type = TYPE_DOUBLE_NIL;
    items->funcItem->data.func_data->found_return = true;
    symtable_add(items->funcItem, symtable_stack_top(sym_st));
    // write ( term1 , term2 , …, term𝑛 )
    init_symtable_item(true);
    items->funcItem->id = "write";
    items->funcItem->data.func_data->return_type = TYPE_EMPTY;
    items->funcItem->data.func_data->found_return = true;
    items->funcItem->data.func_data->params_count = -1;
    symtable_add(items->funcItem, symtable_stack_top(sym_st));
    // Int2Double(_ term ∶ Int) -> Double
    init_symtable_item(true);
    items->funcItem->id = "Int2Double";
    items->funcItem->data.func_data->return_type = TYPE_DOUBLE;
    items->funcItem->data.func_data->found_return = true;
    // reset param
    add_param(items->funcItem->data.func_data);
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count].name = "_";
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count].id = ""; // TODO check if okay
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count].type = TYPE_INT;
    add_param(items->funcItem->data.func_data);
    symtable_add(items->funcItem, symtable_stack_top(sym_st));
    // Double2Int(_ term ∶ Double) -> Int
    init_symtable_item(true);
    items->funcItem->id = "Double2Int";
    items->funcItem->data.func_data->return_type = TYPE_INT;
    items->funcItem->data.func_data->found_return = true;
    // reset param
    add_param(items->funcItem->data.func_data);
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count].name = "_";
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count].id = ""; // TODO check if okay
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count].type = TYPE_DOUBLE;
    add_param(items->funcItem->data.func_data);
    symtable_add(items->funcItem, symtable_stack_top(sym_st));
    // length(_ 𝑠 : String) -> Int
    init_symtable_item(true);
    items->funcItem->id = "length";
    items->funcItem->data.func_data->return_type = TYPE_INT;
    items->funcItem->data.func_data->found_return = true;
    // reset param
    add_param(items->funcItem->data.func_data);
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count].name = "_";
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count].id = ""; // TODO check if okay
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count].type = TYPE_STRING;
    add_param(items->funcItem->data.func_data);
    symtable_add(items->funcItem, symtable_stack_top(sym_st));
    // substring(of 𝑠 : String, startingAt 𝑖 : Int, endingBefore 𝑗 : Int) -> String?
    init_symtable_item(true);
    items->funcItem->id = "substring";
    items->funcItem->data.func_data->return_type = TYPE_STRING_NIL;
    items->funcItem->data.func_data->found_return = true;
    // reset param
    add_param(items->funcItem->data.func_data);
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count].name = "of";
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count].id = ""; // TODO check if okay
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count].type = TYPE_STRING;
    add_param(items->funcItem->data.func_data);
    // reset param
    add_param(items->funcItem->data.func_data);
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count].name = "startingAt";
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count].id = ""; // TODO check if okay
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count].type = TYPE_INT;
    add_param(items->funcItem->data.func_data);
    // reset param
    add_param(items->funcItem->data.func_data);
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count].name = "endingBefore";
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count].id = ""; // TODO check if okay
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count].type = TYPE_INT;
    add_param(items->funcItem->data.func_data);
    symtable_add(items->funcItem, symtable_stack_top(sym_st));
    // ord(_ 𝑐 : String) -> Int
    init_symtable_item(true);
    items->funcItem->id = "ord";
    items->funcItem->data.func_data->return_type = TYPE_INT;
    items->funcItem->data.func_data->found_return = true;
    // reset param
    add_param(items->funcItem->data.func_data);
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count].name = "_";
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count].id = ""; // TODO check if okay
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count].type = TYPE_STRING;
    add_param(items->funcItem->data.func_data);
    symtable_add(items->funcItem, symtable_stack_top(sym_st));
    // chr(_ 𝑖 : Int) -> String
    init_symtable_item(true);
    items->funcItem->id = "chr";
    items->funcItem->data.func_data->return_type = TYPE_STRING;
    items->funcItem->data.func_data->found_return = true;
    // reset param
    add_param(items->funcItem->data.func_data);
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count].name = "_";
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count].id = ""; // TODO check if okay
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count].type = TYPE_INT;
    add_param(items->funcItem->data.func_data);
    symtable_add(items->funcItem, symtable_stack_top(sym_st));

    printf(GREEN);
    DEBUG_SEMANTIC_CODE(symtable_print(symtable_stack_top(sym_st)););
    printf(RESET);
}

int semantic_init()
{
    // symtable init
    DEBUG_SEMANTIC_CODE(printf(RED "PUSH_SCOPE\n" RESET););
    mySymtable = symtable_init();
    symtable_stack_push(sym_st, mySymtable);

    return 0; // tode errors
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

void print_items()
{
    DEBUG_SEMANTIC_CODE(
        printf(MAGENTA "FUNCTION: %s, return type: %d, param_count: %d" RESET "\n", items->funcItem->id, items->funcItem->data.func_data->return_type, items->funcItem->data.func_data->params_count);
        for (int i = 0; i < items->funcItem->data.func_data->params_count; i++) {
            printf(MAGENTA "PARAM: %s, id: %s, type: %d" RESET "\n", items->funcItem->data.func_data->params[i].name, items->funcItem->data.func_data->params[i].id, items->funcItem->data.func_data->params[i].type);
        } printf(BLUE "VARIABLE: %s, type: %d, is const: %d" RESET "\n", items->varItem->id, items->varItem->data.var_data->type, items->varItem->data.var_data->is_const);
        printf("\n"););
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
            ParamData new_psa_param;
            printf("param pointer: %p\n", &new_psa_param); // TODO
            if (token->type == TOKEN_IDENTIFICATOR || token->type == TOKEN_UNDERSCORE)
            {
                new_psa_param.name = token->token_value;
                push_token_get_next(token, token_stack);
                if (token->type == TOKEN_IDENTIFICATOR)
                {
                    new_psa_param.id = token->token_value;
                    push_token_get_next(token, token_stack);
                    if (token->type == TOKEN_DOUBLE_DOT)
                    {
                        push_token_get_next(token, token_stack);
                        if (token->type == TOKEN_TYPE_INT || token->type == TOKEN_TYPE_DOUBLE || token->type == TOKEN_TYPE_STRING || token->type == TOKEN_TYPE_BOOL)
                        {
                            new_psa_param.type = get_expression_type(token);
                            add_param(psa_item->data.func_data); // new psa param
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

void state_let(Token *token, sym_items *items)
{
    printf("%p, %p\n", token, items);
    init_symtable_item(false);
    items->varItem->data.var_data->is_const = true;
}

void state_var(Token *token, sym_items *items)
{
    printf("%p, %p\n", token, items);
    init_symtable_item(false);
    items->varItem->data.var_data->is_const = false;
}

void state_var_id(Token *token, sym_items *items)
{
    printf("%p, %p\n", token, items);
    symtable_item *var_id_item = symtable_find(token->token_value, symtable_stack_top(sym_st), false);
    if (var_id_item != NULL && var_id_item->data.var_data->is_param == false)
    {
        throw_error(FUNCTIONS_ERR, token->line_num, "Variable %s is already defined!", token->token_value)
    }
    else if (var_id_item != NULL && var_id_item->data.var_data->is_param == true)
    {
        items->varItem->data.var_data->is_param = true;
    }
    items->varItem->id = token->token_value;
}

void state_var_type(Token *token, sym_items *items)
{
    printf("%p, %p\n", token, items);
    items->varItem->data.var_data->type = get_expression_type(token);
}

void state_var_exp(Token *token, sym_items *items)
{
    printf("%p, %p\n", token, items);
    items->varItem->data.var_data->is_initialized = true;

    DEBUG_SEMANTIC_CODE(
        (symtable_stack_top(sym_st)););
    psa_return_type return_type = parse_expression();
    DEBUG_SEMANTIC_CODE(print_expression_type(return_type.type););
    if (return_type.is_ok == false)
    {
        if (items->varItem->data.var_data->type == TYPE_EMPTY)
        {
            throw_error(TYPE_ERR, token->line_num, "Unrecognizable type of variable: %s\n", items->varItem->id);
        }
        else
        {
            throw_error(COMPATIBILITY_ERR, token->line_num, "Unrecognizable type of variable: %s\n", items->varItem->id);
        }
    }
    else if (return_type.type == TYPE_NIL)
    {
        if (items->varItem->data.var_data->type == TYPE_EMPTY)
        {
            throw_error(TYPE_ERR, token->line_num, "Couldn't decide the type of %s from type NIL!\n", items->varItem->id);
        }
        else if (items->varItem->data.var_data->type != TYPE_INT_NIL && items->varItem->data.var_data->type != TYPE_STRING_NIL && items->varItem->data.var_data->type != TYPE_DOUBLE_NIL && items->varItem->data.var_data->type != TYPE_BOOL_NIL)
        {
            throw_error(COMPATIBILITY_ERR, token->line_num, "Expression type: %d and type: %d of variable: %s do not match!\n", return_type.type, items->varItem->data.var_data->type, items->varItem->id);
        }
    }
    else if (items->varItem->data.var_data->type == TYPE_EMPTY)
    {
        items->varItem->data.var_data->type = return_type.type;
    }
    else if (!(check_ret_values(return_type.type, items->varItem->data.var_data->type) || isTypeConvertable(items->varItem->data.var_data->type, return_type.type, return_type.is_literal)))
    {
        throw_error(COMPATIBILITY_ERR, token->line_num, "Expression type: %d and type: %d of variable: %s do not match!\n", return_type.type, items->varItem->data.var_data->type, items->varItem->id);
    }
    DEBUG_SEMANTIC_CODE(
        (symtable_stack_top(sym_st)););
    state_var_add(token, items);
}

void state_var_add(Token *token, sym_items *items)
{
    printf("%p, %p\n", token, items);
    if (items->varItem->data.var_data->is_param == false) // new symmbol
    {
        DEBUG_SEMANTIC_CODE(printf(YELLOW "ADDING VAR: %s, type: %d, const: %d\n", items->varItem->id, items->varItem->data.var_data->type, items->varItem->data.var_data->is_const););
        symtable_add(items->varItem, symtable_stack_top(sym_st));
        DEBUG_SEMANTIC_CODE(symtable_print(symtable_stack_top(sym_st)););
    }
    else // is param
    {
        symtable_item *var_add_item = symtable_find_in_stack(items->varItem->id, sym_st, false);
        var_add_item->data.var_data->type = items->varItem->data.var_data->type;
        var_add_item->data.var_data->is_const = items->varItem->data.var_data->is_const;
        var_add_item->data.var_data->is_initialized = items->varItem->data.var_data->is_initialized;
        var_add_item->data.var_data->is_param = false; // is no longer param
    }
}

void state_func_id(Token *token, sym_items *items)
{
    printf("%p, %p\n", token, items);
    init_symtable_item(true);
    symtable_item *func_id_item = symtable_find_in_stack(token->token_value, sym_st, true);
    if (func_id_item != NULL) // is in stack
    {
        if (func_id_item->type == FUNCTION)
        { // is function
            throw_error(FUNCTIONS_ERR, token->line_num, "Function %s is already defined!\n", token->token_value);
        }
    }
    items->funcItem->id = token->token_value;
}

void state_p_name(Token *token, sym_items *items)
{
    printf("%p, %p\n", token, items);
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count].name = token->token_value;
}

void state_p_id(Token *token, sym_items *items)
{
    printf("%p, %p\n", token, items);
    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count].id = token->token_value;
    init_symtable_item(false);
    items->varItem->id = token->token_value;
}

void state_p_type(Token *token, sym_items *items)
{
    printf("%p, %p\n", token, items);

    items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count].type = get_expression_type(token);

    // check param
    if (items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count].name == items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count].id)
    {
        throw_error(FUNCTIONS_ERR, token->line_num, "Parameter name matches parameter id");
    }
    for (int i = 0; i < items->funcItem->data.func_data->params_count; i++)
    {
        DEBUG_SEMANTIC_CODE(printf(CYAN "ADDED PARAM: %s, id: %s, type: %d\n", items->funcItem->data.func_data->params[i].name, items->funcItem->data.func_data->params[i].id, items->funcItem->data.func_data->params[i].type););
        if (strcmp(items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count].id, items->funcItem->data.func_data->params[i].id) == 0)
        {
            throw_error(FUNCTIONS_ERR, token->line_num, "Parameter: %s in function: %s is already defined", items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count].id, items->funcItem->id);
        }
    }

    add_param(items->funcItem->data.func_data);
    // DEBUG_PSA_CODE(printf(YELLOW "ADDING PARAM: %s, id: %s, type: %d\n", items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count].name, items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count].id, items->funcItem->data.func_data->params[items->funcItem->data.func_data->params_count].type););
    // reset param
    add_param(items->funcItem->data.func_data);
}

void state_r_type(Token *token, sym_items *items)
{
    printf("%p, %p\n", token, items);
    items->funcItem->data.func_data->return_type = get_expression_type(token);
}

void state_func_header_done(Token *token, sym_items *items)
{
    printf("%p, %p\n", token, items);

    DEBUG_SEMANTIC_CODE(printf(YELLOW "ADDING FUNC: %s, return type: %d\n", items->funcItem->id, items->funcItem->data.func_data->return_type););
    symtable_add(items->funcItem, symtable_stack_top(sym_st));
    DEBUG_SEMANTIC_CODE(symtable_print(symtable_stack_top(sym_st)););

    // push scope
    DEBUG_SEMANTIC_CODE(printf(RED "PUSH_SCOPE\n" RESET););
    symtable symtable = symtable_init();
    symtable_stack_push(sym_st, symtable);

    // add params as vars to new scope
    if (items->funcItem->data.func_data->params_count <= 0) // no params
    {
        return;
    }
    for (int i = 0; i < items->funcItem->data.func_data->params_count; i++)
    {
        if (strcmp(items->funcItem->data.func_data->params[i].id, "_") != 0)
        {
            init_symtable_item(false);
            items->varItem->id = items->funcItem->data.func_data->params[i].id;
            items->varItem->data.var_data->type = items->funcItem->data.func_data->params[i].type;
            items->varItem->data.var_data->is_const = true;
            items->varItem->data.var_data->is_initialized = true;
            items->varItem->data.var_data->is_param = true;
            symtable_add(items->varItem, symtable_stack_top(sym_st));
            DEBUG_SEMANTIC_CODE(printf(YELLOW "ADDING VAR TO FUNCTIONS: %s, type: %d, const: %d\n", items->varItem->id, items->varItem->data.var_data->type, items->varItem->data.var_data->is_const););
        }
    }
    DEBUG_SEMANTIC_CODE(symtable_print(symtable_stack_top(sym_st)););
}

void state_push_scope(Token *token, sym_items *items)
{
    printf("%p, %p\n", token, items);

    DEBUG_SEMANTIC_CODE(printf(RED "PUSH_SCOPE\n" RESET););
    symtable symtable = symtable_init();
    symtable_stack_push(sym_st, symtable);
}

void state_pop_scope(Token *token, sym_items *items)
{
    printf("%p, %p\n", token, items);
    DEBUG_SEMANTIC_CODE(printf(RED "POP_SCOPE\n"););
    symtable_print(symtable_stack_top(sym_st));
    symtable_stack_pop(sym_st);
}

void state_r_exp(Token *token, sym_items *items)
{
    printf("%p, %p\n", token, items);
    psa_return_type return_type2 = parse_expression();
    if (((items->funcItem->data.func_data->return_type == TYPE_EMPTY) ^ (return_type2.type == TYPE_EMPTY))) // items->funcItem_type XOR exp_type
    {
        throw_error(RETURN_ERR, token->line_num, " ");
    }
    else if (return_type2.is_ok == false)
    {
        throw_error(PARAM_TYPE_ERR, token->line_num, "Unrecognizable type of return value in function: %s \n", items->funcItem->id);
    }
    else if (!(check_ret_values(return_type2.type, items->funcItem->data.func_data->return_type)))
    {
        throw_error(PARAM_TYPE_ERR, token->line_num, "Expression type: %d and return type: %d of function: %s do not match!\n", return_type2.type, items->funcItem->data.func_data->return_type, items->funcItem->id);
    }
    symtable_find_in_stack(items->funcItem->id, sym_st, true)->data.func_data->found_return = true;
    DEBUG_SEMANTIC_CODE(print_expression_type(return_type2.type););
}

void state_cond_exp(Token *token, sym_items *items)
{
    printf("%p, %p\n", token, items);

    DEBUG_SEMANTIC_CODE(printf(CYAN "COND EXP: %s\n", token->token_value);
                        symtable_print(symtable_stack_top(sym_st)););
    psa_return_type return_type3 = parse_expression();
    DEBUG_SEMANTIC_CODE(printf(CYAN "COND EXP: %d\n", return_type3.type););
    if (return_type3.is_ok == false)
    {
        throw_error(COMPATIBILITY_ERR, token->line_num, "Invalid expression!\n");
    }
    else if (return_type3.type != TYPE_BOOL)
    {
        throw_error(COMPATIBILITY_ERR, token->line_num, "Expression type: %d and type: %d of variable: %s do not match!\n", return_type3.type, TYPE_BOOL, items->varItem->id);
    }

    // push new scope
    DEBUG_SEMANTIC_CODE(printf(RED "PUSH_SCOPE\n" RESET););
    symtable symtable = symtable_init();
    symtable_stack_push(sym_st, symtable);

    DEBUG_SEMANTIC_CODE(print_expression_type(return_type3.type););
}

void state_let_in_if(Token *token, sym_items *items)
{
    printf("%p, %p\n", token, items);

    init_symtable_item(false);
    symtable_item *let_in_if_item = symtable_find_in_stack(token->token_value, sym_st, false);
    if (let_in_if_item == NULL || let_in_if_item->data.var_data->is_const == false)
    {
        throw_error(SEMANTICS_ERR, token->line_num, "Variable %s is not a defined const!\n", token->token_value);
    }
    else
    {
        // prepare items->varItem
        *(items->varItem->data.var_data) = *(let_in_if_item->data.var_data);
        items->varItem->id = token->token_value;
        switch (items->varItem->data.var_data->type)
        {
        case TYPE_BOOL_NIL:
            items->varItem->data.var_data->type = TYPE_BOOL;
            break;
        case TYPE_DOUBLE_NIL:
            items->varItem->data.var_data->type = TYPE_DOUBLE;
            break;
        case TYPE_INT_NIL:
            items->varItem->data.var_data->type = TYPE_INT;
            break;
        case TYPE_STRING_NIL:
            items->varItem->data.var_data->type = TYPE_STRING;
            break;
        default:
            break;
        }
    }

    // push new scope
    DEBUG_SEMANTIC_CODE(printf(RED "PUSH_SCOPE\n" RESET););
    symtable symtable = symtable_init();
    symtable_stack_push(sym_st, symtable);

    print_items();

    // add temporary var to scope
    symtable_add(items->varItem, symtable_stack_top(sym_st));

    DEBUG_SEMANTIC_CODE(symtable_print(symtable_stack_top(sym_st)););
}

void state_func_body_done(Token *token, sym_items *items)
{
    printf("%p, %p\n", token, items);

    symtable_item *func_body_item = symtable_find_in_stack(items->funcItem->id, sym_st, true);
    if (func_body_item->data.func_data->found_return == false && func_body_item->data.func_data->return_type != TYPE_EMPTY)
    {
        throw_error(PARAM_TYPE_ERR, token->line_num, "Function %s of type: %d does not have a return statement!\n", items->funcItem->id, func_body_item->data.func_data->return_type);
    }
    state_pop_scope(token, items);
}

void state_load_identif(Token *token, sym_items *items)
{
    printf("%p, %p\n", token, items);

    // init_symtable_item(false);
    DEBUG_SEMANTIC_CODE(printf(CYAN);
                        symtable_print(symtable_stack_top(sym_st)););
    symtable_item *item = symtable_find_in_stack(token->token_value, sym_st, false);
    if (item == NULL)
    {
        throw_error(FUNCTIONS_ERR, token->line_num, "Variable %s is not defined!\n", token->token_value);
    }
    else if (item->data.var_data->is_const == true)
    {
        fprintf(stderr, RED "Variable %s is const!\n", token->token_value);
        throw_error(COMPATIBILITY_ERR, token->line_num, "Variable %s is const!\n", token->token_value);
    }
    // DEBUG_SEMANTIC_CODE(printf("FOUND: %s, type: %d, const: %d\n", item->id, item->data.var_data->type, item->data.var_data->is_const););
    items->varItem->id = token->token_value;
    print_items();
}

void state_identif_exp(Token *token, sym_items *items)
{
    printf("%p, %p\n", token, items);

    psa_return_type return_type4 = parse_expression();
    DEBUG_SEMANTIC_CODE(print_expression_type(return_type4.type););
    if (return_type4.is_ok == false)
    {
        throw_error(COMPATIBILITY_ERR, token->line_num, "Unrecognizable type of variable: %s \n", items->varItem->id);
    }
    symtable_item *identif_exp_item = symtable_find_in_stack(items->varItem->id, sym_st, false);
    if (!(check_ret_values(return_type4.type, identif_exp_item->data.var_data->type) || isTypeConvertable(identif_exp_item->data.var_data->type, return_type4.type, return_type4.is_literal)))
    {
        throw_error(COMPATIBILITY_ERR, token->line_num, "Expression type: %d and type: %d of variable: %s do not match!\n", return_type4.type, identif_exp_item->data.var_data->type, items->varItem->id);
    }
    identif_exp_item->data.var_data->is_initialized = true;
}

void state_func_call_psa(Token *token, sym_items *items)
{
    printf("%p, %p\n", token, items);

    psa_return_type return_type5 = parse_expression();
    if (return_type5.is_ok)
    {
    }
    DEBUG_SEMANTIC_CODE(print_expression_type(return_type5.type););
}

int check_semantic(Token *token, sym_items *items, Control_state sem_rule)
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

bool isTypeConvertable(Expression_type variable_type, Expression_type expression_type, bool is_expression_literal)
{
    bool var_is_double = variable_type == TYPE_DOUBLE || variable_type == TYPE_DOUBLE_NIL;
    bool exprsn_is_int = expression_type == TYPE_INT || expression_type == TYPE_INT_NIL;

    return (var_is_double && exprsn_is_int && is_expression_literal);
}