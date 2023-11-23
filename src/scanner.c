// TODO check comments
/**
 * @file scanner.c
 * @brief lexical analysis of the project
 * @author Simona Valkovská
 *
 * Project: IFJ compiler
 */

#include <stdio.h>
#include <string.h>

#include <stdlib.h>
#include <ctype.h>
#include "scanner.h"
#include "stack.h"

DEFINE_STACK_FUNCTIONS(Token)
#define CHAR_WTH_SPACE_LENGTH (sizeof(char_without_space) / sizeof(*char_without_space)) // length of array *char_without_space
Token_type state = NEW_TOKEN;                                                            // initial state of scanner
char *char_without_space[] = {":", ".", "{", "}", "(", ")", ",", " ", "=", "!", "+", "-", "*", "/", "<", ">", "\n", "_"};

int ret = 0;
unsigned int line_num = 0;
Token_stack *scanner_stack;

int main_scanner(Token *token)
{
    ret = 0;
    if (Token_stack_empty(scanner_stack))
    {
        char *code = "\0";
        ret = generate_token(token, code);
        code = NULL;
    }
    else
    {
        *token = Token_stack_pop(scanner_stack);
    }
    if (token->preceded_by_nl)
    {
        line_num++;
    }
    return ret;
}
int generate_token(Token *token, char *code)
{
    Token_map defined_tokens[] = {
        {"Double", TOKEN_TYPE_DOUBLE},
        {"String", TOKEN_TYPE_STRING},
        {"Int", TOKEN_TYPE_INT},
        {"Bool", TOKEN_TYPE_BOOL},
        {"Double?", TOKEN_TYPE_DOUBLE_NIL},
        {"String?", TOKEN_TYPE_STRING_NIL},
        {"Int?", TOKEN_TYPE_INT_NIL},
        {"Bool?", TOKEN_TYPE_BOOL_NIL},
        {"func", TOKEN_FUNC},
        {"nil", TOKEN_NIL},
        {"if", TOKEN_IF},
        {"else", TOKEN_ELSE},
        {"true", TOKEN_BOOL},
        {"false", TOKEN_BOOL},
        {"return", TOKEN_RETURN},
        {"while", TOKEN_WHILE},
        {"var", TOKEN_VAR},
        {"let", TOKEN_LET},
        {"break", TOKEN_BREAK},
        {"continue", TOKEN_CONTINUE},
        {"readString", TOKEN_READSTRING},
        {"readInt", TOKEN_READINT},
        {"readDouble", TOKEN_READDOUBLE},
        {"write", TOKEN_WRITE},
        {"Int2Double", TOKEN_INT2DOUBLE},
        {"Double2Int", TOKEN_DOUBLE2INT},
        {"length", TOKEN_LENGTH},
        {"substring", TOKEN_SUBSTRING},
        {"ord", TOKEN_ORD},
        {"chr", TOKEN_CHR},
    };
    token->preceded_by_nl = false;
    int code_len = 1;
    code = malloc(sizeof(char) * code_len);
    while (1)
    {
        check_length(&code_len, 0, code);
        switch (state)
        {
        /*
         * NEW_TOKEN state each time before generating new token
         */
        case NEW_TOKEN:
        {
            memset(code, '\0', strlen(code));
            char c = (char)getchar();
            switch (c)
            {
            case '\t':
            case ' ':
            case '\v':
            case '\f':
            case '\r':
                state = NEW_TOKEN;
                break;
            case '\n':
                token->preceded_by_nl = true;
                state = NEW_TOKEN;
                line_num++;
                break;
            case EOF:
                return set_token(END_STATE, "", TOKEN_EOF, token);
            case ',':
                return set_token(NEW_TOKEN, ",", TOKEN_COMMA, token);
            case ':':
                return set_token(NEW_TOKEN, ":", TOKEN_DOUBLE_DOT, token);
            case '+':
                return set_token(NEW_TOKEN, "+", TOKEN_PLUS, token);
            case '-':
                c = getchar();
                if (c == '>')
                {
                    return set_token(NEW_TOKEN, "->", TOKEN_ARROW, token);
                }
                else
                {
                    ungetc(c, stdin);
                    return set_token(NEW_TOKEN, "-", TOKEN_MINUS, token);
                }
            case '*':
                return set_token(NEW_TOKEN, "*", TOKEN_MUL, token);
            case '/':
                c = getchar();
                /*
                 * COMMENTARY starts after '//' is read from stdin
                 * COMMENTARY_BL starts after is read from stdin
                 * Any other pair or subsequent sequence means nothing else so '/' is generated as division
                 */
                if (c == '/')
                {
                    state = COMMENTARY;
                }
                else if (c == '*')
                {
                    state = COMMENTARY_BL;
                }
                else
                {
                    ungetc(c, stdin);
                    return set_token(NEW_TOKEN, "/", TOKEN_DIV, token);
                }
                break;
            case ')':
                return set_token(NEW_TOKEN, ")", TOKEN_R_BRACKET, token);
            case '(':
                return set_token(NEW_TOKEN, "(", TOKEN_L_BRACKET, token);
            case '}':
                return set_token(NEW_TOKEN, "}", TOKEN_R_CURLY, token);
            case '{':
                return set_token(NEW_TOKEN, "{", TOKEN_L_CURLY, token);
            case '?':
                c = (char)getchar();
                /*
                 * COMMENTARY starts after '//' is read from stdin
                 * TOKEN_CLOSING is generated after '?>' is read from stdin
                 * Any other pair or subsequent sequence means nothing else so '?' is generated as TOKEN_TYPE_SUFFIX
                 * and now were expecting TYPE_ID
                 */
                if (c == '?')
                {
                    return set_token(NEW_TOKEN, "??", TOKEN_BINARY_OPERATOR, token);
                }
                else
                {
                    ret = LEXICAL_ERR;
                    return LEXICAL_ERR;
                }
            case '<':
                c = (char)getchar();
                if (c == '=')
                {
                    return set_token(NEW_TOKEN, "<=", TOKEN_LESS_EQ, token);
                }
                else
                {
                    ungetc(c, stdin);
                    return set_token(NEW_TOKEN, "<", TOKEN_LESS, token);
                }
            case '>':
                c = (char)getchar();
                if (c == '=')
                {
                    return set_token(NEW_TOKEN, ">=", TOKEN_MORE_EQ, token);
                }
                else
                {
                    ungetc(c, stdin);
                    return set_token(NEW_TOKEN, ">", TOKEN_MORE, token);
                }
            case '=':
                /*
                 * After '=' is read and there is no other '=' sign after that we generate TOKEN_ASSIGN
                 * Otherwise only 3 subsequent '===' means equal so any other sequence is LEXICAL_ERR
                 */
                c = (char)getchar();
                if (c == '=')
                {
                    return set_token(NEW_TOKEN, "==", TOKEN_EQ, token);
                }
                else
                {
                    ungetc(c, stdin);
                    return set_token(NEW_TOKEN, "=", TOKEN_ASSIGN, token);
                }
            case '!':
                /*
                 * '!' at the beginning of token tas to create sequence '!==' otherwise it has no meaning and ends with LEXICAL_ERR
                 */
                c = (char)getchar();
                if (c == '=')
                {
                    return set_token(NEW_TOKEN, "!=", TOKEN_NEQ, token);
                }
                else
                {
                    ungetc(c, stdin);
                    return set_token(NEW_TOKEN, "!", TOKEN_NOT, token);
                }
            case '&':
                c = (char)getchar();
                if (c == '&')
                {
                    return set_token(NEW_TOKEN, "&&", TOKEN_AND, token);
                }
                else
                {
                    ret = LEXICAL_ERR;
                    return LEXICAL_ERR;
                }
            case '|':
                c = (char)getchar();
                if (c == '|')
                {
                    return set_token(NEW_TOKEN, "||", TOKEN_OR, token);
                }
                else
                {
                    ret = LEXICAL_ERR;
                    return LEXICAL_ERR;
                }
            case '"':
                state = STRING_1;
                break;
            case '0' ... '9':
                code[strlen(code)] = c;
                state = INTEGER;
                break;
            case 'A' ... 'Z':
            case 'a' ... 'z':
                code[strlen(code)] = c;
                state = IDENTIFICATOR;
                break;
            case '_':
                state = UNDERSCORE;
                break;
            default:
                return LEXICAL_ERR;
            }
            break;
        }
        case UNDERSCORE:
        {
            char c = (char)getchar();
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c <= '9' && c >= '0'))
            {
                check_length(&code_len, 0, code);
                code[strlen(code)] = '_';
                ungetc(c, stdin);
                state = IDENTIFICATOR;
            }
            else if (c == ' ')
            {
                return set_token(NEW_TOKEN, "_", TOKEN_UNDERSCORE, token);
            }
            else
            {
                return LEXICAL_ERR;
            }
            break;
        }
            /*
             * Everything before EOL is considered a line commentary
             */
        case COMMENTARY:
        {
            char c = (char)getchar();
            while (c != '\n' && c != EOF)
            {
                check_length(&code_len, 0, code);
                code[strlen(code)] = c;
                c = (char)getchar();
            }
            state = NEW_TOKEN;
            break;
        }
            /*
             * Block commentary has to end with sequence '*'+'/'otherwise it cannot be considered a commentary and LEXICAL_ERR is generated
             */
        case COMMENTARY_BL:
        {
            char c = (char)getchar();
            while (1)
            {
                if (c == '*')
                {
                    c = (char)getchar();
                    if (c == '/')
                    {
                        state = NEW_TOKEN;
                        break;
                    }
                    check_length(&code_len, 0, code);
                    code[strlen(code)] = c;
                }
                if (c == EOF)
                {
                    return LEXICAL_ERR;
                }
                check_length(&code_len, 0, code);
                code[strlen(code)] = c;
                c = (char)getchar();
            }
            break;
        }
            /*
             * We expect identifier to be a particular keyword,
             * builtin function or some user defined identificator
             */
        case IDENTIFICATOR:
        {
            char c = (char)getchar();
            while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_')
            {
                check_length(&code_len, 0, code);
                strncat(code, &c, 1);
                c = (char)getchar();
            }
            if(c == '?' && (strcmp(code,"Double") == 0 || strcmp(code,"Int") == 0 || strcmp(code,"String") == 0 || strcmp(code,"Bool") == 0)){
                check_length(&code_len, 0, code);
                strncat(code, &c, 1);
            }else{
                ungetc(c, stdin);
            }
            for (size_t i = 0; i < sizeof(defined_tokens) / sizeof(defined_tokens[0]); i++)
            {
                if (strcmp(code, defined_tokens[i].code) == 0)
                {
                    return set_token(NEW_TOKEN, code, defined_tokens[i].token, token);
                }
            }
            c = (char)getchar();
            if (c == '(')
            {
                ungetc(c, stdin);
                return set_token(NEW_TOKEN, code, TOKEN_FUNC_ID, token);
            }
            else
            {
                ungetc(c, stdin);
                return set_token(NEW_TOKEN, code, TOKEN_IDENTIFICATOR, token);
            }
            // return set_token(NEW_TOKEN, code, TOKEN_IDENTIFICATOR, token, code);
        }

        /*
         * TOKEN_INT is generated if integer is read and there is no dec.point subsequent it
         * TOKEN_DOUBLE is generated if dec.point and dec.part has been read
         * state = EXP_START if there is e/E read after the end of float/int
         */
        case DEC_POINT:
        case INTEGER:
        {
            if (state == INTEGER)
            {
                ungetc(code[strlen(code) - 1], stdin);
                code[strlen(code) - 1] = '\0';
            }
            char c = (char)getchar();
            while (c <= '9' && c >= '0')
            {
                if (c != '0' || *code != '\0')
                {
                    check_length(&code_len, 0, code);
                    strncat(code, &c, 1);
                }
                c = (char)getchar();
            }
            if (*code == '\0')
            {
                *code = '0';
            }
            if (c == 'e' || c == 'E')
            {
                if (state == INTEGER)
                {
                    return set_token(EXP_START, code, TOKEN_INT, token);
                }
                else
                {
                    return set_token(EXP_START, code, TOKEN_DOUBLE, token);
                }
            }
            else if (c == '.' && state == INTEGER)
            {
                check_length(&code_len, 0, code);
                strncat(code, &c, 1);
                state = DEC_POINT;
                break;
            }
            else
            {
                ungetc(c, stdin);
                if (state == INTEGER)
                {
                    return set_token(NEW_TOKEN, code, TOKEN_INT, token);
                }
                else
                {
                    return set_token(NEW_TOKEN, code, TOKEN_DOUBLE, token);
                }
            }
        }
            /*
             * E/e was read and after possible sign there has to be integer otherwise its LEXICAL_ERR
             */
        case EXP_START:
        {
            char c = (char)getchar();
            if (c == '+' || c == '-')
            {
                check_length(&code_len, 0, code);
                strncat(code, &c, 1);
                c = (char)getchar();
            }
            if (c >= '0' && c <= '9')
            {
                while (c >= '0' && c <= '9')
                {
                    if (c != '0' ||
                        (code[strlen(code) - 1] != '-' && code[strlen(code) - 1] != '+' && code[0] != '\0'))
                    {
                        check_length(&code_len, 0, code);
                        strncat(code, &c, 1);
                    }
                    c = (char)getchar();
                }
            }
            else
            {
                return LEXICAL_ERR;
            }
            if (*code == '\0')
            {
                *code = '0';
            }
            ungetc(c, stdin);
            return set_token(NEW_TOKEN, code, TOKEN_EXP, token);
        }
            /*
             * either " or """  has been read
             * Characters are read till " or """ is read again
             */
        case STRING_BLOCK:
        case STRING:
        {
            char c = (char)getchar();
            while (c != '"')
            {
                if (c == '\\')
                {
                    if (state == STRING)
                    {
                        state = STRING_ESCAPE;
                    }
                    else
                    {
                        state = STRING_BLOCK_ESCAPE;
                    }
                    break;
                }
                else if (c == EOF)
                {
                    return LEXICAL_ERR;
                }
                else
                {
                    check_length(&code_len, 0, code);
                    strncat(code, &c, 1);
                    c = '\0';
                    c = (char)getchar();
                }
            }
            if (state == STRING)
            {
                return set_token(NEW_TOKEN, code, TOKEN_STRING, token);
            }
            else if (state == STRING_BLOCK)
            {
                state = STRING_1_END;
            }
            break;
        }
            /*
             * '\' was read while in STRING state
             */
            // TODO fix realloc
        case STRING_BLOCK_ESCAPE:
        case STRING_ESCAPE:
        {
            char c = (char)getchar();
            switch (c)
            {
            case '{':
                if (state == STRING_ESCAPE)
                {
                    state = HEX_START;
                }
                else
                {
                    state = HEX_START_BLOCK;
                }
                break;
            case '\\':
                check_length(&code_len, 0, code);
                strcat(code, "\\");
                if (state == STRING_ESCAPE)
                {
                    state = STRING;
                }
                else
                {
                    state = STRING_BLOCK;
                }
                break;
            case 'n':
                check_length(&code_len, 0, code);
                strcat(code, "\n");
                if (state == STRING_ESCAPE)
                {
                    state = STRING;
                }
                else
                {
                    state = STRING_BLOCK;
                }
                break;
            case 't':
                check_length(&code_len, 0, code);
                strcat(code, "\t");
                if (state == STRING_ESCAPE)
                {
                    state = STRING;
                }
                else
                {
                    state = STRING_BLOCK;
                }
                break;
            case 'r':
                check_length(&code_len, 0, code);
                strcat(code, "\r");
                if (state == STRING_ESCAPE)
                {
                    state = STRING;
                }
                else
                {
                    state = STRING_BLOCK;
                }
                break;
            case '"':
                check_length(&code_len, 0, code);
                strcat(code, "\"");
                if (state == STRING_ESCAPE)
                {
                    state = STRING;
                }
                else
                {
                    state = STRING_BLOCK;
                }
                break;
            default:
                check_length(&code_len, 0, code);
                strcat(code, "\\");
                check_length(&code_len, 0, code);
                strncat(code, &c, 1);
                if (state == STRING_ESCAPE)
                {
                    state = STRING;
                }
                else
                {
                    state = STRING_BLOCK;
                }
                break;
            }
            break;
        }
            /* '\{' has been read
             * correct indicated whether sequence is valid hexadecimal number in range 01 - ff (case-insensitive)
             */
        case HEX_START_BLOCK:
        case HEX_START:
        {
            char hex[3];
            memset(hex, '\0', sizeof(hex));
            char c = '\0';
            bool correct = 1;
            for (int i = 0; i < 2; i++)
            {
                c = (char)getchar();
                if (!((c <= '9' && c >= '0') || (c <= 'f' && c >= 'a') || (c <= 'F' && c >= 'A')))
                {
                    correct = 0;
                    if (c == '"' || c == '\\')
                    {
                        char str[3] = "{";
                        strcat("{", hex);
                        check_length(&code_len, strlen(str), code);
                        strcat(code, str);
                        ungetc(c, stdin);
                    }
                    else
                    {
                        hex[i] = c;
                    }
                }
                else
                {
                    hex[i] = c;
                }
            }
            c = '\0';
            c = (char)getchar();
            if (c != '}')
            {
                correct = 0;
                ungetc(c, stdin);
            }
            if (correct == 1)
            {
                char str[5];
                memset(str, '\0', sizeof(str));
                sprintf(str, "0x%s", hex);
                long dec_num = strtol(str, NULL, 16);
                char dec_val[20];
                memset(dec_val, '\0', sizeof(dec_val));
                sprintf(dec_val, "%ld", dec_num);
                check_length(&code_len, strlen(dec_val), code);
                strcat(code, dec_val);
            }
            else
            {
                char str[3] = "{";
                strcat("{", hex);
                check_length(&code_len, strlen(str), code);
                strcat(code, str);
            }
            if (state == HEX_START)
            {
                state = STRING;
            }
            else
            {
                state = STRING_BLOCK;
            }
            break;
        }
        case STRING_1:
        {
            char c = (char)getchar();
            if (c == '"')
            {
                state = STRING_2;
            }
            else
            {
                ungetc(c, stdin);
                state = STRING;
            }
            break;
        }
        case STRING_2:
        {
            char c = (char)getchar();
            if (c == '"')
            {
                state = STRING_BLOCK;
            }
            else
            {
                ungetc(c, stdin);
                return set_token(NEW_TOKEN, "", TOKEN_STRING, token);
            }
            break;
        }
        case STRING_1_END:
        case STRING_2_END:
        {
            char c = (char)getchar();
            if (c == '"')
            {
                if (state == STRING_1_END)
                {
                    state = STRING_2_END;
                }
                else
                {
                    int len = strlen(code);
                    // Remove newline characters from the end of the string
                    while (len > 0 && (code[len - 1] == '\n' || code[len - 1] == '\r'))
                    {
                        code[len - 1] = '\0';
                        len--;
                    }
                    // Remove newline characters from the beginning of the string
                    int start = 0;
                    while (code[start] != '\0' && (code[start] == '\n' || code[start] == '\r'))
                    {
                        start++;
                    }
                    if (start > 0)
                    {
                        memmove(code, code + start, len - start + 1);
                    }
                    return set_token(NEW_TOKEN, code, TOKEN_STRING, token);
                }
            }
            else
            {
                return LEXICAL_ERR;
            }
            break;
        }
        default:
            return LEXICAL_ERR;
        }
    }
}
/*
 * Before each character is added to *code it reallocates memory if needed
 */
void check_length(int *code_len, int add, char *code)
{
    if (strlen(code) + add >= (long unsigned int)*code_len)
    {
        code = realloc(code, sizeof(char) * (*code_len *= 2));
        if (code == NULL)
        {
            ret = INTERNAL_ERR;
            exit(INTERNAL_ERR);
        }
    }
}
/*
 * set_token is called when token is generated successfully
 * Checks whether valid characters is next from *char_without_space
 */
int set_token(int next_state, char *val, Token_type type, Token *token)
{
    char c = (char)getchar();
    int correct = 0;
    for (int i = 0; i < (int)CHAR_WTH_SPACE_LENGTH; i++)
    {
        if (c == *char_without_space[i] || c == EOF || isspace(c) != 0 || val[strlen(val) - 1] == *char_without_space[i])
        {
            correct = 1;
        }
    }
    ungetc(c, stdin);
    if (correct == 1)
    {
        state = next_state;
        token->type = type;
        token->token_value = val;
        // printf("type:%d, value:%s\n", token->type, token->token_value);
        return 0;
    }
    else
    {
        return LEXICAL_ERR;
    }
}

void scanner_init()
{
    scanner_stack = Token_stack_init();
}

void return_token(Token token)
{
    if (token.preceded_by_nl)
    {
        line_num--;
    }
    Token_stack_push(scanner_stack, token);
}
