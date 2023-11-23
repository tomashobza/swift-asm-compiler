#!/bin/bash

COMPILER=../bin/ifjcompiler
TEST_DIR=./sys_tests
RESULT_DIR=./sys_results

echo "Running system tests..."
for testfile in $(ls $TEST_DIR/*.in); do
    base=$(basename $testfile .in)
    expected_output=$TEST_DIR/$base.out
    actual_output=$RESULT_DIR/$base.result
    
    echo "Running test $base..."
    $COMPILER < $testfile
    #if cmp -s $expected_output $actual_output; then
    #    echo "Test $base PASSED"
    #else
    #    echo "Test $base FAILED"
    #fi
done
