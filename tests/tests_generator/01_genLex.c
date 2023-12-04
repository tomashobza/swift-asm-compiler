#include <assert.h>
#include "lexer.h" // Nahraďte vlastním souborem, který obsahuje lexer
#include "token.h" // Nahraďte vlastním souborem, který obsahuje definice tokenů

void testLexicalAnalysis()
{
    char *sourceCode = "int a = 5;";
    TokenList *tokens = lexicalAnalysis(sourceCode);

    assert(tokens != NULL);
    assert(tokens->count == 4); // Předpokládáme 4 tokeny: int, a, =, 5
    assert(tokens->tokens[0].type == TOKEN_TYPE_INT);
    assert(tokens->tokens[1].type == TOKEN_TYPE_IDENTIFIER);
    assert(tokens->tokens[2].type == TOKEN_TYPE_ASSIGN);
    assert(tokens->tokens[3].type == TOKEN_TYPE_NUMBER);

    // Uvolnění paměti a další čistící operace
}

int main()
{
    testLexicalAnalysis();
    // další testy...
    return 0;
}