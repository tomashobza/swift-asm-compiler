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
        printf "\033[42m\033[1;37m PASS \033[0m\n"
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

# Function to convert IS_OK/IS_NOT_OK to integer
convert_is_ok() {
    case "$1" in
    "IS_OK") echo $IS_OK ;;
    "IS_NOT_OK") echo $IS_NOT_OK ;;
    *)
        echo "Invalid IS_OK/IS_NOT_OK value: $1" >&2
        exit 1
        ;;
    esac
}

# Function to convert return code to integer
convert_return_code() {
    case "$1" in
    "NO_ERR") echo $NO_ERR ;;
    "LEXICAL_ERR") echo $LEXICAL_ERR ;;
    "SYNTACTIC_ERR") echo $SYNTACTIC_ERR ;;
    "FUNCTIONS_ERR") echo $FUNCTIONS_ERR ;;
    "PARAM_TYPE_ERR") echo $PARAM_TYPE_ERR ;;
    "VARIABLES_ERR") echo $VARIABLES_ERR ;;
    "RETURN_ERR") echo $RETURN_ERR ;;
    "COMPATIBILITY_ERR") echo $COMPATIBILITY_ERR ;;
    "TYPE_ERR") echo $TYPE_ERR ;;
    "SEMANTICS_ERR") echo $SEMANTICS_ERR ;;
    "INTERNAL_ERR") echo $INTERNAL_ERR ;;
    *)
        echo "Invalid return code value: $1" >&2
        exit 1
        ;;
    esac
}

# Function to convert type to integer
convert_type() {
    case "$1" in
    "TYPE_VOID") echo $TYPE_VOID ;;
    "TYPE_EMPTY") echo $TYPE_EMPTY ;;
    "TYPE_INVALID") echo $TYPE_INVALID ;;
    "TYPE_INT") echo $TYPE_INT ;;
    "TYPE_DOUBLE") echo $TYPE_DOUBLE ;;
    "TYPE_STRING") echo $TYPE_STRING ;;
    "TYPE_BOOL") echo $TYPE_BOOL ;;
    "TYPE_NIL") echo $TYPE_NIL ;;
    "TYPE_INT_NIL") echo $TYPE_INT_NIL ;;
    "TYPE_DOUBLE_NIL") echo $TYPE_DOUBLE_NIL ;;
    "TYPE_STRING_NIL") echo $TYPE_STRING_NIL ;;
    "TYPE_BOOL_NIL") echo $TYPE_BOOL_NIL ;;
    *)
        echo "Invalid type value: $1" >&2
        exit 1
        ;;
    esac
}

extract_expected_values() {
    local testfile=$1
    read -r first_line <"$testfile"

    # Use bash regex to extract the values
    if [[ $first_line =~ \/\/\ *([^\ ]+)\ *([^\ ]+)\ *([^\ ]+) ]]; then
        EXPECTED_IS_OK=$(convert_is_ok ${BASH_REMATCH[1]})
        EXPECTED_TYPE=$(convert_type ${BASH_REMATCH[2]})
        EXPECTED_RETURN_CODE=$(convert_return_code ${BASH_REMATCH[3]})
    else
        echo "Error: First line of test file is not in expected format."
        exit 1
    fi
}

test_file() {
    local testinput="$(dirname "$0")/tests/$1"

    echo testinput=$testinput

    extract_expected_values "$testinput"

    "$(dirname "$0")"/../../bin/ifjcompiler_debug "$EXPECTED_IS_OK" "$EXPECTED_TYPE" <"$testinput" >/dev/null
    RETURN_CODE=$?
    print_test_name "01.swift"
    print_error $RETURN_CODE "$EXPECTED_RETURN_CODE"
}

###############
#### TESTS ####
###############

# specific test
if [ "$TESTFILE" != "-1" ]; then
    test_file "$TESTFILE"
    exit 0
fi

# test 01
test_file "01.swift"

print_sep # separator

# test 02
test_file "02.swift"
