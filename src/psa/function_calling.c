#include "psa.h"

PSA_Token parseFunctionCall(PSA_Token_stack *main_s, PSA_Token id)
{
    PSA_Token ERROR_TOKEN = (PSA_Token){
        .type = TOKEN_EXPRSN,
        .token_value = id.token_value,
        .canBeNil = false,
        .expr_type = TYPE_INVALID,
        .preceded_by_nl = false};

    bool is_ok = true;

    // check if the id of the function is in the symtable
    symtable_item *found_func = symtable_find_in_stack(id.token_value, sym_st);
    if (found_func == NULL)
    {
        is_ok = false;

        // TODO: if not -> error (for now), call the function checking the rest of the input source

        throw_error(FUNCTIONS_ERR, "Function '%s' not found!", id.token_value);
    }

    printf("Function '%s' found\n", id.token_value);

    // read the next token (should be ( token)
    char next_token_error = 0;
    if (readNextToken(main_s, &next_token_error, NULL).type != TOKEN_L_BRACKET)
    {
        throw_error(SYNTACTIC_ERR, "Missing '(' after function name!");

        is_ok = false;

        return ERROR_TOKEN;
    }

    // parse the next n parameters using parse_expression_param

    unsigned int param_counter = 0;
    bool params_ok = true;
    psa_return_type parsed_param;

    while (found_func == NULL || param_counter < (unsigned int)found_func->data.func_data->params_count)
    {
        params_ok = params_ok && checkParameter(main_s, param_counter, found_func, &parsed_param);

        // TODO: save parameters for later checking if the function is not in the symtable

        param_counter++;

        // parameter will be empty if the next token is a ) token (end of the parameter list)
        if (parsed_param.type == TYPE_EMPTY || parsed_param.type == TYPE_INVALID)
        {
            break;
        }
    }
    // read the next token (should be ) token)
    if (readNextToken(main_s, &next_token_error, NULL).type != TOKEN_R_BRACKET)
    {
        // if not -> error
        // TODO: throw error
        is_ok = false;
    }

    // TODO:check if the correct number of parameters was provided

    is_ok = is_ok && params_ok;

    if (is_ok)
    {
        return (PSA_Token){
            .type = TOKEN_EXPRSN,
            .token_value = found_func->id,
            .canBeNil = false,
            .expr_type = found_func->data.func_data->return_type,
            .preceded_by_nl = id.preceded_by_nl};
    }

    return ERROR_TOKEN;
}

bool checkParameter(PSA_Token_stack *main_s, unsigned int param_index, symtable_item *found_func, psa_return_type *parsed_param)
{
    // TODO: handle param errors

    bool name_ok = checkParamName(main_s, param_index, found_func);

    printf("The name is: %s\n", name_ok ? "ok" : "not ok");

    (*parsed_param) = parse_expression_param();

    printf("param[%d] has type: ", param_index);
    print_expression_type((*parsed_param).type);
    printf("\n");

    if (found_func == NULL)
    {
        // TODO: save for later checking
        return true;
    }

    return (*parsed_param).is_ok && (*parsed_param).type == found_func->data.func_data->params[param_index].type && name_ok;
}

bool checkParamName(PSA_Token_stack *main_s, unsigned int param_index, symtable_item *found_func)
{
    // read the first token (should be an identificator)
    char next_token_error = 0;
    PSA_Token id = readNextToken(main_s, &next_token_error, NULL);

    // read the second token (should be : token)
    PSA_Token colon = readNextToken(main_s, &next_token_error, NULL);

    bool has_name = id.type == TOKEN_IDENTIFICATOR && colon.type == TOKEN_DOUBLE_DOT;

    // if the tokens don't match the pattern of a parameter name, return them
    if (!has_name)
    {
        return_token(convertPSATokenToToken(colon));
        return_token(convertPSATokenToToken(id));
    }

    if (found_func == NULL)
    {
        // TODO: save for later checking
        return true;
    }

    bool should_have_name = param_index < (unsigned int)found_func->data.func_data->params_count && strcmp(found_func->data.func_data->params[param_index].name, "_");

    bool name_is_ok = false;

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