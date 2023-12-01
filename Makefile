DEBUG_PSA?=0
DEBUG_SEMANTIC?=0
DEBUG_SYNTAX?=0
DEBUG_LEXER?=0

# Compiler and flags
CC = gcc
CFLAGS = -Iinclude -Werror -Wall -Wextra -std=c99 -fdiagnostics-color=always
TESTFLAGS = -g -D DEBUG_SEMANTIC=$(DEBUG_SEMANTIC) -D DEBUG_SYNTAX=$(DEBUG_SYNTAX) -D DEBUG_LEXER=$(DEBUG_LEXER)

# Source files (excluding main.c for test build)
SRCS = $(filter-out main.c, $(wildcard *.c))


# Target executable name
TARGET = ifjcompiler

# Test executable name
TEST_TARGET = ifjcompiler_debug

TESTFILE?=-1

# Default target
all: $(TARGET)

$(TARGET): main.c $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@

# build the program and run with the tests/test.swift file
test: main.c $(SRCS)
	@$(CC) $(CFLAGS) -D DEBUG_PSA=$(DEBUG_PSA) $(TESTFLAGS) $^ -o $(TEST_TARGET)
	./$(TEST_TARGET) <tests/test.swift

# build the program and run with the tests/test.sh test script
test-all: main.c $(SRCS)
	@$(CC) $(CFLAGS) -D DEBUG_PSA=$(DEBUG_PSA) $(TESTFLAGS) $^ -o $(TEST_TARGET)
	bash tests/test.sh $(TESTFILE)


# clean, compile and run
run: clean all
	.$(TARGET) <tests/test.swift

# Clean up
clean:
	@rm -f $(TARGET) $(TEST_TARGET)

.PHONY: all clean run test
