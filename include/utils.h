#ifndef UTILS_H
#define UTILS_H

#include "scanner.h"
#include "psa.h"
#include "colorful_printf.h"

/**
 * @brief Utility function for printing the token type.
 *
 * @param type
 */
void print_token_type(Token_type type);

/**
 * @brief Utility function for printing the expression type.
 *
 * @param type
 */
void print_expression_type(Expression_type type);

#endif // UTILS_H