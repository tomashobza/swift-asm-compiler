#include "semantic.h"

Stack *myStack;                 // Stack for symtable
symtable_item_t *varItem;    // Item to be added to symtable
symtable_item_t *funcItem;    // Item to be added to symtable
symtable_t *mySymtable;          // Symtable

int semantic_init() {
    myStack = stack_init();
    varItem = malloc(sizeof(symtable_item_t));
    funcItem = malloc(sizeof(symtable_item_t));
    VariableData *varData = malloc(sizeof(VariableData));
    FunctionData *funcData = malloc(sizeof(FunctionData));
    varItem->type = VARIABLE;
    varItem->data.var_data = varData;
    funcItem->type = FUNCTION;
    funcItem->data.func_data = funcData;

    // Inicializace symtable
    mySymtable = init_symtable();
    stack_push(myStack, &mySymtable);

    return 0; // tode errors
}

void semantic_destroy() {  
    free(funcItem->data.func_data);
    free(varItem->data.var_data);
    free(varItem);
    free(funcItem);
    //symtable_free(mySymtable);
    stack_free(myStack);
}

void print_items() {
    printf(MAGENTA "FUNCTION: %s, return type: %d, is defined: %d" RESET "\n", funcItem->id, funcItem->data.func_data->return_type, funcItem->data.func_data->is_defined);
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
            break;
        case FUNC_ID:
            funcItem->id = (*token)->token_value;
            break;
    }

    symtable_print(*stack_top(myStack));
    print_items();

    return 0;
}