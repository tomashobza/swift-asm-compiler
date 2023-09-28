#include "symtable.h"
#include <string.h>

const uint32_t FNV_PRIME = 16777619;
const uint32_t FNV_OFFSET_BASIS = 2166136261;

uint32_t hash(char *input)
{
    int len = strlen(input);
    if (len <= 0)
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