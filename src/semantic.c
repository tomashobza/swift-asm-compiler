#include "semantic.h"

symtable_item *varItem;  // Item to be added to symtable
symtable_item *funcItem; // Item to be added to symtable
symtable mySymtable;     // Symtable
ParamData new_param;     // ParamData to be added to FunctionData

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

    // Inicializace symtable
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
}

void reset_func()
{
    funcItem->id = NULL;
    funcItem->data.func_data->return_type = TYPE_EMPTY;
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
    default:
        return TYPE_INVALID;
        break;
    }
}

void print_items()
{
    DEBUG_SEMANTIC_CODE(
        printf(MAGENTA "FUNCTION: %s, return type: %d" RESET "\n", funcItem->id, funcItem->data.func_data->return_type);
        for (int i = 0; i < funcItem->data.func_data->params_count; i++) {
            printf(MAGENTA "PARAM: %s, id: %s, type: %d" RESET "\n", funcItem->data.func_data->params[i].name, funcItem->data.func_data->params[i].id, funcItem->data.func_data->params[i].type);
        } printf(BLUE "VARIABLE: %s, type: %d, is const: %d" RESET "\n", varItem->id, varItem->data.var_data->type, varItem->data.var_data->is_const);
        printf("\n"););
}

bool is_defined(char *name)
{
    if (symtable_find(name, symtable_stack_top(sym_st), false) == NULL)
    {
        return false;
    }
    else
    {
        return true;
    }
}

int push_token_get_next(Token *token, Token_stack *token_stack)
{
    Token_stack_push(token_stack, *token);
    return main_scanner(token);
}

bool get_func_definition(Token *token, char *name, symtable_item *psa_item)
{
    Token_stack *token_stack = Token_stack_init();
    while (token->token_value != name || token->type != TOKEN_FUNC_ID)
    {
        push_token_get_next(token, token_stack);
    }

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
                return false;
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
                return false;
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
                return false;
            }
            push_token_get_next(token, token_stack);
            break;
        case PARAM:
            ParamData new_psa_param;

            if (token->type == TOKEN_IDENTIFICATOR)
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
                            return false;
                        }
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
            push_token_get_next(token, token_stack);
            break;
        case P_SEP:
            if (token->type == TOKEN_COMMA)
            {
                nstate = PARAM;
            }
            else if (token->type == TOKEN_R_BRACKET)
            {
                nstate = R_BRACKET;
            }
            else
            {
                return false;
            }
            push_token_get_next(token, token_stack);
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
                return false;
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
                return false;
            }
            push_token_get_next(token, token_stack);
            break;
        default:
            return false;
        }
    }

    // return tokens to scanner
    return_token(*token);
    while (Token_stack_empty(token_stack) != false)
    {
        Token_stack_pop(token_stack);
        return_token(*token);
    }

    return true;
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
        if (is_defined(token->token_value))
        {
            fprintf(stderr, RED "Variable %s is already defined!" RESET "\n", token->token_value);
            return 3;
        }
        varItem->id = token->token_value;
        break;
    case VAR_TYPE:
        varItem->data.var_data->type = get_expression_type(token);
        // todo semantic checks
        DEBUG_SEMANTIC_CODE(printf(YELLOW "ADDING VAR: %s, type: %d, const: %d\n", varItem->id, varItem->data.var_data->type, varItem->data.var_data->is_const););
        symtable_add(*varItem, symtable_stack_top(sym_st));
        DEBUG_SEMANTIC_CODE(symtable_print(symtable_stack_top(sym_st)););
        break;
    case VAR_ASSIGN1:
        printf("VAR_ASSIGN2\n");
        DEBUG_SEMANTIC_CODE(printf(YELLOW "ADDING VAR: %s, type: %d, const: %d\n", varItem->id, varItem->data.var_data->type, varItem->data.var_data->is_const););
        symtable_add(*varItem, symtable_stack_top(sym_st));
        DEBUG_SEMANTIC_CODE(symtable_print(symtable_stack_top(sym_st)););
        break;
    case VAR_ASSIGN2:
        printf("VAR_ASSIGN2\n");
        DEBUG_SEMANTIC_CODE(printf(YELLOW "ADDING VAR: %s, type: %d, const: %d\n", varItem->id, varItem->data.var_data->type, varItem->data.var_data->is_const););
        symtable_item *var_ass_item = symtable_find_in_stack(varItem->id, sym_st, false);
        var_ass_item->data.var_data->is_initialized = true;
        DEBUG_SEMANTIC_CODE(symtable_print(symtable_stack_top(sym_st)););
        break;
    case VAR_EXP:
        psa_return_type return_type = parse_expression(sym_st);
        DEBUG_SEMANTIC_CODE(print_expression_type(return_type.type););
        if (return_type.is_ok == false)
        {
            fprintf(stderr, RED "Unrecognizable type of variable: %s\n" RESET, varItem->id);
            return -1;
        }
        if (return_type.type == TYPE_NIL)
        {
            if (varItem->data.var_data->type == TYPE_EMPTY)
            {
                fprintf(stderr, RED "Couldn't decide the type of %s from type NIL!\n" RESET, varItem->id);
                return -1;
            }
            else if (varItem->data.var_data->type != TYPE_NIL)
            {
                fprintf(stderr, RED "Expression type: %d and type: %d of variable: %s do NOT match!\n" RESET, return_type.type, varItem->data.var_data->type, varItem->id);
                return -1;
            }
        }
        else if (varItem->data.var_data->type == TYPE_EMPTY)
        {
            (symtable_find(varItem->id, symtable_stack_top(sym_st), false))->data.var_data->type = return_type.type;
        }
        else if (varItem->data.var_data->type != return_type.type)
        {
            fprintf(stderr, RED "Expression type: %d and type: %d of variable: %s do not match!\n" RESET, return_type.type, varItem->data.var_data->type, varItem->id);
            return -1;
        }
        DEBUG_SEMANTIC_CODE(symtable_print(symtable_stack_top(sym_st)););
        // data_type check
        break;
    case FUNC_ID:
        reset_func();
        symtable_item *func_id_item = symtable_find_in_stack(token->token_value, sym_st, true);
        if (func_id_item != NULL) // is in stack
        {
            if (func_id_item->type == FUNCTION)
            { // is function
                fprintf(stderr, RED "Function %s is already defined!\n" RESET, token->token_value);
                return 3;
            }
        }
        funcItem->id = token->token_value;
        break;
    case P_NAME:
        new_param.name = token->token_value;
        break;
    case P_ID:
        new_param.id = token->token_value;
        break;
    case P_TYPE:
        new_param.type = get_expression_type(token);
        add_param(funcItem->data.func_data, new_param);
        reset_param();
        break;
    case R_TYPE:
        funcItem->data.func_data->return_type = get_expression_type(token);
        break;
    case FUNC_HEADER_DONE:
        DEBUG_SEMANTIC_CODE(printf(YELLOW "ADDING FUNC: %s, return type: %d\n", funcItem->id, funcItem->data.func_data->return_type););
        symtable_add(*funcItem, symtable_stack_top(sym_st));
        DEBUG_SEMANTIC_CODE(symtable_print(symtable_stack_top(sym_st)););
        goto PUSH_SCOPE;
        break;
    case PUSH_SCOPE:
    PUSH_SCOPE:
    {
        symtable symtable = symtable_init();
        symtable_stack_push(sym_st, symtable);
    }
    break;
    case POP_SCOPE:
    POP_SCOPE:
        symtable_stack_pop(sym_st);
        break;
    case R_EXP:
        psa_return_type return_type2 = parse_expression(sym_st);
        if (return_type2.is_ok == false)
        {
            fprintf(stderr, RED "Unrecognizable type of variable: %s \n" RESET, varItem->id);
            return -1;
        }
        if (return_type2.type != funcItem->data.func_data->return_type)
        {
            fprintf(stderr, RED "Expression type: %d and type: %d of variable: %s do not match!\n" RESET, return_type2.type, funcItem->data.func_data->return_type, varItem->id);
            return -1;
        }
        symtable_find_in_stack(varItem->id, sym_st, true)->data.func_data->found_return = true;
        DEBUG_SEMANTIC_CODE(print_expression_type(return_type2.type););
        break;
    case COND_EXP:
        psa_return_type return_type3 = parse_expression(sym_st);
        if (return_type3.is_ok == false)
        {
            fprintf(stderr, RED "Unrecognizable type of variable: %s \n" RESET, varItem->id);
            return -1;
        }
        if (return_type3.type != TYPE_BOOL)
        {
            fprintf(stderr, RED "Expression type: %d and type: %d of variable: %s do not match!\n" RESET, return_type3.type, TYPE_BOOL, varItem->id);
            return -1;
        }
        DEBUG_SEMANTIC_CODE(print_expression_type(return_type3.type););
        break;
    case FUNC_BODY_DONE:
        symtable_item *func_body_item = symtable_find_in_stack(funcItem->id, sym_st, true);
        if (func_body_item->data.func_data->found_return == false && func_body_item->data.func_data->return_type != TYPE_EMPTY)
        {
            fprintf(stderr, RED "Function %s of type: %d does not have a return statement!\n" RESET, funcItem->id, func_body_item->data.func_data->return_type);
            goto POP_SCOPE;
            return -1;
        }
        goto POP_SCOPE;
        break;
    case LOAD_IDENTIF:
        // reset_var();
        DEBUG_SEMANTIC_CODE(printf(CYAN);
                            symtable_print(symtable_stack_top(sym_st)););
        symtable_item *item = symtable_find_in_stack(token->token_value, sym_st, false);
        if (item == NULL)
        {
            fprintf(stderr, RED "Variable %s is not defined!\n", token->token_value);
            return 99;
        }
        else if (item->data.var_data->is_const == true)
        {
            fprintf(stderr, RED "Variable %s is const!\n", token->token_value);
            return 99;
        }
        DEBUG_SEMANTIC_CODE(printf("FOUND: %s, type: %d, const: %d\n", item->id, item->data.var_data->type, item->data.var_data->is_const););
        varItem->id = token->token_value;
        print_items();
        break;
    case IDENTIF_EXP:
        psa_return_type return_type4 = parse_expression(sym_st);
        DEBUG_SEMANTIC_CODE(print_expression_type(return_type4.type););
        if (return_type4.is_ok == false)
        {
            fprintf(stderr, RED "Unrecognizable type of variable: %s \n" RESET, varItem->id);
            return -1;
        }
        symtable_item *identif_exp_item = symtable_find_in_stack(varItem->id, sym_st, false);
        if (return_type4.type != identif_exp_item->data.var_data->type)
        {
            fprintf(stderr, RED "Expression type: %d and type: %d of variable: %s do not match!\n" RESET, return_type4.type, identif_exp_item->data.var_data->type, varItem->id);
            return -1;
        }
        identif_exp_item->data.var_data->is_initialized = true;
        break;
    case FUNC_CALL_PSA:
        psa_return_type return_type5 = parse_expression(sym_st);
        if (return_type5.is_ok)
        {
        }
        DEBUG_SEMANTIC_CODE(print_expression_type(return_type5.type););
        break;
    default:
        break;
    }

    print_items();

    return 0;
}