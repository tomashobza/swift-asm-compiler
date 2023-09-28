#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdint.h>

typedef struct
{

} Symtable_t;

/**
 * @brief Calculates the hashed value of a string. Source: https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
 *
 * @param input value to be hashed
 * @return int
 */
uint32_t hash(char *input);

#endif // SYMTABLE_H
