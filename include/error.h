/**
 * @file error.h
 * @brief error header file
 * @author Simona Valkovská
 *
 * Project: IFJ compiler
 */

#ifndef IFJ23_ERROR_H
#define IFJ23_ERROR_H

#include "colorful_printf.h"
#include "scanner.h"

/*
• 1- chybavprogramuvrámcilexikálníanalýzy(chybnástrukturaaktuálníholexému).
• 2 - chyba v programu v rámci syntaktické analýzy (chybná syntaxe programu, chy- bějící hlavička, atp.).
• 3 - sémantická chyba v programu – nedefinovaná funkce, redefinice proměnné.
• 4 - sémantická chyba v programu – špatný počet/typ parametrů u volání funkce či špatný typ návratové hodnoty z funkce.
• 5 - sémantická chyba v programu – použití nedefinované nebo neinicializované pro- měnné.
• 6 - sémantická chyba v programu – chybějící/přebývající výraz v příkazu návratu z funkce.
• 7 - sémantická chyba typové kompatibility v aritmetických, řetězcových a relačních výrazech.
• 8 - sémantická chyba odvození typu – typ proměnné nebo parametru není uveden a nelze odvodit od použitého výrazu.
• 9 - ostatní sémantické chyby.
• 99 - interní chyba překladače tj. neovlivněná vstupním programem (např. chyba alo- kace paměti atd.).
*/

typedef enum
{
    LEXICAL_ERR = 1,       // chyba v programu v rámci lexikální analýzy(chybná struktura aktuálního lexému)
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

typedef struct
{
    Error_code code;
    unsigned int line_num;
    char *message;
} Error;

DECLARE_STACK_FUNCTIONS(Error)

// Macro for formatted error messages
#define throw_error(code, fmt, ...)                                                 \
    do                                                                              \
    {                                                                               \
        char formatted_message[256];                                                \
        snprintf(formatted_message, sizeof(formatted_message), fmt, ##__VA_ARGS__); \
        throw_error_base(code, formatted_message);                                  \
    } while (0);

/**
 * @brief Adds an error structure to the global error stack.
 *
 * @param code error code
 * @param line_num the line number where the error situates
 * @param message message to go along with the error
 */
void throw_error_base(Error_code code, char *message);

/**
 * @brief Sorts and prints all the errors in the stack ordered by the line number.
 *
 */
void print_errors();

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