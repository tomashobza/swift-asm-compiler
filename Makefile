DEBUG_PSA?=0
DEBUG_SEMANTIC?=0
DEBUG_SYNTAX?=0
DEBUG_LEXER?=0

# Compiler and flags
CC = gcc
CFLAGS = -Iinclude -Werror -Wall -Wextra -std=c99 -fdiagnostics-color=always
TESTFLAGS = -g -DDEBUG_PSA=$(DEBUG_PSA) -DDEBUG_SEMANTIC=$(DEBUG_SEMANTIC) -DDEBUG_SYNTAX=$(DEBUG_SYNTAX) -DDEBUG_LEXER=$(DEBUG_LEXER)

# Source files (excluding main.c for test build)
SRCS = $(filter-out src/main.c, $(wildcard src/*.c)) $(filter-out src/main.c, $(wildcard src/**/*.c))

# Test files in the test directory (explicitly list your test files here)
TESTS = tests/test.c

# Target executable name
TARGET = ifjcompiler

# Test executable name
TEST_TARGET = ifjcompiler_debug

# TEST_INPUT?=tests/test.in

# Default target
all: $(TARGET)

$(TARGET): src/main.c $(SRCS)
	$(CC) $(CFLAGS) $^ -o bin/$@

build: $(SRCS) $(TESTS)
	@$(CC) $(CFLAGS) $(TESTFLAGS) $^ -o bin/$(TEST_TARGET)

# Test target
test: $(SRCS) $(TESTS)
	@$(CC) $(CFLAGS) $(TESTFLAGS) $^ -o bin/$(TEST_TARGET)
	./bin/$(TEST_TARGET) <tests/test.swift

# clean, compile and run
run: clean all
	./bin/$(TARGET) <tests/test.swift

# Clean up
clean:
	@rm -f $(TARGET) $(TEST_TARGET)

.PHONY: all clean run test
