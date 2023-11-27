#!/bin/bash

TESTFILE=$1

## ENUM ##
# is_ok
IS_OK=1
IS_NOT_OK=0

NO_ERR=0
LEXICAL_ERR=1
SYNTACTIC_ERR=2
FUNCTIONS_ERR=3
PARAM_TYPE_ERR=4
VARIABLES_ERR=5
RETURN_ERR=6
COMPATIBILITY_ERR=7
TYPE_ERR=8
SEMANTICS_ERR=9
INTERNAL_ERR=99

# Function to print error message
print_error() {
    local error_code=$1
    local should_be_error_code=$2

    if [ "$error_code" != "$should_be_error_code" ]; then
        printf "\033[41m\033[1;37m FAIL \033[0m"
        echo " \033[0;31m[expected: $should_be_error_code, got: $error_code]\033[0m"
    else
        echo "\033[42m\033[1;37m PASS \033[0m"
    fi

    echo
}

print_test_name() {
    local test_name=$1
    printf "\n\033[46m\033[1;30m $test_name: \033[0m "
}

print_sep() {
    # Get the width of the terminal window
    local width=$(tput cols)

    # Print '=' characters to fill the width
    printf '%*s\n' "$width" '' | tr ' ' '='
}

# expression types
TYPE_VOID=-3
TYPE_EMPTY=-2
TYPE_INVALID=-1
TYPE_INT=0
TYPE_DOUBLE=1
TYPE_STRING=2
TYPE_BOOL=3
TYPE_NIL=4
TYPE_INT_NIL=5
TYPE_DOUBLE_NIL=6
TYPE_STRING_NIL=7
TYPE_BOOL_NIL=8

# if testfile is not -1, run the testfile and return
if [ "$TESTFILE" != "-1" ]; then
    $(dirname $0)/../../bin/ifjcompiler_debug <tests/psa/tests/"$TESTFILE"
    RETURN_CODE=$?
    print_test_name "$TESTFILE"
    print_error $RETURN_CODE
    exit 0
fi

###############
#### TESTS ####
###############

# test 01
$(dirname $0)/../../bin/ifjcompiler_debug $IS_NOT_OK $TYPE_INVALID <tests/psa/tests/01.swift >/dev/null
RETURN_CODE=$?
print_test_name "01.swift"
print_error $RETURN_CODE $SEMANTICS_ERR

print_sep # separator

# test 02
$(dirname $0)/../../bin/ifjcompiler_debug $IS_OK $TYPE_INT <tests/psa/tests/02.swift >/dev/null
RETURN_CODE=$?
print_test_name "02.swift"
print_error $RETURN_CODE $NO_ERR
