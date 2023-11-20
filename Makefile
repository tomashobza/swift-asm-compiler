# Compiler and flags
CC = gcc
CFLAGS = -Iinclude
TESTFLAGS = -g -fdiagnostics-color=always -D DEBUG=1

# Source files (excluding main.c for test build)
SRCS = $(filter-out src/main.c, $(wildcard src/*.c))

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
	$(CC) $(CFLAGS) $(TESTFLAGS) $^ -o bin/$(TEST_TARGET)

# Test target
test: $(SRCS) $(TESTS)
	$(CC) $(CFLAGS) $(TESTFLAGS) $^ -o bin/$(TEST_TARGET)
	# ./bin/$(TEST_TARGET) <tests/test.in

# clean, compile and run
run: clean all
	./bin/$(TARGET)

# Clean up
clean:
	rm -f $(TARGET) $(TEST_TARGET)

.PHONY: all clean run test
