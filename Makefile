DEBUG_PSA?=0
DEBUG_SEMANTIC?=0
DEBUG_SYNTAX?=0
DEBUG_LEXER?=0

# Compiler and flags
CC = gcc
CFLAGS = -Iinclude -Werror -Wall -Wextra -std=c99 -fdiagnostics-color=always
TESTFLAGS = -g -D DEBUG_SEMANTIC=$(DEBUG_SEMANTIC) -D DEBUG_SYNTAX=$(DEBUG_SYNTAX) -D DEBUG_LEXER=$(DEBUG_LEXER)

# Source files (excluding main.c for test build)
SRCS = $(filter-out src/main.c, $(wildcard src/*.c)) $(filter-out src/main.c, $(wildcard src/**/*.c))

# Test files in the test directory (explicitly list your test files here)
TESTS = tests/test.c

# Target executable name
TARGET = ifjcompiler

# Test executable name
TEST_TARGET = ifjcompiler_debug

TESTFILE?=-1

# Default target
all: $(TARGET)

$(TARGET): src/main.c $(SRCS)
	$(CC) $(CFLAGS) $^ -o bin/$@
	cp bin/$(TARGET) $(TARGET)

build: $(SRCS) $(TESTS)
	@$(CC) $(CFLAGS) $^ -o bin/$(TEST_TARGET)

# Test target
test: $(SRCS) $(TESTS)
	@$(CC) $(CFLAGS) -D DEBUG_PSA=$(DEBUG_PSA) $(TESTFLAGS) $^ -o bin/$(TEST_TARGET)
	./bin/$(TEST_TARGET) <tests/test.swift

test-all: $(SRCS) tests/test.c
	bash tests/test.sh $(TESTFILE)

test-psa: $(SRCS) tests/psa/test_psa.c
	@$(CC) $(CFLAGS) -D DEBUG_PSA=1 $(TESTFLAGS) $^ -o bin/$(TEST_TARGET)
	sh tests/psa/test_psa.sh $(TESTFILE)

# clean, compile and run
run: clean all
	./bin/$(TARGET) <tests/test.swift

# Clean up
clean:
	@rm -f $(TARGET) $(TEST_TARGET)

.PHONY: all clean run test
