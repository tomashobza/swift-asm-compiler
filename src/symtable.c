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

const uint32_t FNV_PRIME = 16777619;
const uint32_t FNV_OFFSET_BASIS = 2166136261;

// TODO: make symtable stack global

uint32_t hash(char *input)
{
    int len = 0;
    if (input == NULL || (len = strlen(input)) <= 0)
    {
        return -1; // in case of empty/invalid string, return an error value
    }

    uint32_t hash = FNV_OFFSET_BASIS;

    for (int i = 0; i < len; i++)
    {
        hash = hash ^ (uint32_t)input[i];
        hash = hash * FNV_PRIME;
    }

    return hash % SYMTABLE_MAX_ITEMS;
}

DEFINE_STACK_FUNCTIONS(symtable);

void symtable_stack_free_all(symtable_stack *stack)
{
    while (stack->size > 0)
    {
        symtable_free(symtable_stack_pop(stack));
    }

    free(stack);
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

symtable_item *symtable_add(symtable_item item, symtable table)
{
    const uint32_t item_hash = hash(item.id);
    if (item_hash == (uint32_t)-1)
    {
        return NULL;
    }

    symtable_item *new_sti = init_symtable_item(item);

    if (table[item_hash] == NULL)
    {
        table[item_hash] = new_sti;
    }
    else
    {

        symtable_item *last_item = table[item_hash];
        while (last_item->next != NULL)
        {
            last_item = last_item->next;
        }
        last_item->next = new_sti;
    }

    return new_sti;
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
        if (strcmp(item->id, name) == 0 && item->type == is_func)
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
            return item;
        }
        node = node->next;
        cnt = cnt + 1;
    }

    return NULL;
}

VariableData *init_var_data()
{
    VariableData *var_data = malloc(sizeof(VariableData));
    if (var_data == NULL)
    {
        return NULL;
    }

    var_data->is_const = false;
    var_data->is_initialized = false;
    var_data->type = TYPE_EMPTY;

    return var_data;
}
ParamData *init_param_data()
{
    ParamData *param_data = malloc(sizeof(ParamData));
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

    func_data->params = init_param_data();
    func_data->params_count = 0;
    func_data->capacity = 0;
    func_data->return_type = TYPE_EMPTY;

    return func_data;
}

symtable_item *init_symtable_item(symtable_item item)
{

    symtable_item *new_sti = malloc(sizeof(symtable_item));
    if (new_sti == NULL)
    {
        return NULL;
    }

    *new_sti = item;

    if (item.type == VARIABLE)
    {
        new_sti->data.var_data = init_var_data();
        *(new_sti->data.var_data) = *(item.data.var_data);
    }
    else if (item.type == FUNCTION)
    {
        new_sti->data.func_data = init_func_data();
        *(new_sti->data.func_data) = *(item.data.func_data);
        (new_sti->data.func_data->params) = (item.data.func_data->params);
    }
    new_sti->next = NULL;

    return new_sti;
}

// void symtable_print(symtable_t table)
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
                    printf("id: %s, type: %d, is_const: %d, is_init: %d", item->id, item->data.var_data->type, item->data.var_data->is_const, item->data.var_data->is_initialized);
                }
                else if (item->type == FUNCTION)
                {
                    printf("id: %s, return_type: %d ", item->id, item->data.func_data->return_type);
                    for (int i = 0; i < item->data.func_data->params_count; i++)
                    {
                        printf("\n");
                        printf("%s: param %d: %s, %d \n", item->id, i, item->data.func_data->params[i].id, item->data.func_data->params[i].type);
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