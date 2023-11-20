#include "semantic.h"

Stack *myStack;                 // Stack for symtable
symtable_item_t *varItem;    // Item to be added to symtable
symtable_item_t *funcItem;    // Item to be added to symtable

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
    symtable_t mySymtable = init_symtable();
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

int check_semantic(Token **token, Sem_type sem_type) {
    
    switch(sem_type) {
        case LET:
            varItem->data.var_data->is_const = true;
        case VAR:
            varItem->data.var_data->is_const = false;
    }

    return 0;
}