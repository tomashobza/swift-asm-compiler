/**
 * @file generator.h
 * @author Tomáš Hobza (xhobza03@vutbr.cz), Jakub Všetečka (xvsete00@vutbr.cz)
 * @brief Header file for the ifjcode2023 code generator.
 * @version 0.1
 * @date 2023-11-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef GENERATOR_H
#define GENERATOR_H

#include <stdio.h>
#include <stdlib.h>
#include "error.h"

extern FILE *out_code_file;

/**
 * @brief Prints the output code to stdout.
 *
 */
void print_out_code();

#endif // GENERATOR_H