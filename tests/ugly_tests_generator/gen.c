/**
 * @file gen.c
 * @brief Parser of project
 * @details This module is used for parsing tokens from scanner and generating code in IFJcode23
 * @author Jakub Všetečka <xvsete00@vutbr.cz>
 *
 * Project: IFJ compiler
 */

#include "gen.h"

int indentationLevel = 0;
#define PRINT_FUNC_NAME() fprintf(stderr, CYAN "%s\n" RESET, __func__);

void generate_func_call()
{
    // print random builtin function
    int rand_builtin = rand() % 9;
    switch (rand_builtin)
    {
        // readString() -> String?
    case 0:
        printf("readString()");
        break;
        // readInt() -> Int?
    case 1:
        printf("readInt()");
        break;
        // readDouble() -> Double?
    case 2:
        printf("readDouble()");
        break;
        // write ( term1 , term2 , …, term𝑛 )
    case 3:
        printf("write(\"tvoje\", \"mama\", \"je\", \"tak\", \"tlusta\")");
        break;
        // Int2Double(_ term ∶ Int) -> Double
    case 4:
        printf("Int2Double(69)");
        break;
        // Double2Int(_ term ∶ Double) -> Int
    case 5:
        printf("Double2Int(69.69)");
        break;
        // length(_ 𝑠 : String) -> Int
    case 6:
        printf("length(\"tvoje mama je tak tlusta\")");
        break;
        // substring(of 𝑠 : String, startingAt 𝑖 : Int, endingBefore 𝑗 : Int) -> String?
    case 7:
        printf("substring(of : \"tvoje mama je tak tlusta\", startingAt : 0, endingBefore : 4)");
        break;
        // ord(_ 𝑐 : String) -> Int
    case 8:
        printf("ord(\"tvoje mama je tak tlusta\")");
        break;
        // chr(_ 𝑖 : Int) -> String
    case 9:
        printf("chr(69)");
        break;
    }
}

void shuffleTokens(Token_type *tokens, int size)
{
    for (int i = size - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        Token_type temp = tokens[i];
        tokens[i] = tokens[j];
        tokens[j] = temp;
    }
}

void printIndentation()
{
    for (int i = 0; i < indentationLevel; i++)
    {
        printf("    "); // Předpokládá se odsazení čtyřmi mezerami
    }
}

void token_type_print(Token_type type)
{
    switch (type)
    {
    case TOKEN_IF:
        printIndentation();
        printf("if ");
        break; // Keyword if 0
    case TOKEN_ELSE:
        printIndentation();
        printf("else ");
        break; // Keyword else 1
    case TOKEN_WHILE:
        printIndentation();
        printf("while ");
        break; // Keyword while 2
    case TOKEN_RETURN:
        printIndentation();
        printf("return ");
        break; // Keyword return 3
    case TOKEN_VAR:
        printf("var ");
        break; // Keyword var 4
    case TOKEN_LET:
        printf("let ");
        break; // Keyword let 5
    case TOKEN_BREAK:
        printf("break");
        break; // Keyword break 6
    case TOKEN_CONTINUE:
        printf("continue");
        break; // Keyword continue 7
    case TOKEN_TYPE_STRING:
        printf("String");
        break; // Keyword String 8
    case TOKEN_TYPE_INT:
        printf("Int");
        break; // Keyword Int 9
    case TOKEN_TYPE_DOUBLE:
        printf("Double");
        break; // Keyword Double 10
    case TOKEN_TYPE_BOOL:
        printf("Bool");
        break; // Keyword Boola 11
    case TOKEN_TYPE_STRING_NIL:
        printf("String?");
        break; // Keyword String? 12
    case TOKEN_TYPE_INT_NIL:
        printf("Int?");
        break; // Keyword Int? 13
    case TOKEN_TYPE_DOUBLE_NIL:
        printf("Double?");
        break; // Keyword Double? 14
    case TOKEN_TYPE_BOOL_NIL:
        printf("Bool?");
        break; // Keyword Bool? 15
    case TOKEN_FUNC:
        printIndentation();
        printf("func ");
        break; // Keyword func 16
    case TOKEN_IDENTIFICATOR:
        printf("x");
        break; // Identificator 17
    case TOKEN_EOF:
        fprintf(stderr, "EOF\n");
        printf(" ");
        break; // EOF 18
    case TOKEN_INT:
        printf("69");
        break; // Integer 19
    case TOKEN_DOUBLE:
        printf("69.240");
        break; // Decimal number 20
    case TOKEN_BOOL:
        printf("true");
        break;
        // Bool value 21;
    case TOKEN_EXP:
        printf("5 + 5");
        break; // Exponent 22
    case TOKEN_FUNC_CALL:
        generate_func_call();
        break; // Function call foo() 23
    case TOKEN_STRING:
        printf("\"tvoje máma je vole\"");
        break; // String 23
    case TOKEN_EQ:
        printf(" == ");
        break; // Equals == 24
    case TOKEN_NEQ:
        printf(" !== ");
        break; // Not equal !== 25
    case TOKEN_LESS:
        printf(" < ");
        break; // Less than < 26
    case TOKEN_MORE:
        printf(" > ");
        break; // More than > 27
    case TOKEN_MORE_EQ:
        printf(" >= ");
        break; // More than or equal >= 28
    case TOKEN_LESS_EQ:
        printf(" <= ");
        break; // Less than or equal <= 29
    case TOKEN_PLUS:
        printf(" + ");
        break; // Plus + 30
    case TOKEN_MINUS:
        printf(" - ");
        break; // Minus - 31
    case TOKEN_MUL:
        printf(" * ");
        break; // Multiply * 32
    case TOKEN_DIV:
        printf(" / ");
        break; // Divide / 33
    case TOKEN_BINARY_OPERATOR:
        printf(" ?? ");
        break; // Binary operator ?? 34
    case TOKEN_ASSIGN:
        printf(" = ");
        break; // Assign = 35
    case TOKEN_L_BRACKET:
        printf(" ( ");
        break; // Left bracket ( 36
    case TOKEN_R_BRACKET:
        printf(" ) ");
        break; // Right bracket ) 37
    case TOKEN_R_CURLY:
        indentationLevel--;
        printIndentation();
        printf("}\n");
        break; // Right bracket } 38
    case TOKEN_L_CURLY:
        printf(" {\n");
        indentationLevel++;
        break; // Left bracket { 39
    case TOKEN_COMMA:
        printf(", ");
        break; // Comma , 40
    case TOKEN_ARROW:
        printf(" -> ");
        break; // Arrow -> 41
    case TOKEN_NIL:
        printf(" nil");
        break; // nil has been read 42
    case TOKEN_DOUBLE_DOT:
        printf(" : ");
        break; // : has been read 43
    case TOKEN_NOT:
        printf("!");
        break; // ! has been read 44
    case TOKEN_AND:
        printf(" &&");
        break; // && has been read 45
    case TOKEN_OR:
        printf(" ||");
        break; // || has been read 46
    case TOKEN_UNDERSCORE:
        printf(" _");
        break; // _ has been read 47
    case TOKEN_FUNC_ID:
        printf("foo");
        break; // func_id 48
    case TOKEN_EXPRSN:
        printf("5 + 5");
        break; // E 49
    case TOKEN_SHIFT:
        printf(" <");
        break; // < 50
    case TOKEN_UNSHIFT:
        printf(" >");
        break; // > 51
    }
}

bool cmp_type(Token_type type)
{
    // printf("\nIndetation lvl: %d\n", indentationLevel);
    token_type_print(type);
    // usleep(10000);
    return true;
}

bool START()
{
    PRINT_FUNC_NAME();
    Token_type availableTokens[] = {
        // TOKEN_EOF,
        TOKEN_FUNC,
        TOKEN_IF,
        TOKEN_IDENTIFICATOR,
        TOKEN_FUNC_ID,
        TOKEN_WHILE,
        TOKEN_VAR,
        TOKEN_LET,
    };
    int numAvailableTokens = sizeof(availableTokens) / sizeof(Token_type);

    shuffleTokens(availableTokens, numAvailableTokens);

    // Select the first token after shuffle
    Token_type selectedToken = availableTokens[0];
    fprintf(stderr, RED "%d\n" RESET, selectedToken);

    switch (selectedToken)
    {
    // START -> STMT_LIST eof
    case TOKEN_EOF:
    case TOKEN_FUNC:
    case TOKEN_IF:
    case TOKEN_IDENTIFICATOR:
    case TOKEN_FUNC_ID:
    case TOKEN_WHILE:
    case TOKEN_VAR:
    case TOKEN_LET:
        return STMT_LIST() && cmp_type(TOKEN_EOF);
    default:
        return false;
    }
}

bool STMT_LIST()
{
    PRINT_FUNC_NAME();
    Token_type availableTokens[] = {
        TOKEN_EOF,
        TOKEN_R_CURLY,
        TOKEN_R_CURLY,
        TOKEN_FUNC,
        TOKEN_IF,
        TOKEN_IDENTIFICATOR,
        TOKEN_FUNC_ID,
        TOKEN_WHILE,
        TOKEN_VAR,
        TOKEN_LET,
    };
    int numAvailableTokens = sizeof(availableTokens) / sizeof(Token_type);

    shuffleTokens(availableTokens, numAvailableTokens);

    // Select the first token after shuffle
    Token_type selectedToken = availableTokens[0];
    fprintf(stderr, RED "%d\n" RESET, selectedToken);

    switch (selectedToken)
    {
    // STMT_LIST -> eps
    case TOKEN_EOF:
    case TOKEN_R_CURLY:
        return true;
    // STMT_LIST -> STMT STMT_LIST
    case TOKEN_FUNC:
    case TOKEN_IF:
    case TOKEN_IDENTIFICATOR:
    case TOKEN_FUNC_ID:
    case TOKEN_WHILE:
    case TOKEN_VAR:
    case TOKEN_LET:
        return STMT() && STMT_LIST();
    default:
        return false;
    }
}
bool LOCAL_STMT_LIST()
{
    PRINT_FUNC_NAME();
    Token_type availableTokens[] = {
        TOKEN_EOF,
        TOKEN_R_CURLY,
        TOKEN_R_CURLY,
        TOKEN_IF,
        TOKEN_IDENTIFICATOR,
        TOKEN_FUNC_ID,
        TOKEN_WHILE,
        TOKEN_VAR,
        TOKEN_LET,
    };
    int numAvailableTokens = sizeof(availableTokens) / sizeof(Token_type);

    shuffleTokens(availableTokens, numAvailableTokens);

    // Select the first token after shuffle
    Token_type selectedToken = availableTokens[0];
    fprintf(stderr, RED "%d\n" RESET, selectedToken);

    switch (selectedToken)
    {
    // STMT_LIST -> eps
    case TOKEN_EOF:
    case TOKEN_R_CURLY:
        return true;
    // STMT_LIST -> STMT STMT_LIST
    case TOKEN_IF:
    case TOKEN_IDENTIFICATOR:
    case TOKEN_FUNC_ID:
    case TOKEN_WHILE:
    case TOKEN_VAR:
    case TOKEN_LET:
        return LOCAL_STMT() && LOCAL_STMT_LIST();
    default:
        return false;
    }
}

bool STMT()
{
    PRINT_FUNC_NAME();
    Token_type availableTokens[] = {
        TOKEN_FUNC,
        TOKEN_IF,
        TOKEN_IDENTIFICATOR,
        TOKEN_FUNC_ID,
        TOKEN_WHILE,
        TOKEN_VAR,
        TOKEN_LET,
    };
    int numAvailableTokens = sizeof(availableTokens) / sizeof(Token_type);

    shuffleTokens(availableTokens, numAvailableTokens);

    // Select the first token after shuffle
    Token_type selectedToken = availableTokens[0];
    fprintf(stderr, RED "%d\n" RESET, selectedToken);

    switch (selectedToken)
    {
    // STMT -> DEF_FUNC
    case TOKEN_FUNC:
        return DEF_FUNC();
    // STMT -> IF_STMT
    case TOKEN_IF:
        return IF_STMT();
    // STMT -> LOAD_ID
    case TOKEN_IDENTIFICATOR:
    case TOKEN_FUNC_ID:
        return LOAD_ID();
    // STMT -> WHILE_STMT
    case TOKEN_WHILE:
        return WHILE_STMT();
    // STMT -> VAR_LET
    case TOKEN_VAR:
        return VAR_LET();
    // STMT -> VAR_LET
    case TOKEN_LET:
        return VAR_LET();
    default:
        return false;
    }
}
bool LOCAL_STMT()
{
    PRINT_FUNC_NAME();
    Token_type availableTokens[] = {
        TOKEN_IF,
        TOKEN_IDENTIFICATOR,
        TOKEN_FUNC_ID,
        TOKEN_WHILE,
        TOKEN_VAR,
        TOKEN_LET,
    };
    int numAvailableTokens = sizeof(availableTokens) / sizeof(Token_type);

    shuffleTokens(availableTokens, numAvailableTokens);

    // Select the first token after shuffle
    Token_type selectedToken = availableTokens[0];
    fprintf(stderr, RED "%d\n" RESET, selectedToken);

    switch (selectedToken)
    {
    // STMT -> IF_STMT
    case TOKEN_IF:
        return IF_STMT();
    // STMT -> LOAD_ID
    case TOKEN_IDENTIFICATOR:
    case TOKEN_FUNC_ID:
        return LOAD_ID();
    // STMT -> WHILE_STMT
    case TOKEN_WHILE:
        return WHILE_STMT();
    // STMT -> VAR_LET
    case TOKEN_VAR:
        return VAR_LET();
    // STMT -> VAR_LET
    case TOKEN_LET:
        return VAR_LET();
    default:
        return false;
    }
}

bool VAR_LET()
{
    PRINT_FUNC_NAME();
    Token_type availableTokens[] = {
        TOKEN_VAR,
        TOKEN_LET,
    };
    int numAvailableTokens = sizeof(availableTokens) / sizeof(Token_type);

    shuffleTokens(availableTokens, numAvailableTokens);

    // Select the first token after shuffle
    Token_type selectedToken = availableTokens[0];
    fprintf(stderr, RED "%d\n" RESET, selectedToken);

    printIndentation();

    switch (selectedToken)
    {
    // VAR_LET -> VAR_SCOPE id TYPE_AND_ASSIGN
    case TOKEN_VAR:
    case TOKEN_LET:
        bool res = VAR_SCOPE() && cmp_type(TOKEN_IDENTIFICATOR) && TYPE_AND_ASIGN();
        printf("\n");
        return res;
    default:
        return false;
    }
}

bool VAR_SCOPE()
{
    PRINT_FUNC_NAME();
    Token_type availableTokens[] = {
        TOKEN_VAR,
        TOKEN_LET,
    };
    int numAvailableTokens = sizeof(availableTokens) / sizeof(Token_type);

    shuffleTokens(availableTokens, numAvailableTokens);

    // Select the first token after shuffle
    Token_type selectedToken = availableTokens[0];
    fprintf(stderr, RED "%d\n" RESET, selectedToken);

    switch (selectedToken)
    {
    case TOKEN_VAR:
        return cmp_type(TOKEN_VAR);
    case TOKEN_LET:
        return cmp_type(TOKEN_LET);
    default:
        return false;
    }
}

bool TYPE_AND_ASIGN()
{
    PRINT_FUNC_NAME();
    Token_type availableTokens[] = {
        TOKEN_DOUBLE_DOT,
        TOKEN_ASSIGN,
    };
    int numAvailableTokens = sizeof(availableTokens) / sizeof(Token_type);

    shuffleTokens(availableTokens, numAvailableTokens);

    // Select the first token after shuffle
    Token_type selectedToken = availableTokens[0];
    fprintf(stderr, RED "%d\n" RESET, selectedToken);

    switch (selectedToken)
    {
    // TYPE_AND_ASIGN -> : D_TYPE R_FLEX
    case TOKEN_DOUBLE_DOT:
        return cmp_type(TOKEN_DOUBLE_DOT) && D_TYPE() && R_FLEX();
    // TYPE_AND_ASIGN -> = EXP
    case TOKEN_ASSIGN:
        return cmp_type(TOKEN_ASSIGN) && EXP(selectedToken);
    default:
        return false;
    }
}

bool R_FLEX()
{
    PRINT_FUNC_NAME();
    Token_type availableTokens[] = {
        TOKEN_EOF,
        TOKEN_IDENTIFICATOR,
        TOKEN_FUNC_ID,
        TOKEN_VAR,
        TOKEN_LET,
        TOKEN_FUNC,
        TOKEN_R_CURLY,
        TOKEN_RETURN,
        TOKEN_WHILE,
        TOKEN_IF,
    };
    int numAvailableTokens = sizeof(availableTokens) / sizeof(Token_type);

    shuffleTokens(availableTokens, numAvailableTokens);

    // Select the first token after shuffle
    Token_type selectedToken = availableTokens[0];
    fprintf(stderr, RED "%d\n" RESET, selectedToken);

    switch (selectedToken)
    {
    // R_FLEX -> = EXP
    case TOKEN_ASSIGN:
        return cmp_type(TOKEN_ASSIGN) && EXP(selectedToken);
    // R_FLEX -> eps
    case TOKEN_EOF:
    case TOKEN_IDENTIFICATOR:
    case TOKEN_FUNC_ID:
    case TOKEN_VAR:
    case TOKEN_LET:
    case TOKEN_FUNC:
    case TOKEN_R_CURLY:
    case TOKEN_RETURN:
    case TOKEN_WHILE:
    case TOKEN_IF:
        return true;
    default:
        return false;
    }
}

bool D_TYPE()
{
    PRINT_FUNC_NAME();
    Token_type availableTokens[] = {
        TOKEN_TYPE_STRING,
        TOKEN_TYPE_INT,
        TOKEN_TYPE_DOUBLE,
        TOKEN_TYPE_BOOL,
        TOKEN_TYPE_STRING_NIL,
        TOKEN_TYPE_INT_NIL,
        TOKEN_TYPE_DOUBLE_NIL,
        TOKEN_TYPE_BOOL_NIL,
    };
    int numAvailableTokens = sizeof(availableTokens) / sizeof(Token_type);

    shuffleTokens(availableTokens, numAvailableTokens);

    // Select the first token after shuffle
    Token_type selectedToken = availableTokens[0];
    fprintf(stderr, RED "%d\n" RESET, selectedToken);

    switch (selectedToken)
    {
    case TOKEN_TYPE_STRING:
        return cmp_type(TOKEN_TYPE_STRING);
    case TOKEN_TYPE_INT:
        return cmp_type(TOKEN_TYPE_INT);
    case TOKEN_TYPE_DOUBLE:
        return cmp_type(TOKEN_TYPE_DOUBLE);
    case TOKEN_TYPE_BOOL:
        return cmp_type(TOKEN_TYPE_BOOL);
    case TOKEN_TYPE_STRING_NIL:
        return cmp_type(TOKEN_TYPE_STRING_NIL);
    case TOKEN_TYPE_INT_NIL:
        return cmp_type(TOKEN_TYPE_INT_NIL);
    case TOKEN_TYPE_DOUBLE_NIL:
        return cmp_type(TOKEN_TYPE_DOUBLE_NIL);
    case TOKEN_TYPE_BOOL_NIL:
        return cmp_type(TOKEN_TYPE_BOOL_NIL);
    default:
        return false;
    }
}

bool DEF_FUNC()
{
    PRINT_FUNC_NAME();
    Token_type availableTokens[] = {
        TOKEN_FUNC,
    };
    int numAvailableTokens = sizeof(availableTokens) / sizeof(Token_type);

    shuffleTokens(availableTokens, numAvailableTokens);

    // Select the first token after shuffle
    Token_type selectedToken = availableTokens[0];
    fprintf(stderr, RED "%d\n" RESET, selectedToken);

    switch (selectedToken)
    {
    //  DEF_FUNC -> func func_id ( P_LIST ) RET_TYPE { FUNC_STMT_LIST }
    case TOKEN_FUNC:
        return cmp_type(TOKEN_FUNC) && cmp_type(TOKEN_FUNC_ID) && cmp_type(TOKEN_L_BRACKET) &&
               P_LIST() && cmp_type(TOKEN_R_BRACKET) && RET_TYPE() && cmp_type(TOKEN_L_CURLY) &&
               FUNC_STMT_LIST() && cmp_type(TOKEN_R_CURLY);
    default:
        return false;
    }
}

bool P_LIST()
{
    PRINT_FUNC_NAME();
    Token_type availableTokens[] = {
        TOKEN_IDENTIFICATOR,
        TOKEN_R_BRACKET,
    };
    int numAvailableTokens = sizeof(availableTokens) / sizeof(Token_type);

    shuffleTokens(availableTokens, numAvailableTokens);

    // Select the first token after shuffle
    Token_type selectedToken = availableTokens[0];
    fprintf(stderr, RED "%d\n" RESET, selectedToken);

    switch (selectedToken)
    {
    // P_LIST -> PARAM
    case TOKEN_IDENTIFICATOR:
        return PARAM();
    // P_LIST -> eps
    case TOKEN_R_BRACKET:
        return true;
    default:
        return false;
    }
}

bool PARAM()
{
    PRINT_FUNC_NAME();
    Token_type availableTokens[] = {
        TOKEN_IDENTIFICATOR,
    };
    int numAvailableTokens = sizeof(availableTokens) / sizeof(Token_type);

    shuffleTokens(availableTokens, numAvailableTokens);

    // Select the first token after shuffle
    Token_type selectedToken = availableTokens[0];
    fprintf(stderr, RED "%d\n" RESET, selectedToken);

    switch (selectedToken)
    {
    // PARAM -> id id : D_TYPE SEP
    case TOKEN_IDENTIFICATOR:
        bool p1 = cmp_type(TOKEN_IDENTIFICATOR);
        printf(" ");
        bool p2 = cmp_type(TOKEN_IDENTIFICATOR) && cmp_type(TOKEN_DOUBLE_DOT) && D_TYPE() && P_SEP();
        return p1 && p2;
    default:
        return false;
    }
}

bool P_SEP()
{
    PRINT_FUNC_NAME();
    Token_type availableTokens[] = {
        TOKEN_COMMA,
        TOKEN_R_BRACKET,
    };
    int numAvailableTokens = sizeof(availableTokens) / sizeof(Token_type);

    shuffleTokens(availableTokens, numAvailableTokens);

    // Select the first token after shuffle
    Token_type selectedToken = availableTokens[0];
    fprintf(stderr, RED "%d\n" RESET, selectedToken);

    switch (selectedToken)
    {
    // P_SEP -> eps
    case TOKEN_R_BRACKET:
        return true;
    // P_SEP -> , PARAM
    case TOKEN_COMMA:
        return cmp_type(TOKEN_COMMA) && PARAM();
    default:
        return false;
    }
}

bool RET_TYPE()
{
    PRINT_FUNC_NAME();
    Token_type availableTokens[] = {
        TOKEN_L_CURLY,
        TOKEN_ARROW,
    };
    int numAvailableTokens = sizeof(availableTokens) / sizeof(Token_type);

    shuffleTokens(availableTokens, numAvailableTokens);

    // Select the first token after shuffle
    Token_type selectedToken = availableTokens[0];
    fprintf(stderr, RED "%d\n" RESET, selectedToken);

    switch (selectedToken)
    {
    // RET_TYPE -> eps
    case TOKEN_L_CURLY:
        return true;
    // RET_TYPE -> -> D_TYPE
    case TOKEN_ARROW:
        return cmp_type(TOKEN_ARROW) && D_TYPE();
    default:
        return false;
    }
}

bool FUNC_STMT_LIST()
{
    PRINT_FUNC_NAME();
    Token_type availableTokens[] = {
        TOKEN_IDENTIFICATOR,
        TOKEN_FUNC_ID,
        TOKEN_VAR,
        TOKEN_LET,
        TOKEN_RETURN,
        TOKEN_WHILE,
        TOKEN_IF,
        TOKEN_R_CURLY,
        TOKEN_R_CURLY,
        TOKEN_R_CURLY,
    };
    int numAvailableTokens = sizeof(availableTokens) / sizeof(Token_type);

    shuffleTokens(availableTokens, numAvailableTokens);

    // Select the first token after shuffle
    Token_type selectedToken = availableTokens[0];
    fprintf(stderr, RED "%d\n" RESET, selectedToken);

    switch (selectedToken)
    {
    // FUNC_STMT_LIST -> FUNC_STMT FUNC_STMT_LIST
    case TOKEN_IDENTIFICATOR:
    case TOKEN_FUNC_ID:
    case TOKEN_VAR:
    case TOKEN_LET:
    case TOKEN_RETURN:
    case TOKEN_WHILE:
    case TOKEN_IF:
        return FUNC_STMT() && FUNC_STMT_LIST();
    // FUNC_STMT_LIST -> eps
    case TOKEN_R_CURLY:
        return true;
    default:
        return false;
    }
}

bool FUNC_STMT()
{
    PRINT_FUNC_NAME();
    Token_type availableTokens[] = {
        TOKEN_IDENTIFICATOR,
        TOKEN_FUNC_ID,
        TOKEN_VAR,
        TOKEN_LET,
        TOKEN_RETURN,
        TOKEN_WHILE,
        TOKEN_IF,
    };
    int numAvailableTokens = sizeof(availableTokens) / sizeof(Token_type);

    shuffleTokens(availableTokens, numAvailableTokens);

    // Select the first token after shuffle
    Token_type selectedToken = availableTokens[0];
    fprintf(stderr, RED "%d\n" RESET, selectedToken);

    switch (selectedToken)
    {
    // FUNC_STMT -> LOAD_ID
    case TOKEN_IDENTIFICATOR:
    case TOKEN_FUNC_ID:
        return LOAD_ID();
    // FUNC_STMT -> VAR_LET
    case TOKEN_VAR:
        return VAR_LET();
    case TOKEN_LET:
        return VAR_LET();
    // FUNC_STMT -> RET
    case TOKEN_RETURN:
        return RET();
    // FUNC_STMT -> FUNC_WHILE
    case TOKEN_WHILE:
        return FUNC_WHILE();
    // FUNC_STMT -> FUNC_IF
    case TOKEN_IF:
        return FUNC_IF();
    default:
        return false;
    }
}

bool RET()
{
    PRINT_FUNC_NAME();
    Token_type availableTokens[] = {
        TOKEN_RETURN,
    };
    int numAvailableTokens = sizeof(availableTokens) / sizeof(Token_type);

    shuffleTokens(availableTokens, numAvailableTokens);

    // Select the first token after shuffle
    Token_type selectedToken = availableTokens[0];
    fprintf(stderr, RED "%d\n" RESET, selectedToken);

    switch (selectedToken)
    {
    // RET -> return EXP
    case TOKEN_RETURN:
        bool res = cmp_type(TOKEN_RETURN) && EXP(selectedToken);
        printf("\n");
        return res;
    default:
        return false;
    }
}

bool FUNC_WHILE()
{
    PRINT_FUNC_NAME();
    Token_type availableTokens[] = {
        TOKEN_WHILE,
    };
    int numAvailableTokens = sizeof(availableTokens) / sizeof(Token_type);

    shuffleTokens(availableTokens, numAvailableTokens);

    // Select the first token after shuffle
    Token_type selectedToken = availableTokens[0];
    fprintf(stderr, RED "%d\n" RESET, selectedToken);

    switch (selectedToken)
    {
    // FUNC_WHILE -> while EXP { FUNC_STMT_LIST }
    case TOKEN_WHILE:
        return cmp_type(TOKEN_WHILE) && EXP(selectedToken) && cmp_type(TOKEN_L_CURLY) &&
               FUNC_STMT_LIST() && cmp_type(TOKEN_R_CURLY);
    default:
        return false;
    }
}

bool FUNC_IF()
{
    PRINT_FUNC_NAME();
    Token_type availableTokens[] = {
        TOKEN_IF,
    };
    int numAvailableTokens = sizeof(availableTokens) / sizeof(Token_type);

    shuffleTokens(availableTokens, numAvailableTokens);

    // Select the first token after shuffle
    Token_type selectedToken = availableTokens[0];
    fprintf(stderr, RED "%d\n" RESET, selectedToken);

    switch (selectedToken)
    {
    // 	FUNC_IF -> if IF_COND { FUNC_STMT_LIST } FUNC_ELSE_CLAUSE
    case TOKEN_IF:
        return cmp_type(TOKEN_IF) && IF_COND() && cmp_type(TOKEN_L_CURLY) &&
               FUNC_STMT_LIST() && cmp_type(TOKEN_R_CURLY) && FUNC_ELSE_CLAUSE();
    default:
        return false;
    }
}

bool FUNC_ELSE_IF()
{
    PRINT_FUNC_NAME();
    Token_type availableTokens[] = {
        TOKEN_IF,
    };
    int numAvailableTokens = sizeof(availableTokens) / sizeof(Token_type);

    shuffleTokens(availableTokens, numAvailableTokens);

    // Select the first token after shuffle
    Token_type selectedToken = availableTokens[0];
    fprintf(stderr, RED "%d\n" RESET, selectedToken);

    switch (selectedToken)
    {
    // 	FUNC_IF -> if IF_COND { FUNC_STMT_LIST } FUNC_ELSE_CLAUSE
    case TOKEN_IF:
        return cmp_type(TOKEN_IF) && IF_COND() && cmp_type(TOKEN_L_CURLY) &&
               FUNC_STMT_LIST() && cmp_type(TOKEN_R_CURLY) && FUNC_ELSE_CLAUSE();
    default:
        return false;
    }
}

bool FUNC_ELSE_CLAUSE()
{
    PRINT_FUNC_NAME();
    Token_type availableTokens[] = {
        TOKEN_IDENTIFICATOR,
        TOKEN_FUNC_ID,
        TOKEN_VAR,
        TOKEN_LET,
        TOKEN_R_CURLY,
        TOKEN_RETURN,
        TOKEN_WHILE,
        TOKEN_IF,
        TOKEN_ELSE,
    };
    int numAvailableTokens = sizeof(availableTokens) / sizeof(Token_type);

    shuffleTokens(availableTokens, numAvailableTokens);

    // Select the first token after shuffle
    Token_type selectedToken = availableTokens[0];
    fprintf(stderr, RED "%d\n" RESET, selectedToken);

    switch (selectedToken)
    {
    // FUNC_ELSE_CLAUSE -> eps
    case TOKEN_IDENTIFICATOR:
    case TOKEN_FUNC_ID:
        return true;
    case TOKEN_VAR:
        return true;
    case TOKEN_LET:
        return true;
    case TOKEN_R_CURLY:
        return true;
    case TOKEN_RETURN:
        return true;
    case TOKEN_WHILE:
        return true;
    case TOKEN_IF:
        return true;
    // FUNC_ELSE_CLAUSE -> else FUNC_AFTER_ELSE
    case TOKEN_ELSE:
        return cmp_type(TOKEN_ELSE) && FUNC_AFTER_ELSE();
    default:
        return false;
    }
}

bool FUNC_AFTER_ELSE()
{
    PRINT_FUNC_NAME();
    Token_type availableTokens[] = {
        TOKEN_L_CURLY,
        TOKEN_IF,
    };
    int numAvailableTokens = sizeof(availableTokens) / sizeof(Token_type);

    shuffleTokens(availableTokens, numAvailableTokens);

    // Select the first token after shuffle
    Token_type selectedToken = availableTokens[0];
    fprintf(stderr, RED "%d\n" RESET, selectedToken);

    switch (selectedToken)
    {
    // FUNC_AFTER_ELSE -> { FUNC_STMT_LIST }
    case TOKEN_L_CURLY:
        return cmp_type(TOKEN_L_CURLY) && FUNC_STMT_LIST() && cmp_type(TOKEN_R_CURLY);
    // FUNC_AFTER_ELSE -> FUNC_IF
    case TOKEN_IF:
        printf("\n");
        return FUNC_ELSE_IF();
    default:
        return false;
    }
}

bool IF_STMT()
{
    PRINT_FUNC_NAME();
    Token_type availableTokens[] = {
        TOKEN_IF,
    };
    int numAvailableTokens = sizeof(availableTokens) / sizeof(Token_type);

    shuffleTokens(availableTokens, numAvailableTokens);

    // Select the first token after shuffle
    Token_type selectedToken = availableTokens[0];
    fprintf(stderr, RED "%d\n" RESET, selectedToken);

    switch (selectedToken)
    {
    // IF_STMT -> if EXP { LOCAL_STMT_LIST } ELSE_CLAUSE
    case TOKEN_IF:
        return cmp_type(TOKEN_IF) && IF_COND() && cmp_type(TOKEN_L_CURLY) &&
               LOCAL_STMT_LIST() && cmp_type(TOKEN_R_CURLY) && ELSE_CLAUSE();
    default:
        return false;
    }
}

bool ELSE_IF_STMT()
{
    PRINT_FUNC_NAME();
    Token_type availableTokens[] = {
        TOKEN_IF,
    };
    int numAvailableTokens = sizeof(availableTokens) / sizeof(Token_type);

    shuffleTokens(availableTokens, numAvailableTokens);

    // Select the first token after shuffle
    Token_type selectedToken = availableTokens[0];
    fprintf(stderr, RED "%d\n" RESET, selectedToken);

    switch (selectedToken)
    {
    // IF_STMT -> if EXP { STMT_LIST } ELSE_CLAUSE
    case TOKEN_IF:
        return cmp_type(TOKEN_IF) && IF_COND() && cmp_type(TOKEN_L_CURLY) &&
               STMT_LIST() && cmp_type(TOKEN_R_CURLY) && ELSE_CLAUSE();
    default:
        return false;
    }
}

bool IF_COND()
{
    PRINT_FUNC_NAME();
    Token_type availableTokens[] = {
        TOKEN_IDENTIFICATOR,
        TOKEN_FUNC_ID,
        TOKEN_L_BRACKET,
        TOKEN_DOUBLE,
        TOKEN_INT,
        TOKEN_STRING,
        TOKEN_BOOL,
        TOKEN_NIL,
        TOKEN_EXP,
        TOKEN_NOT,
        TOKEN_LET,
    };
    int numAvailableTokens = sizeof(availableTokens) / sizeof(Token_type);

    shuffleTokens(availableTokens, numAvailableTokens);

    // Select the first token after shuffle
    Token_type selectedToken = availableTokens[0];
    fprintf(stderr, RED "%d\n" RESET, selectedToken);

    switch (selectedToken)
    {
    // IF_COND -> EXP
    case TOKEN_IDENTIFICATOR:
    case TOKEN_FUNC_ID:
    case TOKEN_L_BRACKET:
    case TOKEN_DOUBLE:
    case TOKEN_INT:
    case TOKEN_STRING:
    case TOKEN_BOOL:
    case TOKEN_NIL:
    case TOKEN_EXP:
    case TOKEN_NOT:
        return EXP(selectedToken);
    // IF_COND -> let id
    case TOKEN_LET:
        return cmp_type(TOKEN_LET) && cmp_type(TOKEN_IDENTIFICATOR);
    default:
        return false;
    }
}

bool ELSE_CLAUSE()
{
    PRINT_FUNC_NAME();
    Token_type availableTokens[] = {
        TOKEN_IDENTIFICATOR,
        TOKEN_FUNC_ID,
        TOKEN_VAR,
        TOKEN_LET,
        TOKEN_R_CURLY,
        TOKEN_WHILE,
        TOKEN_IF,
        TOKEN_ELSE,
        TOKEN_EOF,
    };
    int numAvailableTokens = sizeof(availableTokens) / sizeof(Token_type);

    shuffleTokens(availableTokens, numAvailableTokens);

    // Select the first token after shuffle
    Token_type selectedToken = availableTokens[0];
    fprintf(stderr, RED "%d\n" RESET, selectedToken);

    switch (selectedToken)
    {
    // ELSE_CLAUSE -> eps
    case TOKEN_IDENTIFICATOR:
    case TOKEN_FUNC_ID:
    case TOKEN_VAR:
    case TOKEN_LET:
    case TOKEN_FUNC:
    case TOKEN_R_CURLY:
    case TOKEN_WHILE:
    case TOKEN_IF:
    case TOKEN_EOF:
        return true;
    // ELSE_CLAUSE -> else AFTER_ELSE
    case TOKEN_ELSE:
        return cmp_type(TOKEN_ELSE) && AFTER_ELSE();
    default:
        return false;
    }
}

bool AFTER_ELSE()
{
    PRINT_FUNC_NAME();
    Token_type availableTokens[] = {
        TOKEN_L_CURLY,
        TOKEN_IF,
    };
    int numAvailableTokens = sizeof(availableTokens) / sizeof(Token_type);

    shuffleTokens(availableTokens, numAvailableTokens);

    // Select the first token after shuffle
    Token_type selectedToken = availableTokens[0];
    fprintf(stderr, RED "%d\n" RESET, selectedToken);

    switch (selectedToken)
    {
    // AFTER_ELSE -> { STMT_LIST }
    case TOKEN_L_CURLY:
        return cmp_type(TOKEN_L_CURLY) && LOCAL_STMT_LIST() && cmp_type(TOKEN_R_CURLY);
    // AFTER_ELSE -> IF_STMT
    case TOKEN_IF:
        return ELSE_IF_STMT();
    default:
        return false;
    }
}

bool WHILE_STMT()
{
    PRINT_FUNC_NAME();
    Token_type availableTokens[] = {
        TOKEN_WHILE,
    };
    int numAvailableTokens = sizeof(availableTokens) / sizeof(Token_type);

    shuffleTokens(availableTokens, numAvailableTokens);

    // Select the first token after shuffle
    Token_type selectedToken = availableTokens[0];
    fprintf(stderr, RED "%d\n" RESET, selectedToken);

    switch (selectedToken)
    {
    // WHILE_STMT -> while EXP { STMT_LIST }
    case TOKEN_WHILE:
        return cmp_type(TOKEN_WHILE) && EXP(selectedToken) && cmp_type(TOKEN_L_CURLY) &&
               LOCAL_STMT_LIST() && cmp_type(TOKEN_R_CURLY);
    default:
        return false;
    }
}

bool LOAD_ID()
{
    PRINT_FUNC_NAME();
    Token_type availableTokens[] = {
        TOKEN_IDENTIFICATOR,
        TOKEN_FUNC_ID,
    };
    int numAvailableTokens = sizeof(availableTokens) / sizeof(Token_type);

    shuffleTokens(availableTokens, numAvailableTokens);

    // Select the first token after shuffle
    Token_type selectedToken = availableTokens[0];
    fprintf(stderr, RED "%d\n" RESET, selectedToken);

    printIndentation();

    switch (selectedToken)
    {
    // LOAD_ID -> id = EXP
    case TOKEN_IDENTIFICATOR:
        bool res = cmp_type(TOKEN_IDENTIFICATOR) && cmp_type(TOKEN_ASSIGN) && EXP(selectedToken);
        printf("\n");
        return res;
    // LOAD_ID -> func_id
    case TOKEN_FUNC_ID:
        bool res2 = EXP(selectedToken);
        printf("\n");
        return res2;
    default:
        return false;
    }
}

bool EXP(Token_type type)
{
    PRINT_FUNC_NAME();
    switch (type)
    {
    case TOKEN_FUNC_ID:
        cmp_type(TOKEN_FUNC_CALL);
        break;
    default:
        cmp_type(TOKEN_EXP);
        break;
    }
    return true;
}

int main()
{
    struct timeval time;
    gettimeofday(&time, NULL);
    srand((time.tv_sec * 1000) + (time.tv_usec / 1000));

    START();
    return 0;
}