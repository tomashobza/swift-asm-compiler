# Compiler and flags
CC = gcc
CFLAGS = -Iinclude

# Source files (excluding main.c for test build)
SRCS = $(filter-out src/main.c, $(wildcard src/*.c))

# Test files in the test directory (explicitly list your test files here)
TESTS = tests/test.c

# Target executable name
TARGET = ifjcompiler

# Test executable name
TEST_TARGET = test_runner

# Default target
all: $(TARGET)

$(TARGET): src/main.c $(SRCS)
	$(CC) $(CFLAGS) $^ -o bin/$@

# Test target
test: $(SRCS) $(TESTS)
	$(CC) $(CFLAGS) $^ -o bin/$(TEST_TARGET)
	./bin/$(TEST_TARGET) <tests/test.in

# clean, compile and run
run: clean all
	./bin/$(TARGET)

# Clean up
clean:
	rm -f $(TARGET) $(TEST_TARGET)

.PHONY: all clean run test
