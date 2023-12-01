/**
 * @file error.h
 * @brief error header file
 * @author Simona Valkovská, Tomáš Hobza <xhobza03@vutbr.cz>
 *
 * Project: IFJ compiler
 */

#ifndef IFJ23_ERROR_H
#define IFJ23_ERROR_H

#include <stdlib.h>
#include "colorful_printf.h"
#include "scanner.h"

typedef enum
{
    NO_ERR = EXIT_SUCCESS, // žádná chyba
    LEXICAL_ERR = 1,       // chyba v programu v rámci lexikální analýzy (chybná struktura aktuálního lexému)
    SYNTACTIC_ERR = 2,     // chyba v programu v rámci syntaktické analýzy (chybná syntaxe programu, chybějící hlavička, atp.)
    FUNCTIONS_ERR = 3,     // sémantická chyba v programu – nedefinovaná funkce, redefinice proměnné
    PARAM_TYPE_ERR = 4,    // sémantická chyba v programu – špatný počet/typ parametrů u volání funkce či špatný typ návratové hodnoty z funkce
    VARIABLES_ERR = 5,     // použití nedefinované nebo neinicializované proměnné
    RETURN_ERR = 6,        // chybějící/přebývající výraz v příkazu návratu z funkce
    COMPATIBILITY_ERR = 7, // sémantická chyba typové kompatibility v aritmetických, řetězcových a relačních výrazech
    TYPE_ERR = 8,          // sémantická chyba odvození typu – typ proměnné nebo parametru není uveden a nelze odvodit od použitého výrazu
    SEMANTICS_ERR = 9,     // ostatní sémantické chyby
    INTERNAL_ERR = 99      // interní chyba překladače tj. neovlivněná vstupním programem (např. chyba alokace paměti atd.)
} Error_code;

#include "stack.h"

/**
 * @brief Structure for storing error information.
 *
 */
typedef struct
{
    Error_code code;
    unsigned int line_num;
    char *message;
} Error;

DECLARE_STACK_FUNCTIONS(Error)

/**
 * @brief Macro for throwing an error (pushing it into the error stack).
 *
 */
#define throw_error(code, line_num, fmt, ...)                 \
    do                                                        \
    {                                                         \
        char *formatted_message = malloc(sizeof(char) * 256); \
        snprintf(formatted_message, 256, fmt, ##__VA_ARGS__); \
        throw_error_base(code, formatted_message, line_num);  \
    } while (0);

/**
 * @brief Adds an error structure to the global error stack.
 *
 * @param code error code
 * @param message message to go along with the error
 * @param line_num the line number where the error situates
 * @param message message to go along with the error
 */
void throw_error_base(Error_code code, char *message, int line_num);

/**
 * @brief Sorts and prints all the errors in the stack ordered by the line number.
 *
 */
Error_code print_errors();

/**
 * @brief Prints the error structure in a human readable format.
 *
 * @param error error structure to print
 */
void printError(Error error);

/**
 * @brief Prints the error code in a human readable format.
 *
 * @param code error code to print
 */
void printErrorCode(Error_code code);

#endif // IFJ23_ERROR_H