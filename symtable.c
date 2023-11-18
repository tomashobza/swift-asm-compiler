#include "symtable.h"
#include <string.h>
#include <stdio.h>

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

symtable_t init_symtable()
{
    symtable_t st = malloc(sizeof(symtable_item_t *) * SYMTABLE_MAX_ITEMS);

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

symtable_item_t *symtable_add(symtable_item_t item, symtable_t table)
{
    const uint32_t item_hash = hash(item.id);
    if (item_hash == (uint32_t)-1)
    {
        return NULL;
    }

    symtable_item_t *new_sti = init_symtable_item(item);

    if (table[item_hash] == NULL)
    {
        table[item_hash] = new_sti;
    }
    else
    {
            
        symtable_item_t *last_item = table[item_hash];
        while (last_item->next != NULL)
        {
            last_item = last_item->next;
        }
        last_item->next = new_sti;
    }

    return new_sti;
}

symtable_item_t *init_symtable_item(symtable_item_t item)
{
    symtable_item_t *new_sti = malloc(sizeof(symtable_item_t));
    if (new_sti == NULL)
    {
        return NULL;
    }

    *new_sti = item;
    return new_sti;
}

symtable_item_t *symtable_get(char *id, symtable_t table)
{
    const uint32_t item_hash = hash(id);
    if (item_hash == (uint32_t)-1)
    {
        return NULL;
    }

    symtable_item_t *item = table[item_hash];
    while (item != NULL)
    {
        if (strcmp(item->id, id) == 0)
        {
            return item;
        }
        item = item->next;
    }

    return NULL;
}

void symtable_print(symtable_t table)
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
            printf("Found: ");
            symtable_item_t *item = table[i];
            while (item != NULL)
            {
                printf("id: %s, type: %d", item->id, item->type);
                item = item->next;
            }
            printf("\n");
        }
    }
}

void free_synonyms(symtable_item_t *item)
{
    if (item->next != NULL)
    {
        free_synonyms(item->next);
    }

    free(item);
}

void symtable_free(symtable_t table)
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