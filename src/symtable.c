#include "symtable.h"

const uint32_t FNV_PRIME = 16777619;
const uint32_t FNV_OFFSET_BASIS = 2166136261;

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

DEFINE_STACK_FUNCTIONS(symtable)

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
    const uint32_t item_hash = hash(item.name);
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

symtable_item *symtable_find(char *name, symtable table)
{
    const uint32_t item_hash = hash(name);
    if (item_hash == (uint32_t)-1)
    {
        return NULL;
    }

    symtable_item *item = table[item_hash];
    while (item != NULL)
    {
        if (strcmp(item->name, name) == 0)
        {
            return item;
        }
        item = item->next;
    }

    return NULL;
}

symtable_item *symtable_find_in_stack(char *name, symtable_stack *stack)
{
    symtable_node *node = stack->top;
    while (node != NULL)
    {
        symtable_item *item = symtable_find(name, node->data);
        if (item != NULL)
        {
            return item;
        }
        node = node->next;
    }

    return NULL;
}

symtable_item *init_symtable_item(symtable_item item)
{
    symtable_item *new_sti = malloc(sizeof(symtable_item));
    if (new_sti == NULL)
    {
        return NULL;
    }

    *new_sti = item;
    return new_sti;
}

void symtable_print(symtable table)
{
    for (int i = 0; i < SYMTABLE_MAX_ITEMS; i++)
    {
        if (table[i] == NULL)
        {
            printf("NULL,\n");
        }
        else
        {
            symtable_item *item = table[i];
            while (item != NULL)
            {
                printf("'%s', ", item->name);
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