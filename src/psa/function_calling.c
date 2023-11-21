#include "psa.h"

PSA_Token parseFunctionCall(PSA_Token_stack *main_s, PSA_Token id, symtable_stack *st_stack)
{
    // check if the id of the function is in the symtable
    symtable_item *found_func = symtable_find_in_stack(id.token_value, st_stack);
    if (found_func == NULL)
    {
        // if yes, save the symbol
        printf("Function %s not found!\n", id.token_value);
        // TODO: if not -> error (for now)
        return (PSA_Token){
            .type = TOKEN_EXPRSN,
            .token_value = "E",
            .canBeNil = false,
            .expr_type = TYPE_INVALID,
            .preceded_by_nl = false};
    }

    printf("Function '%s' found\n", id.token_value);

    // TODO: check if function should have parameters
    if (found_func->data.func_data->params_count == 0 || found_func->data.func_data->params == NULL)
    {
        // TODO: handle function without parameters
    }

    // read the next token (should be ( token)
    char next_token_error = 0;
    if (readNextToken(main_s, &next_token_error).type != TOKEN_L_BRACKET)
    {
        // if not -> error
        // TODO: throw error
    }

    // parse the next n parameters using parse_expression_param

    int param_counter = 0;
    psa_return_type parsed_param;

    do
    {
        printf("The name is: %s\n", checkParamName(main_s, param_counter, found_func) ? "ok" : "not ok");
        psa_return_type parsed_param = parse_expression_param(st_stack);
        printf("param[%d] has type: ", param_counter);
        print_expression_type(parsed_param.type);
        printf("\n");
    } while (parsed_param.end_token != TOKEN_R_BRACKET);

    // PSA_Token next_token = (PSA_Token){
    //     .type = parsed_param.end_token,
    //     .token_value = "E",
    //     .expr_type = parsed_param.type,
    //     .canBeNil = parsed_param.canBeNil,
    //     .preceded_by_nl = false};

    // for each parameter print out the tokens making it

    // if the number of read tokens is not n or the next token isn't ), the wrong number of parameters was provided -> error

    return (PSA_Token){
        .type = TOKEN_EXPRSN,
        .token_value = "E",
        .canBeNil = false,
        .expr_type = TYPE_INVALID,
        .preceded_by_nl = false};
}

bool checkParamName(PSA_Token_stack *main_s, unsigned int param_index, symtable_item *found_func)
{
    // read the first token (should be an identificator)
    char next_token_error = 0;
    PSA_Token id = readNextToken(main_s, &next_token_error);

    // read the second token (should be : token)
    PSA_Token colon = readNextToken(main_s, &next_token_error);

    bool has_name = id.type == TOKEN_IDENTIFICATOR && colon.type == TOKEN_DOUBLE_DOT;
    bool should_have_name = !strcmp(found_func->data.func_data->params[param_index].name, "_");

    bool name_is_ok = false;

    // if the tokens don't match the pattern of a parameter name, return them
    if (!has_name)
    {
        return_token(convertPSATokenToToken(colon));
        return_token(convertPSATokenToToken(id));
    }

    /*
        SEMANTIC RULES FOR PARAMETER NAME:
        1. should_have_name = true, has_name = true -> ok
        2. should_have_name = true, has_name = false -> error
        3. should_have_name = false, has_name = true -> error
        4. should_have_name = false, has_name = false -> ok
    */

    // TODO: handle all 4 error states
    if (has_name != should_have_name)
    {
        // 2. should have name, does not have name -> error
        // or
        // 3. should not have name, does have name -> error
        name_is_ok = false;
    }
    else
    {
        if (!should_have_name) // 4. should not have name, does not have name -> ok
        {
            name_is_ok = true;
        }
        else // 1. should have name, does have name -> ok
        {
            // check if the parameter name is correct according to the symtable
            if (!strcmp(found_func->data.func_data->params[param_index].id, id.token_value))
            {
                // the name is correct
                name_is_ok = true;
            }
        }
    }

    return name_is_ok;
}