/**
 * @file symtable.c
 * @author Anastasia Butok (xbutok00@vutbr.cz)
 * @brief Implementation of the table of symbols functions.
 * @version 0.1
 * @date 2023-11-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "symtable.h"

unsigned long gen_id_idx_cnt = 0;
const uint32_t FNV_PRIME = 16777619;
const uint32_t FNV_OFFSET_BASIS = 2166136261;

// TODO: make symtable stack global

uint32_t hash(char *input)
{
    // Check for NULL input
    if (input == NULL)
    {
        return -1; // Return an error value for null input
    }

    // Calculate the length of the input
    int len = strlen(input);

    // Check for empty string
    if (len <= 0)
    {
        return -1; // Return an error value for empty string
    }

    uint32_t hash = FNV_OFFSET_BASIS;

    for (int i = 0; i < len; i++)
    {
        hash ^= (uint32_t)input[i];
        hash *= FNV_PRIME;
    }

    return hash % SYMTABLE_MAX_ITEMS;
}

DEFINE_STACK_FUNCTIONS(symtable);

void symtable_stack_free_all(symtable_stack *stack)
{
    while (stack->size > 0)
    {
        // symtable_free(symtable_stack_pop(stack));
    }

    // free(stack);
}

symtable symtable_init()
{
    symtable st = malloc(sizeof(symtable_item *) * SYMTABLE_MAX_ITEMS);

    if (st == NULL)
    {
        return NULL;
    }

    for (int i = 0; i < SYMTABLE_MAX_ITEMS; i++)
    {
        st[i] = NULL;
    }

    return st;
}

symtable_item *symtable_add(symtable_item *item, symtable table)
{
    // get hash
    const uint32_t item_hash = hash(item->id);
    if (item_hash == (uint32_t)-1)
    {
        return NULL;
    }

    if (table[item_hash] == NULL) // new item
    {
        table[item_hash] = item;
    }
    else // found item
    {

        symtable_item *last_item = table[item_hash];
        while (last_item->next != NULL)
        {
            last_item = last_item->next;
        }
        last_item->next = item;
    }

    if (item->type == VARIABLE)
    {
        item->data.var_data->gen_id_idx = gen_id_idx_cnt;
        gen_id_idx_cnt++;
    }

    return item;
}

symtable_item *symtable_find(char *name, symtable table, bool is_func)
{
    const uint32_t item_hash = hash(name);
    if (item_hash == (uint32_t)-1)
    {
        return NULL;
    }

    symtable_item *item = table[item_hash];
    while (item != NULL)
    {
        if (item->id == 0)
        {
            item = item->next;
            continue;
        }
        if (strcmp(item->id, name) == 0 && item->type == is_func)
        {
            return item;
        }
        else if (strcmp(item->id, name) == 0 && item->type == VARIABLE)
        {
            return item;
        }
        item = item->next;
    }

    return NULL;
}

symtable_item *symtable_find_in_stack(char *name, symtable_stack *stack, bool is_func)
{
    int cnt = 0;
    symtable_node *node = stack->top;
    while (node != NULL)
    {
        symtable_item *item = symtable_find(name, node->data, is_func);
        if (item != NULL)
        {
            DEBUG_SEMANTIC_CODE(printf("Found %s in %d. symtable\n", name, cnt););
            item->scope = (int)sym_st->size - cnt - 1;
            return item;
        }
        node = node->next;
        cnt = cnt + 1;
    }

    return NULL;
}

void add_param(FunctionData *func)
{
    if (func->params == NULL)
    {
        // First param
        func->params = init_param_data(1);
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

    func->params_count++;
}

VariableData *init_var_data()
{
    VariableData *var_data = malloc(sizeof(VariableData));
    if (var_data == NULL)
    {
        return NULL;
    }

    var_data->gen_id_idx = 0;
    var_data->is_const = false;
    var_data->is_initialized = false;
    var_data->type = TYPE_EMPTY;

    return var_data;
}
ParamData *init_param_data(int count)
{
    ParamData *param_data = malloc(count * sizeof(ParamData));
    if (param_data == NULL)
    {
        return NULL;
    }

    param_data->id = NULL;
    param_data->name = NULL;
    param_data->type = TYPE_EMPTY;

    return param_data;
}

FunctionData *init_func_data()
{
    FunctionData *func_data = malloc(sizeof(FunctionData));
    if (func_data == NULL)
    {
        return NULL;
    }

    func_data->params_count = 0;
    func_data->capacity = 0;
    func_data->return_type = TYPE_EMPTY;

    return func_data;
}

symtable_item *init_symtable_item(bool is_func)
{

    symtable_item *new_sti = malloc(sizeof(symtable_item));
    if (new_sti == NULL)
    {
        return NULL;
    }

    if (is_func == false) // is variable
    {
        new_sti->type = VARIABLE;
        new_sti->data.var_data = init_var_data();
    }
    else if (is_func == true) // is function
    {
        new_sti->id = NULL;
        new_sti->type = FUNCTION;
        new_sti->data.func_data = init_func_data();
        new_sti->data.func_data->params = NULL;
        new_sti->data.func_data->params_count = 0;
        new_sti->data.func_data->capacity = 0;
        new_sti->data.func_data->return_type = TYPE_EMPTY;
        new_sti->data.func_data->found_return = false;
    }
    new_sti->next = NULL;

    return new_sti;
}

void symtable_print(symtable table)
{
    for (int i = 0; i < SYMTABLE_MAX_ITEMS; i++)
    {
        printf("%d: ", i);
        if (table[i] == NULL)
        {
            printf("NULL\n");
        }
        else
        {
            symtable_item *item = table[i];
            while (item != NULL)
            {
                if (item->type == VARIABLE)
                {
                    printf("id: %s, type: %d, is_const: %d, is_init: %d, is_param: %d", item->id, item->data.var_data->type, item->data.var_data->is_const, item->data.var_data->is_initialized, item->data.var_data->is_param);
                }
                else if (item->type == FUNCTION)
                {
                    printf("id: %s, return_type: %d, ", item->id, item->data.func_data->return_type);
                    for (int i = 0; i < item->data.func_data->params_count; i++)
                    {
                        printf("param %d: %s, %s, %d, ", i, item->data.func_data->params[i].name, item->data.func_data->params[i].id, item->data.func_data->params[i].type);
                    }
                }
                item = item->next;
            }
            printf("\n");
        }
    }
}

void free_synonyms(symtable_item *item)
{
    if (item->next != NULL)
    {
        free_synonyms(item->next);
    }

    free(item);
}

void symtable_free(symtable table)
{
    for (int i = 0; i < SYMTABLE_MAX_ITEMS; i++)
    {
        if (table[i] != NULL)
        {
            free_synonyms(table[i]);
        }
    }

    free(table);
}
