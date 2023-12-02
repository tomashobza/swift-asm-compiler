/**
 * @file generator.c
 * @author Tomáš Hobza (xhobza03@vutbr.cz), Jakub Všetečka (xvsete00@vutbr.cz)
 * @brief Generator of IFJcode20.
 * @version 0.1
 * @date 2023-11-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "generator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO: napsat funkci, ktera bere token (token.type je jeho typ a token.token_value je string s jeho hodnotou) a vraci string s jeho hodnotou ve formatu pro IFJcode23
// ⟨symb⟩ konstantu nebo proměnnou
//. Identifikátor proměnné se skládá ze dvou částí oddělených zavináčem (znak @; bez bílých znaků), označení rámce LF, TF nebo GF a samotného jména proměnné (sekvence libovolných alfanumerických
// a speciálních znaků bez bílých znaků začínající písmenem nebo speciálním znakem, kde speciální znaky jsou: _, -, $, &, %, *, !, ?). Např. GF@_x značí proměnnou _x uloženou
// v globálním rámci.

// float konstanty prevedeme na hexadecimalni format pomoci %a

// Formating the token for IFJcode23
char *format_token_for_IFJcode23(Token *token)
{
    char *formatted_value;

    switch (token->type)
    {
    case TOKEN_IDENTIFICATOR:
    {
        formatted_value = malloc(strlen(token->token_value) + 12); // Including "identifier@" and '\0'
        sprintf(formatted_value, "identifier@%s", token->token_value);
    }
    case TOKEN_INT:
    {
        // Format integer literals with "int@"
        formatted_value = malloc(strlen(token->token_value) + 5); //"int@" and '\0'
        sprintf(formatted_value, "int@%s", token->token_value);
    }
    case TOKEN_DOUBLE:
    {
        // Format floating-point literals with "float@"
        double double_value = atof(token->token_value); // Convert to double
        formatted_value = malloc(sizeof(char) * 60);    // Allocating enough space
        sprintf(formatted_value, "float@%a", double_value);
    }
    case TOKEN_STRING:
    {
        // Format string literals with "string@"
        formatted_value = malloc(strlen(token->token_value) + 8); //"string@" and '\0'
        sprintf(formatted_value, "string@%s", token->token_value);
    }
    case TOKEN_NIL:
    {
        // Format nil with "nil@"
        formatted_value = strdup("nil@");
    }
    default:
    {
        // Format other tokens with their value
        formatted_value = strdup(token->token_value);
    }
    }

    return formatted_value;
}
