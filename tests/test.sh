#!/bin/bash


TESTFILE=$1

## ENUM ##

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

convert_code_to_string() {
    case "$1" in
    "$NO_ERR") echo "NO_ERR" ;;
    "$LEXICAL_ERR") echo "LEXICAL_ERR" ;;
    "$SYNTACTIC_ERR") echo "SYNTACTIC_ERR" ;;
    "$FUNCTIONS_ERR") echo "FUNCTIONS_ERR" ;;
    "$PARAM_TYPE_ERR") echo "PARAM_TYPE_ERR" ;;
    "$VARIABLES_ERR") echo "VARIABLES_ERR" ;;
    "$RETURN_ERR") echo "RETURN_ERR" ;;
    "$COMPATIBILITY_ERR") echo "COMPATIBILITY_ERR" ;;
    "$TYPE_ERR") echo "TYPE_ERR" ;;
    "$SEMANTICS_ERR") echo "SEMANTICS_ERR" ;;
    "$INTERNAL_ERR") echo "INTERNAL_ERR" ;;
    *)
        echo "Invalid error code value: $1" >&2
        exit 1
        ;;
    esac
}

# Function to print error message
print_error() {
    local error_code=$1
    local should_be_error_code=$2

    if [ "$error_code" != "$should_be_error_code" ]; then
        printf "\033[41m\033[1;37m You shall FAIL! \033[0m"
        echo " \033[0;31m[expected: $(convert_code_to_string $should_be_error_code), got: $(convert_code_to_string $error_code)]\033[0m"
    else
        printf "\033[42m\033[1;37m You shall PASS! \033[0m\n"
    fi
}

print_test_name() {
    local test_name=$1
    printf "\033[46m\033[1;30m $test_name: \033[0m "
}

extract_expected_values() {
    local testfile=$1
    read -r first_line <"$testfile"

    # Use bash regex to extract the values
    if [[ $first_line =~ \/\/\ *([^\s]+) ]]; then
        EXPECTED_RETURN_CODE=$(convert_return_code ${BASH_REMATCH[1]})
    else
        echo "Error: First line of test file is not in expected format."
        exit 1
    fi
}

test_file() {
    local testinput="$(dirname "$0")/tests/$1"

    extract_expected_values "$testinput"

    # if should_output is empty, set it to 0

    if [ -z "$2" ]; then
        "$(dirname "$0")"/../ifjcompiler_debug >/dev/null 2>/dev/null <"$testinput"
        RETURN_CODE=$?
    else
        "$(dirname "$0")"/../ifjcompiler_debug <"$testinput"
        RETURN_CODE=$?
    fi

    print_test_name "$1"
    print_error $RETURN_CODE "$EXPECTED_RETURN_CODE"
}

###############
#### TESTS ####
###############

# specific test
if [ "$TESTFILE" != "-1" ]; then
    test_file "$TESTFILE" "1"
    exit 0
fi

# test 01

# call test_file with test file name for each .swift file in tests/
for testfile in "$(dirname "$0")"/tests/*.swift; do
    test_file "$(basename "$testfile")"
done

echo "                                  ....							"
echo "                                .'' .'''							"
echo ".                             .'   :								"
echo "\\                          .:    :								"
echo " \\                        _:    :       ..----.._			    "
echo "  \\                    .:::.....:::.. .'         ''.				"	
echo "   \\                 .'  #-. .-######'     #        '.			"
echo "    \\                 '.##'/ ' ################       :			"
echo "     \\                  #####################         :			"
echo "      \\               ..##.-.#### .''''###'.._        :			"
echo "       \\             :--:########:            '.    .' :			"
echo "        \\..__...--.. :--:#######.'   '.         '.     :			"
echo "        :     :  : : '':'-:'':'::        .         '.  .'			"
echo "        '---'''..: :    ':    '..'''.      '.        :'			"
echo "           \\  :: : :     '      ''''''.     '.      .:			"
echo "            \\ ::  : :     '            '.      '      :			"
echo "             \\::   : :           ....' ..:       '     '.		"
echo "              \\::  : :    .....####\\ .~~.:.             :		"
echo "               \\':.:.:.:'#########.===. ~ |.'-.   . '''.. :		"
echo "                \\    .'  ########## \ \ _.' '. '-.       '''.	"
echo "                :\\  :     ########   \ \      '.  '-.        :	"
echo "               :  \\'    '   #### :    \ \      :.    '-.      :	"
echo "              :  .'\\   :'  :     :     \ \       :      '-.    :	"
echo "             : .'  .\\  '  :      :     :\ \       :        '.   :"
echo "             ::   :  \\'  :.      :     : \ \      :          '. :"
echo "             ::. :    \\  : :      :    ;  \ \     :           '.:"
echo "              : ':    '\\ :  :     :     :  \:\     :        ..'	"
echo "                 :    ' \\ :        :     ;  \|      :   .'''		"
echo "                 '.   '  \\:                         :.''			"
echo "                  .:..... \\:       :            ..''				"
echo "                 '._____|'.\\......'''''''.:..'''					"
echo "                            \\                                    "