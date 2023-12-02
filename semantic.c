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

symtable_item *varItem;  // Item to be added to symtable
symtable_item *funcItem; // Item to be added to symtable
symtable mySymtable;     // Symtable
ParamData new_param;     // ParamData to be added to FunctionData

void add_builtin_functions()
{
    // readString() -> String?
    reset_func();
    funcItem->id = "readString";
    funcItem->data.func_data->return_type = TYPE_STRING_NIL;
    funcItem->data.func_data->found_return = true;
    symtable_add(*funcItem, symtable_stack_top(sym_st));
    // readInt() -> Int?
    reset_func();
    funcItem->id = "readInt";
    funcItem->data.func_data->return_type = TYPE_INT_NIL;
    funcItem->data.func_data->found_return = true;
    symtable_add(*funcItem, symtable_stack_top(sym_st));
    // readDouble() -> Double?
    reset_func();
    funcItem->id = "readDouble";
    funcItem->data.func_data->return_type = TYPE_DOUBLE_NIL;
    funcItem->data.func_data->found_return = true;
    symtable_add(*funcItem, symtable_stack_top(sym_st));
    // write ( term1 , term2 , …, term𝑛 )
    reset_func();
    funcItem->id = "write";
    funcItem->data.func_data->return_type = TYPE_EMPTY;
    funcItem->data.func_data->found_return = true;
    funcItem->data.func_data->params_count = -1;
    symtable_add(*funcItem, symtable_stack_top(sym_st));
    // Int2Double(_ term ∶ Int) -> Double
    reset_func();
    funcItem->id = "Int2Double";
    funcItem->data.func_data->return_type = TYPE_DOUBLE;
    funcItem->data.func_data->found_return = true;
    reset_param();
    new_param.name = "_";
    new_param.id = ""; // TODO check if okay
    new_param.type = TYPE_INT;
    add_param(funcItem->data.func_data, new_param);
    symtable_add(*funcItem, symtable_stack_top(sym_st));
    // Double2Int(_ term ∶ Double) -> Int
    reset_func();
    funcItem->id = "Double2Int";
    funcItem->data.func_data->return_type = TYPE_INT;
    funcItem->data.func_data->found_return = true;
    reset_param();
    new_param.name = "_";
    new_param.id = ""; // TODO check if okay
    new_param.type = TYPE_DOUBLE;
    add_param(funcItem->data.func_data, new_param);
    symtable_add(*funcItem, symtable_stack_top(sym_st));
    // length(_ 𝑠 : String) -> Int
    reset_func();
    funcItem->id = "length";
    funcItem->data.func_data->return_type = TYPE_INT;
    funcItem->data.func_data->found_return = true;
    reset_param();
    new_param.name = "_";
    new_param.id = ""; // TODO check if okay
    new_param.type = TYPE_STRING;
    add_param(funcItem->data.func_data, new_param);
    symtable_add(*funcItem, symtable_stack_top(sym_st));
    // substring(of 𝑠 : String, startingAt 𝑖 : Int, endingBefore 𝑗 : Int) -> String?
    reset_func();
    funcItem->id = "substring";
    funcItem->data.func_data->return_type = TYPE_STRING_NIL;
    funcItem->data.func_data->found_return = true;
    reset_param();
    new_param.name = "of";
    new_param.id = ""; // TODO check if okay
    new_param.type = TYPE_STRING;
    add_param(funcItem->data.func_data, new_param);
    reset_param();
    new_param.name = "startingAt";
    new_param.id = ""; // TODO check if okay
    new_param.type = TYPE_INT;
    add_param(funcItem->data.func_data, new_param);
    reset_param();
    new_param.name = "endingBefore";
    new_param.id = ""; // TODO check if okay
    new_param.type = TYPE_INT;
    add_param(funcItem->data.func_data, new_param);
    symtable_add(*funcItem, symtable_stack_top(sym_st));
    // ord(_ 𝑐 : String) -> Int
    reset_func();
    funcItem->id = "ord";
    funcItem->data.func_data->return_type = TYPE_INT;
    funcItem->data.func_data->found_return = true;
    reset_param();
    new_param.name = "_";
    new_param.id = ""; // TODO check if okay
    new_param.type = TYPE_STRING;
    add_param(funcItem->data.func_data, new_param);
    symtable_add(*funcItem, symtable_stack_top(sym_st));
    // chr(_ 𝑖 : Int) -> String
    reset_func();
    funcItem->id = "chr";
    funcItem->data.func_data->return_type = TYPE_STRING;
    funcItem->data.func_data->found_return = true;
    reset_param();
    new_param.name = "_";
    new_param.id = ""; // TODO check if okay
    new_param.type = TYPE_INT;
    add_param(funcItem->data.func_data, new_param);
    symtable_add(*funcItem, symtable_stack_top(sym_st));

    printf(GREEN);
    DEBUG_SEMANTIC_CODE(symtable_print(symtable_stack_top(sym_st)););
    printf(RESET);
}

int semantic_init()
{
    varItem = malloc(sizeof(symtable_item));
    funcItem = malloc(sizeof(symtable_item));
    VariableData *varData = malloc(sizeof(VariableData));
    FunctionData *funcData = malloc(sizeof(FunctionData));
    ParamData *paramData = NULL;
    varItem->type = VARIABLE;
    varItem->data.var_data = varData;
    funcItem->type = FUNCTION;
    funcItem->data.func_data = funcData;
    funcItem->data.func_data->params = paramData;

    reset_param();

    // symtable init
    DEBUG_SEMANTIC_CODE(printf(RED "PUSH_SCOPE\n" RESET););
    mySymtable = symtable_init();
    symtable_stack_push(sym_st, mySymtable);

    return 0; // tode errors
}

// Function to add a ParamData to FunctionData, resizing if necessary
void add_param(FunctionData *func, ParamData new_param)
{
    if (func->params == NULL)
    {
        // First param
        func->params = malloc(sizeof(ParamData));
        if (func->params == NULL)
        {
            // Handle memory allocation error
            return;
        }
        func->capacity = 1;
        func->params_count = 0;
    }
    else if (func->params_count == func->capacity)
    {
        // Need to resize
        int new_capacity = func->capacity * 2; // Or choose another resizing strategy
        ParamData *new_params = realloc(func->params, new_capacity * sizeof(ParamData));
        if (new_params == NULL)
        {
            // Handle memory allocation error
            return;
        }
        func->params = new_params;
        func->capacity = new_capacity;
    }

    // Add new_param to the array
    func->params[func->params_count] = new_param;
    func->params_count++;
}

void reset_param()
{
    new_param.name = NULL;
    new_param.id = NULL;
    new_param.type = TYPE_EMPTY;
}

void reset_var()
{
    varItem->id = NULL;
    varItem->data.var_data->type = TYPE_EMPTY;
    varItem->data.var_data->is_const = false;
    varItem->data.var_data->is_initialized = false;
    varItem->data.var_data->is_param = false;
}

void reset_func()
{
    reset_param();
    funcItem->id = NULL;
    funcItem->data.func_data->return_type = TYPE_EMPTY;
    funcItem->data.func_data->found_return = false;
    free(funcItem->data.func_data->params);
    funcItem->data.func_data->params = NULL;
    funcItem->data.func_data->params_count = 0;
    funcItem->data.func_data->capacity = 0;
}

void semantic_destroy()
{
    DEBUG_SEMANTIC_CODE(symtable_print(symtable_stack_top(sym_st)););
    free(funcItem->data.func_data->params);
    free(funcItem->data.func_data);
    free(varItem->data.var_data);
    free(varItem);
    free(funcItem);
    symtable_free(mySymtable);
    // stack_free(myStack);
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
        printf(MAGENTA "FUNCTION: %s, return type: %d, param_count: %d" RESET "\n", funcItem->id, funcItem->data.func_data->return_type, funcItem->data.func_data->params_count);
        for (int i = 0; i < funcItem->data.func_data->params_count; i++) {
            printf(MAGENTA "PARAM: %s, id: %s, type: %d" RESET "\n", funcItem->data.func_data->params[i].name, funcItem->data.func_data->params[i].id, funcItem->data.func_data->params[i].type);
        } printf(BLUE "VARIABLE: %s, type: %d, is const: %d" RESET "\n", varItem->id, varItem->data.var_data->type, varItem->data.var_data->is_const);
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
                            add_param(psa_item->data.func_data, new_psa_param);
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

int check_semantic(Token *token, Sem_rule sem_rule)
{

    switch (sem_rule)
    {
    case LET:
        reset_var();
        varItem->data.var_data->is_const = true;
        break;
    case VAR:
        reset_var();
        varItem->data.var_data->is_const = false;
        break;
    case VAR_ID:
    {
        symtable_item *var_id_item = symtable_find(token->token_value, symtable_stack_top(sym_st), false);
        if (var_id_item != NULL && var_id_item->data.var_data->is_param == false)
        {
            throw_error(FUNCTIONS_ERR, token->line_num, "Variable %s is already defined!", token->token_value)
        }
        else if (var_id_item != NULL && var_id_item->data.var_data->is_param == true)
        {
            varItem->data.var_data->is_param = true;
        }
        varItem->id = token->token_value;
        break;
    }
    case VAR_TYPE:
        varItem->data.var_data->type = get_expression_type(token);
        // todo semantic checks
        // DEBUG_SEMANTIC_CODE(printf(YELLOW "ADDING VAR: %s, type: %d, const: %d\n", varItem->id, varItem->data.var_data->type, varItem->data.var_data->is_const););
        // symtable_add(*varItem, symtable_stack_top(sym_st));
        // DEBUG_SEMANTIC_CODE(symtable_print(symtable_stack_top(sym_st)););
        break;
    case VAR_ASSIGN1:
        // printf("VAR_ASSIGN1\n");
        //  DEBUG_SEMANTIC_CODE(printf(YELLOW "ADDING VAR: %s, type: %d, const: %d\n", varItem->id, varItem->data.var_data->type, varItem->data.var_data->is_const););
        //  symtable_add(*varItem, symtable_stack_top(sym_st));
        //  DEBUG_SEMANTIC_CODE(symtable_print(symtable_stack_top(sym_st)););
        break;
    case VAR_ASSIGN2:
        // printf("VAR_ASSIGN2\n");
        //  DEBUG_SEMANTIC_CODE(printf(YELLOW "ADDING VAR: %s, type: %d, const: %d\n", varItem->id, varItem->data.var_data->type, varItem->data.var_data->is_const););
        //  symtable_item *var_ass_item = symtable_find_in_stack(varItem->id, sym_st, false);
        //  var_ass_item->data.var_data->is_initialized = true;
        //  DEBUG_SEMANTIC_CODE(symtable_print(symtable_stack_top(sym_st)););
        break;
    case VAR_EXP:
    {
        varItem->data.var_data->is_initialized = true;

        DEBUG_SEMANTIC_CODE(
            (symtable_stack_top(sym_st)););
        psa_return_type return_type = parse_expression();
        DEBUG_SEMANTIC_CODE(print_expression_type(return_type.type););
        if (return_type.is_ok == false)
        {
            if (varItem->data.var_data->type == TYPE_EMPTY)
            {
                throw_error(TYPE_ERR, token->line_num, "Unrecognizable type of variable: %s\n", varItem->id);
            }
            else
            {
                throw_error(COMPATIBILITY_ERR, token->line_num, "Unrecognizable type of variable: %s\n", varItem->id);
            }
        }
        else if (return_type.type == TYPE_NIL)
        {
            if (varItem->data.var_data->type == TYPE_EMPTY)
            {
                throw_error(TYPE_ERR, token->line_num, "Couldn't decide the type of %s from type NIL!\n", varItem->id);
            }
            else if (varItem->data.var_data->type != TYPE_INT_NIL && varItem->data.var_data->type != TYPE_STRING_NIL && varItem->data.var_data->type != TYPE_DOUBLE_NIL && varItem->data.var_data->type != TYPE_BOOL_NIL)
            {
                throw_error(COMPATIBILITY_ERR, token->line_num, "Expression type: %d and type: %d of variable: %s do not match!\n", return_type.type, varItem->data.var_data->type, varItem->id);
            }
        }
        else if (varItem->data.var_data->type == TYPE_EMPTY)
        {
            varItem->data.var_data->type = return_type.type;
        }
        else if (!(check_ret_values(return_type.type, varItem->data.var_data->type) || isTypeConvertable(varItem->data.var_data->type, return_type.type, return_type.is_literal)))
        {
            throw_error(COMPATIBILITY_ERR, token->line_num, "Expression type: %d and type: %d of variable: %s do not match!\n", return_type.type, varItem->data.var_data->type, varItem->id);
        }
        DEBUG_SEMANTIC_CODE(
            (symtable_stack_top(sym_st)););
        goto VAR_ADD;
        break;
    }
    case VAR_ADD:
    VAR_ADD:
        if (varItem->data.var_data->is_param == false) // new symmbol
        {
            DEBUG_SEMANTIC_CODE(printf(YELLOW "ADDING VAR: %s, type: %d, const: %d\n", varItem->id, varItem->data.var_data->type, varItem->data.var_data->is_const););
            symtable_add(*varItem, symtable_stack_top(sym_st));
            DEBUG_SEMANTIC_CODE(symtable_print(symtable_stack_top(sym_st)););
        }
        else // is param
        {
            symtable_item *var_add_item = symtable_find_in_stack(varItem->id, sym_st, false);
            var_add_item->data.var_data->type = varItem->data.var_data->type;
            var_add_item->data.var_data->is_const = varItem->data.var_data->is_const;
            var_add_item->data.var_data->is_initialized = varItem->data.var_data->is_initialized;
            var_add_item->data.var_data->is_param = false; // is no longer param
        }
        break;
    case FUNC_ID:
        reset_func();
        symtable_item *func_id_item = symtable_find_in_stack(token->token_value, sym_st, true);
        if (func_id_item != NULL) // is in stack
        {
            if (func_id_item->type == FUNCTION)
            { // is function
                throw_error(FUNCTIONS_ERR, token->line_num, "Function %s is already defined!\n", token->token_value);
            }
        }
        funcItem->id = token->token_value;
        break;
    case P_NAME:
        new_param.name = token->token_value;
        break;
    case P_ID:
        new_param.id = token->token_value;
        reset_var();
        varItem->id = token->token_value;
        break;
    case P_TYPE:
        new_param.type = get_expression_type(token);

        // check param
        if (new_param.name == new_param.id)
        {
            throw_error(FUNCTIONS_ERR, token->line_num, "Parameter name matches parameter id");
        }
        for (int i = 0; i < funcItem->data.func_data->params_count; i++)
        {
            DEBUG_SEMANTIC_CODE(printf(CYAN "ADDED PARAM: %s, id: %s, type: %d\n", funcItem->data.func_data->params[i].name, funcItem->data.func_data->params[i].id, funcItem->data.func_data->params[i].type););
            if (strcmp(new_param.id, funcItem->data.func_data->params[i].id) == 0)
            {
                throw_error(FUNCTIONS_ERR, token->line_num, "Parameter: %s in function: %s is already defined", new_param.id, funcItem->id);
            }
        }

        add_param(funcItem->data.func_data, new_param);
        // DEBUG_PSA_CODE(printf(YELLOW "ADDING PARAM: %s, id: %s, type: %d\n", new_param.name, new_param.id, new_param.type););
        reset_param();
        break;
    case R_TYPE:
        funcItem->data.func_data->return_type = get_expression_type(token);
        break;
    case FUNC_HEADER_DONE:
    {
        DEBUG_SEMANTIC_CODE(printf(YELLOW "ADDING FUNC: %s, return type: %d\n", funcItem->id, funcItem->data.func_data->return_type););
        symtable_add(*funcItem, symtable_stack_top(sym_st));
        DEBUG_SEMANTIC_CODE(symtable_print(symtable_stack_top(sym_st)););

        // push scope
        DEBUG_SEMANTIC_CODE(printf(RED "PUSH_SCOPE\n" RESET););
        symtable symtable = symtable_init();
        symtable_stack_push(sym_st, symtable);

        // add params as vars to new scope
        if (funcItem->data.func_data->params_count <= 0) // no params
        {
            break;
        }
        for (int i = 0; i < funcItem->data.func_data->params_count; i++)
        {
            if (strcmp(funcItem->data.func_data->params[i].id, "_") != 0)
            {
                reset_var();
                varItem->id = funcItem->data.func_data->params[i].id;
                varItem->data.var_data->type = funcItem->data.func_data->params[i].type;
                varItem->data.var_data->is_const = true;
                varItem->data.var_data->is_initialized = true;
                varItem->data.var_data->is_param = true;
                symtable_add(*varItem, symtable_stack_top(sym_st));
                DEBUG_SEMANTIC_CODE(printf(YELLOW "ADDING VAR TO FUNCTIONS: %s, type: %d, const: %d\n", varItem->id, varItem->data.var_data->type, varItem->data.var_data->is_const););
            }
        }
        DEBUG_SEMANTIC_CODE(symtable_print(symtable_stack_top(sym_st)););
        break;
    }
    case PUSH_SCOPE:
    {
        DEBUG_SEMANTIC_CODE(printf(RED "PUSH_SCOPE\n" RESET););
        symtable symtable = symtable_init();
        symtable_stack_push(sym_st, symtable);
    }
    break;
    case POP_SCOPE:
    POP_SCOPE:
        DEBUG_SEMANTIC_CODE(printf(RED "POP_SCOPE\n"););
        DEBUG_SEMANTIC_CODE(symtable_print(symtable_stack_top(sym_st)););
        symtable_stack_pop(sym_st);
        break;
    case R_EXP:
    {
        psa_return_type return_type2 = parse_expression();
        if (((funcItem->data.func_data->return_type == TYPE_EMPTY) ^ (return_type2.type == TYPE_EMPTY))) // funcItem_type XOR exp_type
        {
            throw_error(RETURN_ERR, token->line_num, " ");
        }
        else if (return_type2.is_ok == false)
        {
            throw_error(PARAM_TYPE_ERR, token->line_num, "Unrecognizable type of return value in function: %s \n", funcItem->id);
        }
        else if (!(check_ret_values(return_type2.type, funcItem->data.func_data->return_type)))
        {
            throw_error(PARAM_TYPE_ERR, token->line_num, "Expression type: %d and return type: %d of function: %s do not match!\n", return_type2.type, funcItem->data.func_data->return_type, funcItem->id);
        }
        symtable_find_in_stack(funcItem->id, sym_st, true)->data.func_data->found_return = true;
        DEBUG_SEMANTIC_CODE(print_expression_type(return_type2.type););
        break;
    }
    case COND_EXP:
    {
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
            throw_error(COMPATIBILITY_ERR, token->line_num, "Expression type: %d and type: %d of variable: %s do not match!\n", return_type3.type, TYPE_BOOL, varItem->id);
        }

        // push new scope
        DEBUG_SEMANTIC_CODE(printf(RED "PUSH_SCOPE\n" RESET););
        symtable symtable = symtable_init();
        symtable_stack_push(sym_st, symtable);

        DEBUG_SEMANTIC_CODE(print_expression_type(return_type3.type););
        break;
    }
    case LET_IN_IF:
    {
        reset_var();
        symtable_item *let_in_if_item = symtable_find_in_stack(token->token_value, sym_st, false);
        if (let_in_if_item == NULL || let_in_if_item->data.var_data->is_const == false)
        {
            throw_error(SEMANTICS_ERR, token->line_num, "Variable %s is not a defined const!\n", token->token_value);
        }
        else
        {
            // prepare varItem
            *(varItem->data.var_data) = *(let_in_if_item->data.var_data);
            varItem->id = token->token_value;
            switch (varItem->data.var_data->type)
            {
            case TYPE_BOOL_NIL:
                varItem->data.var_data->type = TYPE_BOOL;
                break;
            case TYPE_DOUBLE_NIL:
                varItem->data.var_data->type = TYPE_DOUBLE;
                break;
            case TYPE_INT_NIL:
                varItem->data.var_data->type = TYPE_INT;
                break;
            case TYPE_STRING_NIL:
                varItem->data.var_data->type = TYPE_STRING;
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
        symtable_add(*varItem, symtable_stack_top(sym_st));

        DEBUG_SEMANTIC_CODE(symtable_print(symtable_stack_top(sym_st)););
        break;
    }
    case FUNC_BODY_DONE:
    {
        symtable_item *func_body_item = symtable_find_in_stack(funcItem->id, sym_st, true);
        if (func_body_item->data.func_data->found_return == false && func_body_item->data.func_data->return_type != TYPE_EMPTY)
        {
            throw_error(PARAM_TYPE_ERR, token->line_num, "Function %s of type: %d does not have a return statement!\n", funcItem->id, func_body_item->data.func_data->return_type);
            goto POP_SCOPE;
        }
        goto POP_SCOPE;
        break;
    }
    case LOAD_IDENTIF:
        // reset_var();
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
        varItem->id = token->token_value;
        print_items();
        break;
    case IDENTIF_EXP:
    {
        psa_return_type return_type4 = parse_expression();
        DEBUG_SEMANTIC_CODE(print_expression_type(return_type4.type););
        if (return_type4.is_ok == false)
        {
            throw_error(COMPATIBILITY_ERR, token->line_num, "Unrecognizable type of variable: %s \n", varItem->id);
        }
        symtable_item *identif_exp_item = symtable_find_in_stack(varItem->id, sym_st, false);
        if (!(check_ret_values(return_type4.type, identif_exp_item->data.var_data->type) || isTypeConvertable(identif_exp_item->data.var_data->type, return_type4.type, return_type4.is_literal)))
        {
            throw_error(COMPATIBILITY_ERR, token->line_num, "Expression type: %d and type: %d of variable: %s do not match!\n", return_type4.type, identif_exp_item->data.var_data->type, varItem->id);
        }
        identif_exp_item->data.var_data->is_initialized = true;
        break;
    }
    case FUNC_CALL_PSA:
    {
        psa_return_type return_type5 = parse_expression();
        if (return_type5.is_ok)
        {
        }
        DEBUG_SEMANTIC_CODE(print_expression_type(return_type5.type););
        break;
    }
    default:
        break;
    }

    print_items();

    return 0;
}

bool isTypeConvertable(Expression_type variable_type, Expression_type expression_type, bool is_expression_literal)
{
    bool var_is_double = variable_type == TYPE_DOUBLE || variable_type == TYPE_DOUBLE_NIL;
    bool exprsn_is_int = expression_type == TYPE_INT || expression_type == TYPE_INT_NIL;

    return (var_is_double && exprsn_is_int && is_expression_literal);
}