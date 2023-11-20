#include "semantic.h"

Stack *myStack;                 // Stack for symtable
symtable_item_t *varItem;    // Item to be added to symtable
symtable_item_t *funcItem;    // Item to be added to symtable
symtable_t mySymtable;          // Symtable
ParamData new_param;           // ParamData to be added to FunctionData

int semantic_init() {
    myStack = stack_init();
    varItem = malloc(sizeof(symtable_item_t));
    funcItem = malloc(sizeof(symtable_item_t));
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
    mySymtable = init_symtable();
    stack_push(myStack, &mySymtable);

    return 0; // tode errors
}

// Function to add a ParamData to FunctionData, resizing if necessary
void add_param(FunctionData *func, ParamData new_param) {
    if (func->params == NULL) {
        // First param
        func->params = malloc(sizeof(ParamData));
        if (func->params == NULL) {
            // Handle memory allocation error
            return;
        }
        func->capacity = 1;
        func->params_count = 0;
    } else if (func->params_count == func->capacity) {
        // Need to resize
        int new_capacity = func->capacity * 2; // Or choose another resizing strategy
        ParamData *new_params = realloc(func->params, new_capacity * sizeof(ParamData));
        if (new_params == NULL) {
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

void reset_param() {
    new_param.name = NULL;
    new_param.id = NULL;
    new_param.type = NULL;
}

void reset_var() {
    varItem->id = NULL;
    varItem->data.var_data->type = VARIABLE;
    varItem->data.var_data->is_const = false;
    varItem->data.var_data->is_initialized = false;
}

void reset_func() {
    funcItem->id = NULL;
    funcItem->data.func_data->return_type = "Void";
    funcItem->data.func_data->is_defined = false;
    free(funcItem->data.func_data->params);
    funcItem->data.func_data->params = NULL;
    funcItem->data.func_data->params_count = 0;
    funcItem->data.func_data->capacity = 0;

}

void semantic_destroy() {  
    symtable_print(*stack_top(myStack));
    free(funcItem->data.func_data->params);
    free(funcItem->data.func_data);
    free(varItem->data.var_data);
    free(varItem);
    free(funcItem);
    symtable_free(mySymtable);
    stack_free(myStack);
}

void print_items() {
    printf(MAGENTA "FUNCTION: %s, return type: %s, is defined: %d" RESET "\n", funcItem->id, funcItem->data.func_data->return_type, funcItem->data.func_data->is_defined);
    for (int i = 0; i < funcItem->data.func_data->params_count; i++) {
        printf(MAGENTA "PARAM: %s, id: %s, type: %s" RESET "\n", funcItem->data.func_data->params[i].name, funcItem->data.func_data->params[i].id, funcItem->data.func_data->params[i].type);
    }
    printf(BLUE "VARIABLE: %s, type: %d, is const: %d" RESET "\n", varItem->id, varItem->data.var_data->type, varItem->data.var_data->is_const);
}

int check_semantic(Token **token, Sem_rule sem_rule) {
    
    switch(sem_rule) {
        case LET:
            varItem->data.var_data->is_const = true;
            break;
        case VAR:
            varItem->data.var_data->is_const = false;
            break;
        case VAR_ID:
            varItem->id = (*token)->token_value;
            break;
        case VAR_TYPE:
            varItem->data.var_data->type = (*token)->type;
            // todo semantic checks
            symtable_add(*varItem, *stack_top(myStack));
            reset_var();
            break;
        case FUNC_ID:
            funcItem->id = (*token)->token_value;
            break;
        case P_NAME:
            new_param.name = (*token)->token_value;
        case P_ID:
            new_param.id = (*token)->token_value;
            break;
        case P_TYPE:
            new_param.type = (*token)->token_value;
            add_param(funcItem->data.func_data, new_param);
            reset_param();
            break;
        case R_TYPE:
            funcItem->data.func_data->return_type = (*token)->token_value;
            break;
        case FUNC_HEADER_DONE:
            funcItem->data.func_data->is_defined = true;
            symtable_add(*funcItem, *stack_top(myStack));

    }

    print_items();

    return 0;
}