#include "psa.h"

PSA_Token parseFunctionCall(PSA_Token id, symtable_stack *st_stack)
{
    // check if the id of the function is in the symtable
    // if yes, save the symbol
    symtable_item *found_func = symtable_find_in_stack(id.token_value, st_stack);
    if (found_func == NULL)
    {
        printf("Function %s not found!\n", id.token_value);
        // TODO: if not -> error (for now)
        return (PSA_Token){
            .type = TOKEN_EXPRSN,
            .token_value = "E",
            .canBeNil = false,
            .expr_type = TYPE_INVALID,
            .preceded_by_nl = false};
    }

    printf("Function %s found\n", id.token_value);

    // check if next token is (
    PSA_Token next_token = readNextToken(NULL, NULL);
    if (next_token.type != TOKEN_L_BRACKET)
    {
        return (PSA_Token){
            .type = TOKEN_EXPRSN,
            .token_value = "E",
            .canBeNil = false,
            .expr_type = TYPE_INVALID,
            .preceded_by_nl = false};
    }

    // initialize a stack of type PSA_Token
    // push the id token to the stack
    PSA_Token_stack *s = PSA_Token_stack_init();
    // PSA_Token_stack_push(s, id);

    // read the next n tokens (n being the number of function parameters) or until ) token is read
    psa_return_type parsed_param = parse_expression_param(st_stack);
    next_token = (PSA_Token){
        .type = parsed_param.end_token,
        .token_value = "E",
        .expr_type = parsed_param.type,
        .canBeNil = parsed_param.canBeNil,
        .preceded_by_nl = false};
    // TODO: change read next token for parameters

    int counter = 0;
    while (next_token.expr_type != TYPE_EMPTY && counter < found_func->data.func_data->params_count)
    {
        PSA_Token_stack_push(s, next_token);
        parsed_param = parse_expression_param(st_stack);
        next_token = (PSA_Token){
            .type = parsed_param.end_token,
            .token_value = "E",
            .expr_type = parsed_param.type,
            .canBeNil = parsed_param.canBeNil,
            .preceded_by_nl = false};
        counter++;
    }

    // for each parameter print out the tokens making it

    // if the number of read tokens is not n or the next token isn't ), the wrong number of parameters was provided -> error

    return (PSA_Token){
        .type = TOKEN_EXPRSN,
        .token_value = "E",
        .canBeNil = false,
        .expr_type = TYPE_INVALID,
        .preceded_by_nl = false};
}