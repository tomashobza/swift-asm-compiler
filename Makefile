DEBUG_PSA?=0
DEBUG_SEMANTIC?=0
DEBUG_SYNTAX?=0
DEBUG_LEXER?=0

# Compiler and flags
CC = gcc
CFLAGS = -Iinclude -Werror -Wall -Wextra -std=c99 -fdiagnostics-color=always
MAINFLAGS = -Wunused-function -Wunused-variable -Wunused-value -Wunused-label -Wunused-parameter -Wunused-but-set-variable -Wunused-but-set-parameter -Wunused-result
TESTFLAGS = -g -D DEBUG_SEMANTIC=$(DEBUG_SEMANTIC) -D DEBUG_SYNTAX=$(DEBUG_SYNTAX) -D DEBUG_LEXER=$(DEBUG_LEXER)

# Source files (excluding main.c for test build)
SRCS = $(filter-out main.c, $(wildcard *.c))


# Target executable name
TARGET = ifjcompiler

# Test executable name
TEST_TARGET = ifjcompiler_debug

PEPEGA_TESTS = compiler_tests.py

TESTFILE?=-1

# Default target
all: $(TARGET)

$(TARGET): main.c $(SRCS)
	$(CC) $(CFLAGS) $(MAINFLAGS) $^ -o $@

# build the program and run with the tests/test.swift file
test: main.c $(SRCS)
	@$(CC) $(CFLAGS) -D DEBUG_PSA=$(DEBUG_PSA) $(TESTFLAGS) $^ -o $(TEST_TARGET)
	./$(TEST_TARGET) <tests/test.swift
	@./$(TEST_TARGET) <tests/test.swift > tests/test_out.ifjcode
	./utils/ic23int tests/test_out.ifjcode


# build the program and run with the tests/test.sh test script
test-all: main.c $(SRCS)
	@$(CC) $(CFLAGS) -D DEBUG_PSA=$(DEBUG_PSA) $(TESTFLAGS) $^ -o $(TEST_TARGET)
	bash tests/test.sh $(TESTFILE)

test-blaza: main.c $(SRCS)
	@$(CC) $(CFLAGS) -D DEBUG_PSA=$(DEBUG_PSA) $(TESTFLAGS) $^ -o $(TEST_TARGET)
	bash tests/test-blaza.sh $(TESTFILE)

test-allall: main.c $(SRCS)
	@$(CC) $(CFLAGS) -D DEBUG_PSA=$(DEBUG_PSA) $(TESTFLAGS) $^ -o $(TEST_TARGET)
	bash tests/test.sh $(TESTFILE)
	bash tests/test-blaza.sh $(TESTFILE)

# build the program and run with the tests/ugly_tests_generator/test_ugly.sh test script
test-ugly: main.c $(SRCS)
	@$(CC) $(CFLAGS) -D DEBUG_PSA=$(DEBUG_PSA) $(TESTFLAGS) $^ -o $(TEST_TARGET)
	bash tests/ugly_tests_generator/test_ugly.sh ./tests/ugly_tests_generator/gen ./$(TEST_TARGET) ./tests/ugly_tests_generator/ugly_test.out


pepe: main.c $(SRCS)
	@$(CC) $(CFLAGS) -D DEBUG_PSA=$(DEBUG_PSA) $(TESTFLAGS) $^ -o $(TEST_TARGET)
	cd ./tests/pepega_tests/ && \
	python3 $(PEPEGA_TESTS) ./$(TEST_TARGET)

# clean, compile and run
run: clean all
	.$(TARGET) <tests/test.swift

# Clean up
clean:
	@rm -f $(TARGET) $(TEST_TARGET)

.PHONY: all clean run test
